.PHONY: all configure build clean debug release
BUILD_DIR = build
EXECUTABLE_NAME = EscapeTheFate
DEFAULT_GENERATOR ?= "Ninja"
BACKUP_GENERATOR ?= "Unix Makefiles"
WINDOWS_GENERATOR ?= "Visual Studio 18 2026"
APPLE_GENERATOR ?= Xcode
CONFIGURE_COMMAND ?= "cmake"
EMSCRIPTEN_CONFIGURE_COMMAND = "emcmake cmake"
IMGUI_DEBUGGING ?= ON
STEAM_ENABLED ?= OFF
STEAM_SDK_DIR ?=
STEAM_APPID_FILE ?= ON
BUILD_TYPE ?= Debug
SYSTEM_PACKAGES ?= ON
ENGINE_CACHED ?= ON
GAME_VERSION ?= 0.2.0
BUILD_COMMAND ?= cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)
PACKAGE_COMMAND ?= cpack --config build/CPackConfig.cmake -C $(BUILD_TYPE)
ADDITIONAL_OPTIONS ?=
ADDITIONAL_BUILD_COMMANDS ?=
IOS_BUILD_COMMANDS = "-- -allowProvisioningUpdates -allowProvisioningDeviceRegistration -jobs 8"
SGFORGE ?= sgforge
# default, should be used after a rebuild of some sort.
UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Darwin)
REBUILD := mrebuild
STEAM_REBUILD := msteamrebuild
# Run from the executable, cause it shows proper debug info
RUN_CMD := ./build/Debug/EscapeTheFate.app/Contents/MacOS/EscapeTheFate
else ifeq ($(UNAME_S),Linux)
REBUILD := lrebuild
STEAM_REBUILD := bsteamrebuild
RUN_CMD := ./build/$(EXECUTABLE_NAME)
else
REBUILD := lrebuild
STEAM_REBUILD := bsteamrebuild
endif

.PHONY: all
# -DCMAKE_POLICY_VERSION_MINIMUM=3.5 use this if we are using past version 4.0
all: pack build run
clean:
	@rm -rf $(BUILD_DIR)
configure:
	$(CONFIGURE_COMMAND) -DCMAKE_POLICY_VERSION_MINIMUM=3.5  -G "$(CMAKE_GENERATOR)" . -B $(BUILD_DIR) -DENGINE_CACHED=$(ENGINE_CACHED) -DIMGUI_DEBUGGING=$(IMGUI_DEBUGGING) -DSYSTEM_PACKAGES=$(SYSTEM_PACKAGES) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DGAME_VERSION=$(GAME_VERSION) $(ADDITIONAL_OPTIONS)  -DLINK_M=$(LINK_M) -DSTEAM_ENABLED=$(STEAM_ENABLED) -DSTEAM_APPID_FILE=$(STEAM_APPID_FILE) $(if $(STEAM_SDK_DIR),-DSTEAM_SDK_DIR=$(STEAM_SDK_DIR))
build:
	@$(BUILD_COMMAND) $(ADDITIONAL_BUILD_COMMANDS)
install:
	@cmake --install $(BUILD_DIR) --config $(BUILD_TYPE) --prefix $(BUILD_DIR)/install
run:
	@$(RUN_CMD)

debug: build
	@lldb  $(RUN_CMD)

package:
	$(PACKAGE_COMMAND)

# Custom build commands that set variables accordingly based on platform.. rebuild is macos, brebuild is backup, wrebuild is windows, erebuild is emscripten, irebuild is ios simulator
rebuild:
	@$(MAKE) $(REBUILD) 
mrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) clean configure build
lrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) LINK_M=ON clean configure build
xrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR)  ADDITIONAL_OPTIONS="-DDISABLE_WERROR=YES" clean configure build devsign package
brebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) clean configure build package
wrebuild:
	$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) configure build package
