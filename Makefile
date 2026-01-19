.PHONY: all configure build clean debug release
BUILD_DIR = build
BINARY_FOLDER = bin
EXECUTABLE_NAME = EscapeTheFate
BINARY_FOLDER_REL_PATH = $(BUILD_DIR)/$(BINARY_FOLDER)
DEFAULT_GENERATOR ?= "Ninja"
BACKUP_GENERATOR ?= "Unix Makefiles"
WINDOWS_GENERATOR ?= "Visual Studio 17 2022"
APPLE_GENERATOR ?= Xcode
CONFIGURE_COMMAND ?= "cmake"
EMSCRIPTEN_CONFIGURE_COMMAND = "emcmake cmake"
BUILD_TYPE ?= Debug
SYSTEM_PACKAGES ?= OFF
ENGINE_CACHED ?= ON
BUILD_COMMAND ?= cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)
UNIX_PACKAGE_COMMAND ?= tar --exclude='*.aseprite' -czvf $(BUILD_DIR)/$(EXECUTABLE_NAME).tgz -C $(BINARY_FOLDER_REL_PATH) .
WINDOWS_PACKAGE_COMMAND ?= "7z a -r $(BUILD_DIR)/$(EXECUTABLE_NAME).zip $(BINARY_FOLDER_REL_PATH)"
PACKAGE_COMMAND ?= $(UNIX_PACKAGE_COMMAND)
# TODO Needed on the build step for ios, so that it can allow provisioning updates, need to put this in correctly
ADDITIONAL_OPTIONS ?=
ADDITIONAL_BUILD_COMMANDS ?=
IOS_BUILD_COMMANDS = "-- -allowProvisioningUpdates"
# default, should be used after a rebuild of some sort.
UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Darwin)
REBUILD := mrebuild
RUN_CMD := open ./build/bin/EscapeTheFate.app
else ifeq ($(UNAME_S),Linux)
REBUILD := lrebuild
RUN_CMD := ./build/bin/$(EXECUTABLE_NAME)
else
REBUILD := lrebuild
endif

.PHONY: all
all:
    @echo "OS: $(UNAME_S) -> using $(REBUILD)"


# -DCMAKE_POLICY_VERSION_MINIMUM=3.5 use this if we are using past version 4.0
#
all: build install run
clean:
	@rm -rf $(BUILD_DIR)
configure:
	$(CONFIGURE_COMMAND) -DCMAKE_POLICY_VERSION_MINIMUM=3.5  -G "$(CMAKE_GENERATOR)" . -B $(BUILD_DIR) -DENGINE_CACHED=$(ENGINE_CACHED) -DSYSTEM_PACKAGES=$(SYSTEM_PACKAGES) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(ADDITIONAL_OPTIONS)  -DLINK_M=$(LINK_M)
build:
	@$(BUILD_COMMAND) $(ADDITIONAL_BUILD_COMMANDS)
install:
	@cmake --install $(BUILD_DIR) --config $(BUILD_TYPE)
run:
	@$(RUN_CMD)

debug: build install
	@gdb  $(RUN_CMD)

package:
	$(PACKAGE_COMMAND)

# Custom build commands that set variables accordingly based on platform.. rebuild is macos, brebuild is backup, wrebuild is windows, erebuild is emscripten, irebuild is ios simulator
rebuild:
	@$(MAKE) $(REBUILD) 
mrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) clean configure build install
lrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) LINK_M=ON clean configure build install 
xrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF ADDITIONAL_OPTIONS="-DDISABLE_WERROR=YES" clean configure build install package
brebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) SYSTEM_PACKAGES=OFF clean configure build install package
wrebuild:
	$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) PACKAGE_COMMAND=$(WINDOWS_PACKAGE_COMMAND) SYSTEM_PACKAGES=OFF configure build install package
erebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) CONFIGURE_COMMAND=$(EMSCRIPTEN_CONFIGURE_COMMAND) SYSTEM_PACKAGES=OFF clean configure build
irebuild:
	$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF  ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE -DDISABLE_WERROR=YES" clean configure build install package
iosrebuild:
	$(MAKE) \
		CMAKE_GENERATOR=$(APPLE_GENERATOR) \
		SYSTEM_PACKAGES=OFF \
		ADDITIONAL_BUILD_COMMANDS=$(IOS_BUILD_COMMANDS) \
		ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE" \
		clean configure build install package
	# Custom run commands
erun:
	emrun ./build/bin/$(EXECUTABLE_NAME).html
irun:
	xcrun simctl install 8E52A7E9-F047-4888-962D-78E252321592 build/bin/Debug/EscapeTheFate.app
idevices:
	xcrun simctl list devices
# Used when you want to run instruments when not using xcode to build (local dev)
codesign:
	@codesign --force --deep --sign - --entitlements cmake/EscapeTheFate.entitlements build/bin/EscapeTheFate.app

teamid:
	@security find-certificate -c "Apple Development" -p | openssl x509 -inform pem -noout -subject

