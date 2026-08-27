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
# SETUP (one-time, on the build machine):
#
#   brew install --cask android-commandlinetools
#   brew install --cask temurin@17
#
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "platform-tools"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "platforms;android-34"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "build-tools;34.0.0"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "ndk;27.0.12077973"
#   sdkmanager --sdk_root=$HOME/Library/Android/sdk "cmake;3.31.6"
#
#   echo 'export ANDROID_HOME=$HOME/Library/Android/sdk' >> ~/.zshrc
#   source ~/.zshrc
#
# WORKFLOW:
#   make android-build         # Build debug APK (packs assets automatically)
#   make android-install       # Push APK to device
#   make android-run           # Launch app
#   make android-logcat        # View logs
#   make android-open          # Open in Android Studio for interactive dev
# =========================================================

ANDROID_BUILD_TYPE ?= Debug
ANDROID_PACKAGE = com.supergoon.rpg
ANDROID_ACTIVITY = $(ANDROID_PACKAGE)/.EscapeTheFateActivity
ANDROID_HOME ?= $(HOME)/Library/Android/sdk
ANDROID_NDK_VERSION ?= 27.0.12077973
ANDROID_NDK = $(ANDROID_HOME)/ndk/$(ANDROID_NDK_VERSION)
ANDROID_CMAKE = $(ANDROID_HOME)/cmake/3.31.6/bin/cmake
ANDROID_BUILD_DIR = build-android
ANDROID_ABI ?= arm64-v8a
ADB ?= $(ANDROID_HOME)/platform-tools/adb

android-configure: pack
	@mkdir -p android/app/src/main/assets
	@cp etf.sg android/app/src/main/assets/etf.sg
	$(ANDROID_CMAKE) -B $(ANDROID_BUILD_DIR) \
		-DCMAKE_TOOLCHAIN_FILE=$(ANDROID_NDK)/build/cmake/android.toolchain.cmake \
		-DANDROID_ABI=$(ANDROID_ABI) \
		-DANDROID_PLATFORM=android-21 \
		-DCMAKE_BUILD_TYPE=$(ANDROID_BUILD_TYPE) \
		-DENGINE_CACHED=ON \
		-DIMGUI_DEBUGGING=OFF \
		-DDEBUG_ASAN=OFF \
		-DSYSTEM_PACKAGES=OFF \
		-DPRELOAD_ALL_ASSETS=ON \
		-DSDL_BACKEND=ON \
		-DSDL_GL=ON \
		-DSDL_RENDERER=OFF \
		-DSTEAM_ENABLED=OFF \
		-DLINK_M=OFF \
		-DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
		-DCMAKE_CXX_SCAN_FOR_MODULES=OFF

android-build: android-configure
	$(ANDROID_CMAKE) --build $(ANDROID_BUILD_DIR) -j$$(nproc 2>/dev/null || sysctl -n hw.ncpu)

android-install:
	$(ADB) install -r $(ANDROID_BUILD_DIR)/*.apk

android-run:
	$(ADB) shell am start -n $(ANDROID_ACTIVITY)

android-stop:
	$(ADB) shell am force-stop $(ANDROID_PACKAGE)

android-logcat:
	$(ADB) logcat -s ETF:* SDL:* SDL/APP:* AndroidRuntime:* GLES:*

android-rebuild: android-clean android-build android-install android-run

android-open:
	open -a "Android Studio" android/

android-clean:
	@rm -rf $(ANDROID_BUILD_DIR)
	@rm -f android/app/src/main/assets/etf.sg

.PHONY: android-configure android-build android-install android-run android-stop \
        android-logcat android-rebuild android-open android-clean

steam:
	@./steamcmd +login enf3rno +quit
	@base64 -i ~/Library/Application\ Support/Steam/config/config.vdf
