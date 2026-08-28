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
		-DCMAKE_CXX_SCAN_FOR_MODULES=OFF \
		-DCMAKE_C_STANDARD_LIBRARIES="-lm" \
		-DCMAKE_CXX_STANDARD_LIBRARIES="-lm"

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

# Could not determine the dependencies of task ':app:buildCMakeDebug[arm64-v8a][EscapeTheFate]'.
# > Could not create task ':app:configureCMakeDebug[arm64-v8a]'.
#    > DefaultTaskContainer#NamedDomainObjectProvider.configure(Action) on task set cannot be executed in the current context.

# * Try:
# > Run with --info or --debug option to get more log output.
# > Run with --scan to get full insights.
# > Get more help at https://help.gradle.org.

# * Exception is:
# org.gradle.api.internal.tasks.TaskDependencyResolveException: Could not determine the dependencies of task ':app:buildCMakeDebug[arm64-v8a][EscapeTheFate]'.
# 	at org.gradle.api.internal.tasks.CachingTaskDependencyResolveContext.getDependencies(CachingTaskDependencyResolveContext.java:68)
# 	at org.gradle.execution.plan.TaskDependencyResolver.resolveDependenciesFor(TaskDependencyResolver.java:49)
# 	at org.gradle.execution.plan.LocalTaskNode.getDependencies(LocalTaskNode.java:148)
# 	at org.gradle.execution.plan.LocalTaskNode.resolveDependencies(LocalTaskNode.java:122)
# 	at org.gradle.execution.plan.DefaultExecutionPlan.discoverNodeRelationships(DefaultExecutionPlan.java:182)
# 	at org.gradle.execution.plan.DefaultExecutionPlan.doAddEntryNodes(DefaultExecutionPlan.java:154)
# 	at org.gradle.execution.plan.DefaultExecutionPlan.addEntryTasks(DefaultExecutionPlan.java:129)
# 	at org.gradle.execution.plan.DefaultExecutionPlan.addEntryTasks(DefaultExecutionPlan.java:121)
# 	at org.gradle.execution.TaskNameResolvingBuildTaskScheduler.scheduleRequestedTasks(TaskNameResolvingBuildTaskScheduler.java:54)
# 	at org.gradle.execution.DefaultTasksBuildTaskScheduler.scheduleRequestedTasks(DefaultTasksBuildTaskScheduler.java:72)
# 	at org.gradle.initialization.DefaultTaskExecutionPreparer.lambda$scheduleRequestedTasks$0(DefaultTaskExecutionPreparer.java:46)
# 	at org.gradle.internal.Factories$1.create(Factories.java:31)
# 	at org.gradle.internal.work.DefaultWorkerLeaseService.withReplacedLocks(DefaultWorkerLeaseService.java:360)
# 	at org.gradle.api.internal.project.DefaultProjectStateRegistry$DefaultBuildProjectRegistry.withMutableStateOfAllProjects(DefaultProjectStateRegistry.java:240)
# 	at org.gradle.api.internal.project.DefaultProjectStateRegistry$DefaultBuildProjectRegistry.withMutableStateOfAllProjects(DefaultProjectStateRegistry.java:233)
# 	at org.gradle.initialization.DefaultTaskExecutionPreparer.scheduleRequestedTasks(DefaultTaskExecutionPreparer.java:45)
# 	at org.gradle.initialization.VintageBuildModelController.lambda$scheduleRequestedTasks$0(VintageBuildModelController.java:76)
# 	at org.gradle.internal.model.StateTransitionController.lambda$inState$1(StateTransitionController.java:99)
# 	at org.gradle.internal.model.StateTransitionController.lambda$inState$2(StateTransitionController.java:114)
# 	at org.gradle.internal.work.DefaultSynchronizer.withLock(DefaultSynchronizer.java:44)
# 	at org.gradle.internal.model.StateTransitionController.inState(StateTransitionController.java:110)
# 	at org.gradle.internal.model.StateTransitionController.inState(StateTransitionController.java:98)
# 	at org.gradle.initialization.VintageBuildModelController.scheduleRequestedTasks(VintageBuildModelController.java:76)
# 	at org.gradle.internal.build.DefaultBuildLifecycleController$DefaultWorkGraphBuilder.addRequestedTasks(DefaultBuildLifecycleController.java:404)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeWorkPreparer.lambda$scheduleRequestedTasks$0(DefaultBuildTreeWorkPreparer.java:41)
# 	at org.gradle.internal.build.DefaultBuildLifecycleController.lambda$populateWorkGraph$7(DefaultBuildLifecycleController.java:189)
# 	at org.gradle.internal.build.DefaultBuildWorkPreparer.populateWorkGraph(DefaultBuildWorkPreparer.java:42)
# 	at org.gradle.internal.build.BuildOperationFiringBuildWorkPreparer$PopulateWorkGraph.populateTaskGraph(BuildOperationFiringBuildWorkPreparer.java:106)
# 	at org.gradle.internal.build.BuildOperationFiringBuildWorkPreparer$PopulateWorkGraph.run(BuildOperationFiringBuildWorkPreparer.java:92)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:29)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:26)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:66)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:157)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.run(DefaultBuildOperationRunner.java:47)
# 	at org.gradle.internal.operations.DefaultBuildOperationExecutor.run(DefaultBuildOperationExecutor.java:68)
# 	at org.gradle.internal.build.BuildOperationFiringBuildWorkPreparer.populateWorkGraph(BuildOperationFiringBuildWorkPreparer.java:67)
# 	at org.gradle.internal.build.DefaultBuildLifecycleController.lambda$populateWorkGraph$8(DefaultBuildLifecycleController.java:189)
# 	at org.gradle.internal.model.StateTransitionController.lambda$inState$1(StateTransitionController.java:99)
# 	at org.gradle.internal.model.StateTransitionController.lambda$inState$2(StateTransitionController.java:114)
# 	at org.gradle.internal.work.DefaultSynchronizer.withLock(DefaultSynchronizer.java:44)
# 	at org.gradle.internal.model.StateTransitionController.inState(StateTransitionController.java:110)
# 	at org.gradle.internal.model.StateTransitionController.inState(StateTransitionController.java:98)
# 	at org.gradle.internal.build.DefaultBuildLifecycleController.populateWorkGraph(DefaultBuildLifecycleController.java:189)
# 	at org.gradle.internal.build.DefaultBuildWorkGraphController$DefaultBuildWorkGraph.populateWorkGraph(DefaultBuildWorkGraphController.java:169)
# 	at org.gradle.composite.internal.DefaultBuildController.populateWorkGraph(DefaultBuildController.java:76)
# 	at org.gradle.composite.internal.DefaultIncludedBuildTaskGraph$DefaultBuildTreeWorkGraphBuilder.withWorkGraph(DefaultIncludedBuildTaskGraph.java:153)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeWorkPreparer.lambda$scheduleRequestedTasks$1(DefaultBuildTreeWorkPreparer.java:41)
# 	at org.gradle.composite.internal.DefaultIncludedBuildTaskGraph$DefaultBuildTreeWorkGraph$1.run(DefaultIncludedBuildTaskGraph.java:209)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:29)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:26)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:66)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:157)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.run(DefaultBuildOperationRunner.java:47)
# 	at org.gradle.internal.operations.DefaultBuildOperationExecutor.run(DefaultBuildOperationExecutor.java:68)
# 	at org.gradle.composite.internal.DefaultIncludedBuildTaskGraph$DefaultBuildTreeWorkGraph.scheduleWork(DefaultIncludedBuildTaskGraph.java:204)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeWorkPreparer.scheduleRequestedTasks(DefaultBuildTreeWorkPreparer.java:37)
# 	at org.gradle.configurationcache.VintageBuildTreeWorkController$scheduleAndRunRequestedTasks$1.apply(VintageBuildTreeWorkController.kt:36)
# 	at org.gradle.configurationcache.VintageBuildTreeWorkController$scheduleAndRunRequestedTasks$1.apply(VintageBuildTreeWorkController.kt:35)
# 	at org.gradle.composite.internal.DefaultIncludedBuildTaskGraph.withNewWorkGraph(DefaultIncludedBuildTaskGraph.java:112)
# 	at org.gradle.configurationcache.VintageBuildTreeWorkController.scheduleAndRunRequestedTasks(VintageBuildTreeWorkController.kt:35)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeLifecycleController.lambda$scheduleAndRunTasks$1(DefaultBuildTreeLifecycleController.java:77)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeLifecycleController.lambda$runBuild$4(DefaultBuildTreeLifecycleController.java:120)
# 	at org.gradle.internal.model.StateTransitionController.lambda$transition$6(StateTransitionController.java:169)
# 	at org.gradle.internal.model.StateTransitionController.doTransition(StateTransitionController.java:266)
# 	at org.gradle.internal.model.StateTransitionController.lambda$transition$7(StateTransitionController.java:169)
# 	at org.gradle.internal.work.DefaultSynchronizer.withLock(DefaultSynchronizer.java:44)
# 	at org.gradle.internal.model.StateTransitionController.transition(StateTransitionController.java:169)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeLifecycleController.runBuild(DefaultBuildTreeLifecycleController.java:117)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeLifecycleController.scheduleAndRunTasks(DefaultBuildTreeLifecycleController.java:77)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeLifecycleController.scheduleAndRunTasks(DefaultBuildTreeLifecycleController.java:72)
# 	at org.gradle.tooling.internal.provider.runner.BuildModelActionRunner.run(BuildModelActionRunner.java:53)
# 	at org.gradle.launcher.exec.ChainingBuildActionRunner.run(ChainingBuildActionRunner.java:35)
# 	at org.gradle.internal.buildtree.ProblemReportingBuildActionRunner.run(ProblemReportingBuildActionRunner.java:49)
# 	at org.gradle.launcher.exec.BuildOutcomeReportingBuildActionRunner.run(BuildOutcomeReportingBuildActionRunner.java:65)
# 	at org.gradle.tooling.internal.provider.FileSystemWatchingBuildActionRunner.run(FileSystemWatchingBuildActionRunner.java:140)
# 	at org.gradle.launcher.exec.BuildCompletionNotifyingBuildActionRunner.run(BuildCompletionNotifyingBuildActionRunner.java:41)
# 	at org.gradle.launcher.exec.RootBuildLifecycleBuildActionExecutor.lambda$execute$0(RootBuildLifecycleBuildActionExecutor.java:40)
# 	at org.gradle.composite.internal.DefaultRootBuildState.run(DefaultRootBuildState.java:123)
# 	at org.gradle.launcher.exec.RootBuildLifecycleBuildActionExecutor.execute(RootBuildLifecycleBuildActionExecutor.java:40)
# 	at org.gradle.internal.buildtree.InitDeprecationLoggingActionExecutor.execute(InitDeprecationLoggingActionExecutor.java:66)
# 	at org.gradle.internal.buildtree.InitProblems.execute(InitProblems.java:36)
# 	at org.gradle.internal.buildtree.DefaultBuildTreeContext.execute(DefaultBuildTreeContext.java:40)
# 	at org.gradle.launcher.exec.BuildTreeLifecycleBuildActionExecutor.lambda$execute$0(BuildTreeLifecycleBuildActionExecutor.java:71)
# 	at org.gradle.internal.buildtree.BuildTreeState.run(BuildTreeState.java:60)
# 	at org.gradle.launcher.exec.BuildTreeLifecycleBuildActionExecutor.execute(BuildTreeLifecycleBuildActionExecutor.java:71)
# 	at org.gradle.launcher.exec.RunAsBuildOperationBuildActionExecutor$3.call(RunAsBuildOperationBuildActionExecutor.java:61)
# 	at org.gradle.launcher.exec.RunAsBuildOperationBuildActionExecutor$3.call(RunAsBuildOperationBuildActionExecutor.java:57)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$CallableBuildOperationWorker.execute(DefaultBuildOperationRunner.java:200)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$CallableBuildOperationWorker.execute(DefaultBuildOperationRunner.java:195)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:66)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:157)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.call(DefaultBuildOperationRunner.java:53)
# 	at org.gradle.internal.operations.DefaultBuildOperationExecutor.call(DefaultBuildOperationExecutor.java:73)
# 	at org.gradle.launcher.exec.RunAsBuildOperationBuildActionExecutor.execute(RunAsBuildOperationBuildActionExecutor.java:57)
# 	at org.gradle.launcher.exec.RunAsWorkerThreadBuildActionExecutor.lambda$execute$0(RunAsWorkerThreadBuildActionExecutor.java:36)
# 	at org.gradle.internal.work.DefaultWorkerLeaseService.withLocks(DefaultWorkerLeaseService.java:264)
# 	at org.gradle.internal.work.DefaultWorkerLeaseService.runAsWorkerThread(DefaultWorkerLeaseService.java:128)
# 	at org.gradle.launcher.exec.RunAsWorkerThreadBuildActionExecutor.execute(RunAsWorkerThreadBuildActionExecutor.java:36)
# 	at org.gradle.tooling.internal.provider.continuous.ContinuousBuildActionExecutor.execute(ContinuousBuildActionExecutor.java:110)
# 	at org.gradle.tooling.internal.provider.SubscribableBuildActionExecutor.execute(SubscribableBuildActionExecutor.java:64)
# 	at org.gradle.internal.session.DefaultBuildSessionContext.execute(DefaultBuildSessionContext.java:46)
# 	at org.gradle.tooling.internal.provider.BuildSessionLifecycleBuildActionExecuter$ActionImpl.apply(BuildSessionLifecycleBuildActionExecuter.java:92)
# 	at org.gradle.tooling.internal.provider.BuildSessionLifecycleBuildActionExecuter$ActionImpl.apply(BuildSessionLifecycleBuildActionExecuter.java:80)
# 	at org.gradle.internal.session.BuildSessionState.run(BuildSessionState.java:71)
# 	at org.gradle.tooling.internal.provider.BuildSessionLifecycleBuildActionExecuter.execute(BuildSessionLifecycleBuildActionExecuter.java:62)
# 	at org.gradle.tooling.internal.provider.BuildSessionLifecycleBuildActionExecuter.execute(BuildSessionLifecycleBuildActionExecuter.java:41)
# 	at org.gradle.tooling.internal.provider.StartParamsValidatingActionExecuter.execute(StartParamsValidatingActionExecuter.java:64)
# 	at org.gradle.tooling.internal.provider.StartParamsValidatingActionExecuter.execute(StartParamsValidatingActionExecuter.java:32)
# 	at org.gradle.tooling.internal.provider.SessionFailureReportingActionExecuter.execute(SessionFailureReportingActionExecuter.java:51)
# 	at org.gradle.tooling.internal.provider.SessionFailureReportingActionExecuter.execute(SessionFailureReportingActionExecuter.java:39)
# 	at org.gradle.tooling.internal.provider.SetupLoggingActionExecuter.execute(SetupLoggingActionExecuter.java:47)
# 	at org.gradle.tooling.internal.provider.SetupLoggingActionExecuter.execute(SetupLoggingActionExecuter.java:31)
# 	at org.gradle.launcher.daemon.server.exec.ExecuteBuild.doBuild(ExecuteBuild.java:65)
# 	at org.gradle.launcher.daemon.server.exec.BuildCommandOnly.execute(BuildCommandOnly.java:37)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.WatchForDisconnection.execute(WatchForDisconnection.java:39)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.ResetDeprecationLogger.execute(ResetDeprecationLogger.java:29)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.RequestStopIfSingleUsedDaemon.execute(RequestStopIfSingleUsedDaemon.java:35)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.ForwardClientInput$2.create(ForwardClientInput.java:78)
# 	at org.gradle.launcher.daemon.server.exec.ForwardClientInput$2.create(ForwardClientInput.java:75)
# 	at org.gradle.util.internal.Swapper.swap(Swapper.java:38)
# 	at org.gradle.launcher.daemon.server.exec.ForwardClientInput.execute(ForwardClientInput.java:75)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.LogAndCheckHealth.execute(LogAndCheckHealth.java:64)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.LogToClient.doBuild(LogToClient.java:63)
# 	at org.gradle.launcher.daemon.server.exec.BuildCommandOnly.execute(BuildCommandOnly.java:37)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.EstablishBuildEnvironment.doBuild(EstablishBuildEnvironment.java:84)
# 	at org.gradle.launcher.daemon.server.exec.BuildCommandOnly.execute(BuildCommandOnly.java:37)
# 	at org.gradle.launcher.daemon.server.api.DaemonCommandExecution.proceed(DaemonCommandExecution.java:104)
# 	at org.gradle.launcher.daemon.server.exec.StartBuildOrRespondWithBusy$1.run(StartBuildOrRespondWithBusy.java:52)
# 	at org.gradle.launcher.daemon.server.DaemonStateCoordinator$1.run(DaemonStateCoordinator.java:297)
# 	at org.gradle.internal.concurrent.ExecutorPolicy$CatchAndRecordFailures.onExecute(ExecutorPolicy.java:64)
# 	at org.gradle.internal.concurrent.AbstractManagedExecutor$1.run(AbstractManagedExecutor.java:47)
# Caused by: org.gradle.api.internal.tasks.DefaultTaskContainer$TaskCreationException: Could not create task ':app:configureCMakeDebug[arm64-v8a]'.
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer.taskCreationException(DefaultTaskContainer.java:717)
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer.access$600(DefaultTaskContainer.java:78)
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer$TaskCreatingProvider.domainObjectCreationException(DefaultTaskContainer.java:709)
# 	at org.gradle.api.internal.DefaultNamedDomainObjectCollection$AbstractDomainObjectCreatingProvider.tryCreate(DefaultNamedDomainObjectCollection.java:1017)
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer$TaskCreatingProvider.access$1401(DefaultTaskContainer.java:656)
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer$TaskCreatingProvider$1.run(DefaultTaskContainer.java:682)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:29)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:26)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:66)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:157)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.run(DefaultBuildOperationRunner.java:47)
# 	at org.gradle.internal.operations.DefaultBuildOperationExecutor.run(DefaultBuildOperationExecutor.java:68)
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer$TaskCreatingProvider.tryCreate(DefaultTaskContainer.java:678)
# 	at org.gradle.api.internal.DefaultNamedDomainObjectCollection$AbstractDomainObjectCreatingProvider.calculateOwnValue(DefaultNamedDomainObjectCollection.java:991)
# 	at org.gradle.api.internal.provider.AbstractMinimalProvider.calculateOwnPresentValue(AbstractMinimalProvider.java:80)
# 	at org.gradle.api.internal.provider.AbstractMinimalProvider.get(AbstractMinimalProvider.java:100)
# 	at org.gradle.api.internal.DefaultNamedDomainObjectCollection$AbstractDomainObjectCreatingProvider.get(DefaultNamedDomainObjectCollection.java:977)
# 	at org.gradle.api.internal.tasks.DefaultTaskContainer$TaskCreatingProvider.getProducer(DefaultTaskContainer.java:673)
# 	at org.gradle.api.internal.tasks.DefaultTaskDependency.visitDependencies(DefaultTaskDependency.java:104)
# 	at org.gradle.api.internal.tasks.CachingTaskDependencyResolveContext$TaskGraphImpl.getNodeValues(CachingTaskDependencyResolveContext.java:103)
# 	at org.gradle.internal.graph.CachingDirectedGraphWalker$GraphWithEmptyEdges.getNodeValues(CachingDirectedGraphWalker.java:213)
# 	at org.gradle.internal.graph.CachingDirectedGraphWalker.doSearch(CachingDirectedGraphWalker.java:121)
# 	at org.gradle.internal.graph.CachingDirectedGraphWalker.findValues(CachingDirectedGraphWalker.java:73)
# 	at org.gradle.api.internal.tasks.CachingTaskDependencyResolveContext.getDependencies(CachingTaskDependencyResolveContext.java:66)
# 	... 143 more
# Caused by: org.gradle.api.internal.AbstractMutationGuard$IllegalMutationException: DefaultTaskContainer#NamedDomainObjectProvider.configure(Action) on task set cannot be executed in the current context.
# 	at org.gradle.api.internal.AbstractMutationGuard.createIllegalStateException(AbstractMutationGuard.java:39)
# 	at org.gradle.api.internal.AbstractMutationGuard.assertMutationAllowed(AbstractMutationGuard.java:27)
# 	at org.gradle.api.internal.DefaultDomainObjectCollection.assertMutable(DefaultDomainObjectCollection.java:440)
# 	at org.gradle.api.internal.DefaultNamedDomainObjectCollection$ExistingNamedDomainObjectProvider.configure(DefaultNamedDomainObjectCollection.java:909)
# 	at org.gradle.api.internal.tasks.DefaultTaskCollection$ExistingTaskProvider.configure(DefaultTaskCollection.java:217)
# 	at org.gradle.api.internal.tasks.DefaultTaskCollection$ExistingTaskProvider_Decorated.configure(Unknown Source)
# 	at org.gradle.api.internal.tasks.DefaultTaskCollection$ExistingTaskProvider_Decorated$configure.call(Unknown Source)
# 	at build_dz6ba99fvbre3xk9iqaq5hp93$_run_closure3$_closure22.doCall$original(/Users/kevin/git/cpp/EscapeTheFate/android/app/build.gradle:100)
# 	at build_dz6ba99fvbre3xk9iqaq5hp93$_run_closure3$_closure22.doCall(/Users/kevin/git/cpp/EscapeTheFate/android/app/build.gradle)
# 	at java.base/jdk.internal.reflect.DirectMethodHandleAccessor.invoke(DirectMethodHandleAccessor.java:103)
# 	at org.gradle.util.internal.ClosureBackedAction.execute(ClosureBackedAction.java:73)
# 	at org.gradle.util.internal.ConfigureUtil.configureTarget(ConfigureUtil.java:155)
# 	at org.gradle.util.internal.ConfigureUtil.configureSelf(ConfigureUtil.java:131)
# 	at org.gradle.api.internal.AbstractTask.configure(AbstractTask.java:698)
# 	at org.gradle.api.DefaultTask.configure(DefaultTask.java:319)
# 	at org.gradle.api.DefaultTask.configure(DefaultTask.java:45)
# 	at org.gradle.util.internal.ConfigureUtil.configure(ConfigureUtil.java:104)
# 	at org.gradle.util.internal.ConfigureUtil$WrappedConfigureAction.execute(ConfigureUtil.java:167)
# 	at org.gradle.internal.code.DefaultUserCodeApplicationContext$CurrentApplication$1.execute(DefaultUserCodeApplicationContext.java:122)
# 	at org.gradle.api.internal.DefaultCollectionCallbackActionDecorator$BuildOperationEmittingAction$1.run(DefaultCollectionCallbackActionDecorator.java:110)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:29)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$1.execute(DefaultBuildOperationRunner.java:26)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:66)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner$2.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:157)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.execute(DefaultBuildOperationRunner.java:59)
# 	at org.gradle.internal.operations.DefaultBuildOperationRunner.run(DefaultBuildOperationRunner.java:47)
# 	at org.gradle.internal.operations.DefaultBuildOperationExecutor.run(DefaultBuildOperationExecutor.java:68)
# 	at org.gradle.api.internal.DefaultCollectionCallbackActionDecorator$BuildOperationEmittingAction.execute(DefaultCollectionCallbackActionDecorator.java:107)
# 	at org.gradle.api.internal.DefaultMutationGuard$1.execute(DefaultMutationGuard.java:45)
# 	at org.gradle.api.internal.DefaultMutationGuard$1.execute(DefaultMutationGuard.java:45)
# 	at org.gradle.api.internal.collections.CollectionFilter$1.execute(CollectionFilter.java:62)
# 	at org.gradle.internal.ImmutableActionSet$SetWithManyActions.execute(ImmutableActionSet.java:329)
# 	at org.gradle.api.internal.DefaultDomainObjectCollection.doAdd(DefaultDomainObjectCollection.java:262)
# 	at org.gradle.api.internal.DefaultNamedDomainObjectCollection.doAdd(DefaultNamedDomainObjectCollection.java:125)
# 	at org.gradle.api.internal.DefaultDomainObjectCollection.add(DefaultDomainObjectCollection.java:256)
# 	at org.gradle.api.internal.DefaultNamedDomainObjectCollection$AbstractDomainObjectCreatingProvider.tryCreate(DefaultNamedDomainObjectCollection.java:1011)
# 	... 165 more
# Fix with AI