steamrebuild:
	@$(MAKE) $(STEAM_REBUILD)
msteamrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) STEAM_ENABLED=ON STEAM_APPID_FILE=OFF clean configure build install
xsteamrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) STEAM_ENABLED=ON STEAM_APPID_FILE=OFF clean configure build install
wsteamrebuild:
	$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) STEAM_ENABLED=ON STEAM_APPID_FILE=OFF configure build install
bsteamrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) STEAM_ENABLED=ON STEAM_APPID_FILE=OFF clean configure build install
erebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) CONFIGURE_COMMAND=$(EMSCRIPTEN_CONFIGURE_COMMAND) clean configure build
isim:
	$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE -DDISABLE_WERROR=YES" clean configure build package
ios:
	$(MAKE) \
		CMAKE_GENERATOR=$(APPLE_GENERATOR) \
		ADDITIONAL_BUILD_COMMANDS=$(IOS_BUILD_COMMANDS) \
		ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE" \
		IMGUI_DEBUGGING=OFF \
		clean configure build package
xopen:
	@open build/*.xcodeproj
erun:
	@emrun --no_browser --port 6931 ./build/EscapeTheFate.html

.PHONY: ios isim xopen

#Sign before we package
devsign:
	@codesign --force --deep --sign - --entitlements cmake/EscapeTheFate.entitlements build/$(BUILD_TYPE)/EscapeTheFate.app
# Used when you want to run instruments when not using xcode to build (local dev)
codesign:
	# @codesign --force --deep --sign - --entitlements cmake/EscapeTheFate.entitlements ./build/Debug/EscapeTheFate.app/Contents/MacOS/EscapeTheFate
	@codesign --force --deep --sign - --entitlements cmake/EscapeTheFate.entitlements ./build/Debug/EscapeTheFate.app

# This will error if you are using asan if you have leaks, so maybe disable that.
perf:
	@perf record -F 99 -g -- $(RUN_CMD) && perf script > out.perf && stackcollapse-perf.pl out.perf > out.folded && flamegraph.pl out.folded > test.svg && firefox test.svg

teamid:
	@security find-certificate -c "Apple Development" -p | openssl x509 -inform pem -noout -subject

# spctl --assess --verbose=4 Release/EscapeTheFate.app
# Release/EscapeTheFate.app: rejected
#
#spctl --assess --verbose=4 --raw Release/EscapeTheFate.app
# check signature, this is good
# codesign -vvv --deep --strict Release/EscapeTheFate.app
# Release/EscapeTheFate.app: valid on disk
# Release/EscapeTheFate.app: satisfies its Designated Requirement
# Check for quarantines
# xattr -lr Release/EscapeTheFate.app
# if there is, run command
# xattr -dr com.apple.quarantine Release/EscapeTheFate.app
#
#variables for packing
DIRS := ./assets/audio/bgm ./assets/audio/sfx ./assets/img ./assets/aseprite ./assets/battle ./assets/config  ./assets/dialog ./assets/ui ./assets/fonts ./assets/shaders ./assets/tiled/templates ./assets/tiled
#./assets/tiled
#./assets/tiled/templates
# FILES := $(filter-out %.aseprite,$(foreach d,$(DIRS),$(wildcard $(d)/*)))
# FILES := $(shell find $(DIRS) -type f)
FILES := $(shell find $(DIRS) -type f \
    ! -name "*.aseprite" \
    ! -name "*.tmx" \
    ! -name "*.tsx")


ALL_FILES_STRING := $(foreach f,$(FILES),$(f) )
pack:
	@$(SGFORGE) $(ALL_FILES_STRING) -o etf.sg

# =========================================================
# Android Build & Deploy
# =========================================================
# FULL SETUP (run these commands on the build machine):
#
#   # 1. Install command-line tools (no Android Studio GUI needed)
#   brew install --cask android-commandlinetools
#   brew install --cask temurin@17
#   brew install gradle
#
#   # 2. Install SDK components via sdkmanager
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "platform-tools"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "platforms;android-34"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "build-tools;34.0.0"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "ndk;27.0.12077973"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "cmake;3.31.6"
#
#   # 3. Add to ~/.zshrc (or ~/.bashrc):
#   echo 'export ANDROID_HOME=$HOME/Library/Android/sdk' >> ~/.zshrc
#   echo 'export ANDROID_NDK_HOME=$ANDROID_HOME/ndk/27.0.12077973' >> ~/.zshrc
#   echo 'export PATH=$PATH:$ANDROID_HOME/platform-tools' >> ~/.zshrc
#   echo 'export PATH=$PATH:$ANDROID_HOME/cmdline-tools/latest/bin' >> ~/.zshrc
#   source ~/.zshrc
#
#   # 4. Generate gradle wrapper (one-time)
   # cd android && gradle wrapper && cd ..
#
# WORKFLOW:
#   make pack                  # Build etf.sg asset pack
#   make android-setup         # One-time: fetch SDL3 Java sources
#   make android-build         # Build debug APK
#   make android-install       # Push to device
#   make android-run           # Launch app
#   make android-logcat        # View logs
#   make android-debug         # Attach native debugger
# =========================================================

ANDROID_BUILD_DIR ?= build-android
ANDROID_BUILD_TYPE ?= debug
ANDROID_PACKAGE = com.supergoon.rpg
ANDROID_ACTIVITY = $(ANDROID_PACKAGE)/.EscapeTheFateActivity
ANDROID_APK_DEBUG = android/app/build/outputs/apk/debug/app-debug.apk
ANDROID_APK_RELEASE = android/app/build/outputs/apk/release/app-release-unsigned.apk
ANDROID_APK = $(if $(filter release,$(ANDROID_BUILD_TYPE)),$(ANDROID_APK_RELEASE),$(ANDROID_APK_DEBUG))
ANDROID_HOME ?= $(HOME)/Library/Android/sdk
ANDROID_NDK_HOME ?= $(shell ls -d $(ANDROID_HOME)/ndk/2* 2>/dev/null | sort -V | tail -1)
ADB ?= $(ANDROID_HOME)/platform-tools/adb
GRADLEW = cd android && ./gradlew

android-setup:
	@echo "=== Running CMake to fetch SDL3 sources (Unix Makefiles, no Ninja) ==="
	cmake -G "Unix Makefiles" \
	      -DCMAKE_SYSTEM_NAME=Android \
	      -DCMAKE_ANDROID_NDK=$(ANDROID_NDK_HOME) \
	      -DANDROID_ABI=arm64-v8a \
	      -DANDROID_PLATFORM=android-21 \
	      -DENGINE_CACHED=ON \
	      -DIMGUI_DEBUGGING=OFF \
	      -DDEBUG_ASAN=OFF \
	      -DSYSTEM_PACKAGES=OFF \
	      -DPRELOAD_ALL_ASSETS=ON \
	      -DSDL_BACKEND=ON \
	      -DSDL_GL=ON \
	      -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
	      -B $(ANDROID_BUILD_DIR) .
	@echo "=== Copying SDL3 Java sources into android project ==="
	@mkdir -p android/app/src/main/java/org/libsdl/app
	@cp -rf $(ANDROID_BUILD_DIR)/_deps/sdl3-src/android-project/app/src/main/java/org/libsdl/app/. \
	        android/app/src/main/java/org/libsdl/app/
	@echo "=== Done! SDL3 Java sources installed. ==="

android-studio: android-setup android-assets
	@echo "=== Project ready for Android Studio ==="
	@echo "=== Open the 'android/' folder in Android Studio ==="
	@echo "=== Android Studio will run CMake/Ninja internally — do NOT run make android-build ==="

android-open: android-studio
	open -a "Android Studio" android/

android-assets: pack
	@mkdir -p android/app/src/main/assets
	@cp etf.sg android/app/src/main/assets/etf.sg
	@echo "=== etf.sg copied to android assets ==="

android-build: android-assets
	@echo "=== Building Android APK ($(ANDROID_BUILD_TYPE)) ==="
	$(GRADLEW) assemble$(shell echo $(ANDROID_BUILD_TYPE) | python3 -c "import sys; print(sys.stdin.read().strip().capitalize())")

android-install:
	@echo "=== Installing APK to device ==="
	$(ADB) install -r $(ANDROID_APK)

android-run:
	@echo "=== Launching $(ANDROID_PACKAGE) ==="
	$(ADB) shell am start -n $(ANDROID_ACTIVITY)

android-stop:
	$(ADB) shell am force-stop $(ANDROID_PACKAGE)

android-logcat:
	$(ADB) logcat -s ETF:* SDL:* SDL/APP:* AndroidRuntime:* GLES:*

android-rebuild: android-build android-install android-run

android-debug:
	@echo "=== Attaching native debugger ==="
	@echo "=== Make sure app is running (make android-run) ==="
	$(ANDROID_NDK_HOME)/prebuilt/darwin-x86_64/bin/ndk-gdb \
	    --project android/app \
	    --launch

android-clean:
	$(GRADLEW) clean
	@rm -rf $(ANDROID_BUILD_DIR)
	@rm -f android/app/src/main/assets/etf.sg

.PHONY: android-setup android-studio android-open android-assets android-build \
        android-install android-run android-stop android-logcat android-rebuild \
        android-debug android-clean

steam:
	@./steamcmd +login enf3rno +quit
	@base64 -i ~/Library/Application\ Support/Steam/config/config.vdf


[12/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/BattleZone.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/BattleZone.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
ninja: build stopped: subcommand failed.

C++ build system [build] failed while executing:
    /Users/kevin/Library/Android/sdk/cmake/3.31.6/bin/ninja \
      -C \
      /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a \
      EscapeTheFate
  from /Users/kevin/git/cpp/EscapeTheFate/android/app
	at org.gradle.internal.UncheckedException.throwAsUncheckedException(UncheckedException.java:69)
	at org.gradle.internal.UncheckedException.throwAsUncheckedException(UncheckedException.java:42)
	at org.gradle.internal.reflect.JavaMethod.invoke(JavaMethod.java:129)
	at org.gradle.api.internal.project.taskfactory.StandardTaskAction.doExecute(StandardTaskAction.java:58)
	at org.gradle.api.internal.project.taskfactory.StandardTaskAction.execute(StandardTaskAction.java:51)
	at org.gradle.api.internal.project.taskfactory.StandardTaskAction.execute(StandardTaskAction.java:29)
	at org.gradle.api.internal.tasks.execution.TaskExecution$3.run(TaskExecution.java:259)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:30)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:27)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:67)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:60)
	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:167)
	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:60)
	at org.gradle.internal.operations.DefaultBuildOperationRunner.run(DefaultBuildOperationRunner.java:48)
	at org.gradle.api.internal.tasks.execution.TaskExecution.executeAction(TaskExecution.java:244)
	at org.gradle.api.internal.tasks.execution.TaskExecution.executeActions(TaskExecution.java:227)
	at org.gradle.api.internal.tasks.execution.TaskExecution.executeWithPreviousOutputFiles(TaskExecution.java:210)
	at org.gradle.api.internal.tasks.execution.TaskExecution.execute(TaskExecution.java:176)
	at org.gradle.internal.execution.steps.ExecuteStep.executeInternal(ExecuteStep.java:167)
	at org.gradle.internal.execution.steps.ExecuteStep.access$000(ExecuteStep.java:47)
	at org.gradle.internal.execution.steps.ExecuteStep$1.call(ExecuteStep.java:137)
	at org.gradle.internal.execution.steps.ExecuteStep$1.call(ExecuteStep.java:134)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$CallableBuildOperationWorker.execute(DefaultBuildOperationRunner.java:210)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$CallableBuildOperationWorker.execute(DefaultBuildOperationRunner.java:205)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:67)
	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:60)
	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:167)
	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:60)
	at org.gradle.internal.operations.DefaultBuildOperationRunner.call(DefaultBuildOperationRunner.java:54)
	at org.gradle.internal.execution.steps.ExecuteStep.execute(ExecuteStep.java:134)
	at org.gradle.internal.execution.steps.ExecuteStep$Mutable.execute(ExecuteStep.java:80)
	at org.gradle.internal.execution.steps.CancelExecutionStep.execute(CancelExecutionStep.java:42)
	at org.gradle.internal.execution.steps.TimeoutStep.executeWithoutTimeout(TimeoutStep.java:75)
	at org.gradle.internal.execution.steps.TimeoutStep.execute(TimeoutStep.java:55)
	at org.gradle.internal.execution.steps.PreCreateOutputParentsStep.execute(PreCreateOutputParentsStep.java:51)
	at org.gradle.internal.execution.steps.PreCreateOutputParentsStep.execute(PreCreateOutputParentsStep.java:29)
	at org.gradle.internal.execution.steps.RemovePreviousOutputsStep.executeMutable(RemovePreviousOutputsStep.java:67)
	at org.gradle.internal.execution.steps.RemovePreviousOutputsStep.executeMutable(RemovePreviousOutputsStep.java:39)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.BroadcastChangingOutputsStep.execute(BroadcastChangingOutputsStep.java:42)
	at org.gradle.internal.execution.steps.BroadcastChangingOutputsStep.execute(BroadcastChangingOutputsStep.java:24)
	at org.gradle.internal.execution.steps.CaptureOutputsAfterExecutionStep.execute(CaptureOutputsAfterExecutionStep.java:69)
	at org.gradle.internal.execution.steps.CaptureOutputsAfterExecutionStep.execute(CaptureOutputsAfterExecutionStep.java:46)
	at org.gradle.internal.execution.steps.ResolveInputChangesStep.executeMutable(ResolveInputChangesStep.java:39)
	at org.gradle.internal.execution.steps.ResolveInputChangesStep.executeMutable(ResolveInputChangesStep.java:28)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.BuildCacheStep.executeWithoutCache(BuildCacheStep.java:189)
	at org.gradle.internal.execution.steps.BuildCacheStep.lambda$execute$1(BuildCacheStep.java:76)
	at org.gradle.internal.Either$Right.fold(Either.java:176)
	at org.gradle.internal.execution.caching.CachingState.fold(CachingState.java:62)
	at org.gradle.internal.execution.steps.BuildCacheStep.execute(BuildCacheStep.java:74)
	at org.gradle.internal.execution.steps.BuildCacheStep.execute(BuildCacheStep.java:49)
	at org.gradle.internal.execution.steps.StoreExecutionStateStep.executeMutable(StoreExecutionStateStep.java:46)
	at org.gradle.internal.execution.steps.StoreExecutionStateStep.executeMutable(StoreExecutionStateStep.java:35)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.SkipUpToDateStep.executeBecause(SkipUpToDateStep.java:75)
	at org.gradle.internal.execution.steps.SkipUpToDateStep.lambda$execute$2(SkipUpToDateStep.java:53)
	at org.gradle.internal.execution.steps.SkipUpToDateStep.execute(SkipUpToDateStep.java:53)
	at org.gradle.internal.execution.steps.SkipUpToDateStep.execute(SkipUpToDateStep.java:35)
	at org.gradle.internal.execution.steps.legacy.MarkSnapshottingInputsFinishedStep.execute(MarkSnapshottingInputsFinishedStep.java:37)
	at org.gradle.internal.execution.steps.legacy.MarkSnapshottingInputsFinishedStep.execute(MarkSnapshottingInputsFinishedStep.java:27)
	at org.gradle.internal.execution.steps.ResolveMutableCachingStateStep.executeDelegate(ResolveMutableCachingStateStep.java:70)
	at org.gradle.internal.execution.steps.ResolveMutableCachingStateStep.executeDelegate(ResolveMutableCachingStateStep.java:32)
	at org.gradle.internal.execution.steps.AbstractResolveCachingStateStep.execute(AbstractResolveCachingStateStep.java:69)
	at org.gradle.internal.execution.steps.AbstractResolveCachingStateStep.execute(AbstractResolveCachingStateStep.java:37)
	at org.gradle.internal.execution.steps.ResolveChangesStep.executeMutable(ResolveChangesStep.java:63)
	at org.gradle.internal.execution.steps.ResolveChangesStep.executeMutable(ResolveChangesStep.java:34)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.ValidateStep$Mutable.executeDelegate(ValidateStep.java:79)
	at org.gradle.internal.execution.steps.ValidateStep$Mutable.executeDelegate(ValidateStep.java:65)
	at org.gradle.internal.execution.steps.ValidateStep.execute(ValidateStep.java:105)
	at org.gradle.internal.execution.steps.ValidateStep$Mutable.execute(ValidateStep.java:65)
	at org.gradle.internal.execution.steps.CaptureMutableStateBeforeExecutionStep.executeMutable(CaptureMutableStateBeforeExecutionStep.java:86)
	at org.gradle.internal.execution.steps.CaptureMutableStateBeforeExecutionStep.execute(CaptureMutableStateBeforeExecutionStep.java:65)
	at org.gradle.internal.execution.steps.CaptureMutableStateBeforeExecutionStep.execute(CaptureMutableStateBeforeExecutionStep.java:45)
	at org.gradle.internal.execution.steps.SkipEmptyMutableWorkStep.executeWithNonEmptySources(SkipEmptyMutableWorkStep.java:210)
	at org.gradle.internal.execution.steps.SkipEmptyMutableWorkStep.executeMutable(SkipEmptyMutableWorkStep.java:85)
	at org.gradle.internal.execution.steps.SkipEmptyMutableWorkStep.executeMutable(SkipEmptyMutableWorkStep.java:53)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.legacy.MarkSnapshottingInputsStartedStep.execute(MarkSnapshottingInputsStartedStep.java:38)
	at org.gradle.internal.execution.steps.LoadPreviousExecutionStateStep.executeMutable(LoadPreviousExecutionStateStep.java:36)
	at org.gradle.internal.execution.steps.LoadPreviousExecutionStateStep.executeMutable(LoadPreviousExecutionStateStep.java:23)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.HandleStaleOutputsStep.executeMutable(HandleStaleOutputsStep.java:77)
	at org.gradle.internal.execution.steps.HandleStaleOutputsStep.executeMutable(HandleStaleOutputsStep.java:43)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.AssignMutableWorkspaceStep.lambda$executeMutable$0(AssignMutableWorkspaceStep.java:34)
	at org.gradle.api.internal.tasks.execution.TaskExecution$4.withWorkspace(TaskExecution.java:305)
	at org.gradle.internal.execution.steps.AssignMutableWorkspaceStep.executeMutable(AssignMutableWorkspaceStep.java:30)
	at org.gradle.internal.execution.steps.AssignMutableWorkspaceStep.executeMutable(AssignMutableWorkspaceStep.java:21)
	at org.gradle.internal.execution.steps.MutableStep.execute(MutableStep.java:26)
	at org.gradle.internal.execution.steps.ChoosePipelineStep.execute(ChoosePipelineStep.java:40)
	at org.gradle.internal.execution.steps.ChoosePipelineStep.execute(ChoosePipelineStep.java:23)
	at org.gradle.internal.execution.steps.ExecuteWorkBuildOperationFiringStep.lambda$execute$2(ExecuteWorkBuildOperationFiringStep.java:67)
	at org.gradle.internal.execution.steps.ExecuteWorkBuildOperationFiringStep.execute(ExecuteWorkBuildOperationFiringStep.java:67)
	at org.gradle.internal.execution.steps.ExecuteWorkBuildOperationFiringStep.execute(ExecuteWorkBuildOperationFiringStep.java:39)
	at org.gradle.internal.execution.steps.IdentityCacheStep.execute(IdentityCacheStep.java:46)
	at org.gradle.internal.execution.steps.IdentityCacheStep.execute(IdentityCacheStep.java:34)
	at org.gradle.internal.execution.steps.IdentifyStep.execute(IdentifyStep.java:56)
	at org.gradle.internal.execution.steps.IdentifyStep.execute(IdentifyStep.java:38)
	at org.gradle.internal.execution.impl.DefaultExecutionEngine$1.execute(DefaultExecutionEngine.java:68)
	at org.gradle.api.internal.tasks.execution.ExecuteActionsTaskExecuter.executeIfValid(ExecuteActionsTaskExecuter.java:132)
	... 30 more
Caused by: com.android.ide.common.process.ProcessException: ninja: Entering directory `/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a'
[0/2] Re-checking globbed directories...
[1/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/main.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/main.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/main.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/main.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/main.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/main.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/main.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/main.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/main.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[2/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameConfig.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameConfig.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameConfig.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[3/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/LocalPlayer.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/LocalPlayer.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[4/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameState.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameState.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameState.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[5/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/Battler.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/Battler.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Battler.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[6/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/EnemyBattler.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/EnemyBattler.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/EnemyBattler.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[7/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/GameObject.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/GameObject.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/GameObject.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[8/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/BattleLocation.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/BattleLocation.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleLocation.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[9/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/PlayerBattler.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/PlayerBattler.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[10/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/Textbox.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/Textbox.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/Textbox.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[11/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/MapExit.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/MapExit.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/MapExit.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
[12/334] Scanning /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/BattleZone.cpp for CXX dependencies
FAILED: CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi 
"CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND" -format=p1689 -- /Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang++ --target=aarch64-none-linux-android21 --sysroot=/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/sysroot -DAL_LIBTYPE_STATIC -DCGLM_STATIC -DEscapeTheFate_EXPORTS -DIS_MOBILE -DPRELOAD_ALL_ASSETS -I/Users/kevin/git/cpp/EscapeTheFate/src -I/Users/kevin/git/cpp/EscapeTheFate/external -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_json-c-build -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_cglm-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/./include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgtools-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgforge-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_ogg-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_openal-src/include/AL -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_vorbis-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-build/include-revision -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/sdl3-src/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-build/include -I/Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_freetype-src/include -isystem /Users/kevin/git/cpp/EscapeTheFate/SupergoonEngine/include -isystem /usr/local/include -isystem /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a/_deps/lib_sgsound-src/include -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -Wformat -Werror=format-security  -std=c++23 -fno-limit-debug-info  -std=gnu++2b -fPIC -Wall -Wextra -Wdeprecated -Wno-unused-parameter -Wno-sign-compare -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -x c++ /Users/kevin/git/cpp/EscapeTheFate/src/gameobject/gameobjects/BattleZone.cpp -c -o CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o -resource-dir "/Users/kevin/Library/Android/sdk/ndk/26.1.10909125/toolchains/llvm/prebuilt/darwin-x86_64/lib/clang/17" -MT CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi -MD -MF CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi.d > CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi.tmp && mv CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi.tmp CMakeFiles/EscapeTheFate.dir/src/gameobject/gameobjects/BattleZone.cpp.o.ddi
/bin/sh: CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS-NOTFOUND: command not found
ninja: build stopped: subcommand failed.

C++ build system [build] failed while executing:
    /Users/kevin/Library/Android/sdk/cmake/3.31.6/bin/ninja \
      -C \
      /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a \
      EscapeTheFate
  from /Users/kevin/git/cpp/EscapeTheFate/android/app
	at com.android.build.gradle.internal.cxx.process.ExecuteProcessKt.execute(ExecuteProcess.kt:288)
	at com.android.build.gradle.internal.cxx.process.ExecuteProcessKt$executeProcess$1.invoke(ExecuteProcess.kt:108)
	at com.android.build.gradle.internal.cxx.process.ExecuteProcessKt$executeProcess$1.invoke(ExecuteProcess.kt:106)
	at com.android.build.gradle.internal.cxx.timing.TimingEnvironmentKt.time(TimingEnvironment.kt:32)
	at com.android.build.gradle.internal.cxx.process.ExecuteProcessKt.executeProcess(ExecuteProcess.kt:106)
	at com.android.build.gradle.internal.cxx.process.ExecuteProcessKt.executeProcess$default(ExecuteProcess.kt:85)
	at com.android.build.gradle.internal.cxx.build.CxxRegularBuilder.executeProcessBatch(CxxRegularBuilder.kt:332)
	at com.android.build.gradle.internal.cxx.build.CxxRegularBuilder.build(CxxRegularBuilder.kt:129)
	at com.android.build.gradle.tasks.ExternalNativeBuildTask$doTaskAction$$inlined$recordTaskAction$1.invoke(BaseTask.kt:70)
	at com.android.build.gradle.internal.tasks.Blocks.recordSpan(Blocks.java:51)
	at com.android.build.gradle.tasks.ExternalNativeBuildTask.doTaskAction(ExternalNativeBuildTask.kt:145)
	at com.android.build.gradle.internal.tasks.UnsafeOutputsTask$taskAction$$inlined$recordTaskAction$1.invoke(BaseTask.kt:65)
	at com.android.build.gradle.internal.tasks.Blocks.recordSpan(Blocks.java:51)
	at com.android.build.gradle.internal.tasks.UnsafeOutputsTask.taskAction(UnsafeOutputsTask.kt:63)
	at org.gradle.internal.reflect.JavaMethod.invoke(JavaMethod.java:125)
	... 129 more
Caused by: com.android.ide.common.process.ProcessException: Error while executing process /Users/kevin/Library/Android/sdk/cmake/3.31.6/bin/ninja with arguments {-C /Users/kevin/git/cpp/EscapeTheFate/android/app/.cxx/Debug/352hv1a5/arm64-v8a EscapeTheFate}
	at com.android.build.gradle.internal.process.GradleProcessResult.buildProcessException(GradleProcessResult.java:73)
	at com.android.build.gradle.internal.process.GradleProcessResult.assertNormalExitValue(GradleProcessResult.java:48)
	at com.android.build.gradle.internal.cxx.process.ExecuteProcessKt.execute(ExecuteProcess.kt:277)
	... 143 more
Caused by: org.gradle.process.ProcessExecutionException: Process 'command '/Users/kevin/Library/Android/sdk/cmake/3.31.6/bin/ninja'' finished with non-zero exit value 1
	at org.gradle.process.internal.DefaultExecHandle$ExecResultImpl.assertNormalExitValue(DefaultExecHandle.java:460)
	at com.android.build.gradle.internal.process.GradleProcessResult.assertNormalExitValue(GradleProcessResult.java:46)
	... 144 more


Deprecated Gradle features were used in this build, making it incompatible with Gradle 10.

You can use '--warning-mode all' to show the individual deprecation warnings and determine if they come from your own scripts or plugins.

For more on this, please refer to https://docs.gradle.org/9.7.1/userguide/command_line_interface.html#sec:command_line_warnings in the Gradle documentation.

BUILD FAILED in 488ms
32 actionable tasks: 2 executed, 30 up-to-date
