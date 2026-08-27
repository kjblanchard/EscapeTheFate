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
irebuild:
	$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE -DDISABLE_WERROR=YES" clean configure build package
iosrebuild:
	$(MAKE) \
		CMAKE_GENERATOR=$(APPLE_GENERATOR) \
		ADDITIONAL_BUILD_COMMANDS=$(IOS_BUILD_COMMANDS) \
		ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE" \
		IMGUI_DEBUGGING=OFF \
		clean configure build package
	# Custom run commands
erun:
	@emrun --no_browser --port 6931 ./build/EscapeTheFate.html

# =========================================================
# iOS Deploy & Debug
# =========================================================
# Tools needed:
#   - Xcode (includes xcrun, simctl)
#   - ios-deploy for physical device: brew install ios-deploy
#   - For wireless debug: pair device in Xcode first (Window -> Devices)
#
# Simulator workflow:
#   make irebuild && make irun
# Device workflow:
#   make iosrebuild && make idev-run
# =========================================================

IOS_BUNDLE_ID = com.supergoon.rpg
ISIM_APP_PATH = build/Debug/EscapeTheFate.app
IOS_DEVICE_APP_PATH = build/Debug/EscapeTheFate.app
ISIM_UDID ?= $(shell xcrun simctl list devices booted -j 2>/dev/null | python3 -c "import json,sys;d=json.load(sys.stdin);devs=[v for vs in d['devices'].values() for v in vs if v['state']=='Booted'];print(devs[0]['udid'] if devs else '')" 2>/dev/null)

isim-list:
	xcrun simctl list devices available

idevices:
	xcrun xctrace list devices 2>&1 | grep -v Simulator

irun:
	@if [ -z "$(ISIM_UDID)" ]; then echo "No booted simulator found. Boot one or set ISIM_UDID=<uuid>"; exit 1; fi
	xcrun simctl install $(ISIM_UDID) $(ISIM_APP_PATH)
	xcrun simctl launch $(ISIM_UDID) $(IOS_BUNDLE_ID)

idebug:
	@if [ -z "$(ISIM_UDID)" ]; then echo "Set ISIM_UDID=<uuid> from 'make isim-list'"; exit 1; fi
	xcrun simctl install $(ISIM_UDID) $(ISIM_APP_PATH)
	xcrun simctl launch --wait-for-debugger $(ISIM_UDID) $(IOS_BUNDLE_ID) &
	@sleep 1
	lldb -o "platform select ios-simulator" -o "platform connect $(ISIM_UDID)" -o "process attach --name EscapeTheFate --waitfor"

idev-install:
	ios-deploy --bundle $(IOS_DEVICE_APP_PATH) --no-wifi

idev-run:
	ios-deploy --bundle $(IOS_DEVICE_APP_PATH) --no-wifi --justlaunch

idev-debug:
	ios-deploy --bundle $(IOS_DEVICE_APP_PATH) --no-wifi --debug

idev-wireless-debug:
	@if [ -z "$(DEVICE_ID)" ]; then echo "Set DEVICE_ID=<udid> from 'xcrun xctrace list devices'"; exit 1; fi
	ios-deploy --bundle $(IOS_DEVICE_APP_PATH) --id $(DEVICE_ID) --debug

.PHONY: isim-list idevices irun idebug idev-install idev-run idev-debug idev-wireless-debug

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
	@echo "=== Running CMake to fetch SDL3 sources ==="
	cmake -DCMAKE_SYSTEM_NAME=Android \
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
	@echo "=== Generate gradle wrapper: cd android && gradle wrapper ==="

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

.PHONY: android-setup android-assets android-build android-install android-run \
        android-stop android-logcat android-rebuild android-debug android-clean

steam:
	@./steamcmd +login enf3rno +quit
	@base64 -i ~/Library/Application\ Support/Steam/config/config.vdf



