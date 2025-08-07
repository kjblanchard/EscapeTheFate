.PHONY: all configure build clean debug release aseprite
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
DEFAULT_IMGUI ?= ON
SYSTEM_PACKAGES ?= ON
ENGINE_CACHED ?= ON
BUILD_COMMAND ?= cmake --build $(BUILD_DIR) --config $(BUILD_TYPE)
UNIX_PACKAGE_COMMAND ?= tar --exclude='*.aseprite' -czvf $(BUILD_DIR)/$(EXECUTABLE_NAME).tgz -C $(BINARY_FOLDER_REL_PATH) .
WINDOWS_PACKAGE_COMMAND ?= "7z a -r $(BUILD_DIR)/$(EXECUTABLE_NAME).zip $(BINARY_FOLDER_REL_PATH)"
PACKAGE_COMMAND ?= $(UNIX_PACKAGE_COMMAND)
# TODO Needed on the build step for ios, so that it can allow provisioning updates, need to put this in correctly
ADDITIONAL_OPTIONS ?=
ADDITIONAL_BUILD_COMMANDS ?=
# IOS_BUILD_COMMANDS ?= -- -allowProvisioningUpdates
IOS_BUILD_COMMANDS = "-- -allowProvisioningUpdates"
# Tiled Configuration
TILED_PATH = /Applications/Tiled.app/Contents/MacOS/Tiled
TILED_FOLDER_PATH = ./assets/tiled
TILED_EXPORT_TILESETS = background terrain house inside
TILED_EXPORT_MAPS = debugTown debugSouth cloud debugTownHome forest1
# Aseprite
ASEPRITE_DIR = assets/aseprite
JSON_TO_LUA_SCRIPT = tools/jsontolua.py
# default, should be used after a rebuild of some sort.
all: build install run
clean:
	@rm -rf $(BUILD_DIR)
configure:
	$(CONFIGURE_COMMAND) -G "$(CMAKE_GENERATOR)" . -B $(BUILD_DIR) -Dimgui=$(DEFAULT_IMGUI) -DENGINE_CACHED=$(ENGINE_CACHED) -DSYSTEM_PACKAGES=$(SYSTEM_PACKAGES) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(ADDITIONAL_OPTIONS)
build:
	@$(BUILD_COMMAND) $(ADDITIONAL_BUILD_COMMANDS)
install:
	@cmake --install $(BUILD_DIR) --config $(BUILD_TYPE)
run:
	@open ./build/bin/$(EXECUTABLE_NAME).app || ./build/bin/$(EXECUTABLE_NAME)
debug: build
	@lldb -s breakpoints.lldb ./build/bin/$(EXECUTABLE_NAME).app/Contents/MacOS/$(EXECUTABLE_NAME)

package:
	$(PACKAGE_COMMAND)

# Custom build commands that set variables accordingly based on platform.. rebuild is macos, brebuild is backup, wrebuild is windows, erebuild is emscripten, irebuild is ios simulator
rebuild:
	@$(MAKE) CMAKE_GENERATOR=$(DEFAULT_GENERATOR) clean configure build install
xrebuild:
	@$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF clean configure build install package
brebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) SYSTEM_PACKAGES=OFF clean configure build install package
wrebuild:
	$(MAKE) CMAKE_GENERATOR=$(WINDOWS_GENERATOR) PACKAGE_COMMAND=$(WINDOWS_PACKAGE_COMMAND) SYSTEM_PACKAGES=OFF configure build install package
erebuild:
	@$(MAKE) CMAKE_GENERATOR=$(BACKUP_GENERATOR) CONFIGURE_COMMAND=$(EMSCRIPTEN_CONFIGURE_COMMAND) BUILD_COMMAND='sudo $(BUILD_COMMAND)' SYSTEM_PACKAGES=OFF DEFAULT_IMGUI=OFF clean configure build
# Haven't tested this locally with systempackages off, added this after removing engine.
irebuild:
	$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF DEFAULT_IMGUI=OFF ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE" clean configure build install package
# iosrebuild:
# 	$(MAKE) CMAKE_GENERATOR=$(APPLE_GENERATOR) SYSTEM_PACKAGES=OFF DEFAULT_IMGUI=OFF ADDITIONAL_BUILD_COMMANDS=$(IOS_BUILD_COMMANDS) ADDITIONAL_OPTIONS="-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 -DTARGET_OS_IOS=TRUE" clean configure build install package
iosrebuild:
	$(MAKE) \
		CMAKE_GENERATOR=$(APPLE_GENERATOR) \
		SYSTEM_PACKAGES=OFF \
		DEFAULT_IMGUI=OFF \
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
#Helpers
buildtime:
	./tools/quick_build_times.py -C build
trace:
	./tools/ninja_trace.py build/.ninja_log > trace.json
# Upload trace to about:trace in chrome, or https://ui.perfetto.dev/
bloaty:
	dsymutil ./$(EXECUTABLE_NAME) -o SupergoonClient.dSYM
	bloaty -d compileunits SupergoonClient --debug-file SupergoonClient.dSYM/Contents/Resources/DWARF/SupergoonClient
valgrind:
	valgrind --track-origins=yes --leak-check=yes --leak-resolution=low --show-leak-kinds=definite ./build/bin/$(EXECUTABLE_NAME) 2>&1 | tee memcheck.txt
# Exports the tilesets if we need to as lua files for tsx/tmx
tiled:
	@$(foreach file,$(TILED_EXPORT_MAPS),\
		$(TILED_PATH) --export-map --detach-templates --embed-tilesets --resolve-types-and-properties lua $(TILED_FOLDER_PATH)/$(file).tmj $(TILED_FOLDER_PATH)/$(file).lua; \
	)
aseprite:
	@echo "Converting Aseprite JSON files to Lua..."
	@python3 $(JSON_TO_LUA_SCRIPT) --dir $(ASEPRITE_DIR)
	@echo "Conversion complete."
teamid:
	@security find-certificate -c "Apple Development" -p | openssl x509 -inform pem -noout -subject

