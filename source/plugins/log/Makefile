TARGET := log
SOURCE := log
TARGET_TYPE := dll
PLUGIN_TYPE := dll
BUILD_PLATFORM ?= x64
BUILD_CONFIGURATION ?= debug
DEJAINSIGHT_ENABLED ?= yes
COMPILER ?= cpp
PARALLEL_BUILD ?= yes
NUM_PROCS := $(shell nproc)

BASE := $(realpath ../../..)
SHARED_DIR := $(BASE)/shared
SRC_DIR := $(BASE)/source
BUILD_DIR := $(BASE)/build
PLUGINS_BIN_DIR := $(BASE)/plugins
PLUGINS_SRC_DIR := $(SRC_DIR)/plugins
TRACELOGGER_SRC_DIR := $(SRC_DIR)/tracelogger
TRACELOGGER_BUILD_DIR := $(BUILD_DIR)/tracelogger
PLUGIN_SRC_DIR := $(SRC_DIR)/plugins/$(TARGET)
PLUGIN_FULL_PATH := $(BASE)/plugins/$(TARGET).dll
LOGGER_CPP_FILES := $(wildcard $(TRACELOGGER_SRC_DIR)/*.cpp)
LOGGER_OBJS := $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(LOGGER_CPP_FILES))


PLUGINS_BUILD_INFO := $(BASE)/plugins/$(BUILD_PLATFORM).nfo

DEJA_INCLUDES := $(BASE)/externals/dejainsight/include
DEJA_LIBS := $(BASE)/externals/dejainsight/lib
DEJA_WINDOWS_LIBRARY := DejaInsight.$(BUILD_PLATFORM).lib
DEJA_LINUX_LIBRARY := libDejaInsight.linux64.a
DEJA_LINUX_MODULE := libDejaInsight.linux64.so.1.0
DEJA_DLL_NAME := DejaInsight.$(BUILD_PLATFORM).dll

# C compiler for 32bits and 64bits platforms
CC64 = x86_64-w64-mingw32-gcc
CC32 = i686-w64-mingw32-g++
# C++ compiler for 32bits and 64bits platforms
CCXX64 = x86_64-w64-mingw32-g++
CCXX32 = i686-w64-mingw32-g++

# systems Includes and Libraries for 32bits  platforms
INCLUDE_X86 = /usr/i686-w64-mingw32/include
LIB_X86 = /usr/i686-w64-mingw32/lib
# systems Includes and Libraries for 64bits platforms
INCLUDE_X64 = /usr/x86_64-w64-mingw32/include
LIB_X64 = /usr/x86_64-w64-mingw32/lib

# Determine compiler, include, and lib dirs based on build configuration
ifeq ($(BUILD_PLATFORM),x86)
    CC := $(CC32)
    CCXX := $(CCXX32)
    INCLUDE_DIR := $(INCLUDE_X86)
    LIB_DIR := $(LIB_X86)
else ifeq ($(BUILD_PLATFORM),x64)
    CC := $(CC64)
	CCXX := $(CCXX64)
    INCLUDE_DIR := $(INCLUDE_X64)
    LIB_DIR := $(LIB_X64)
else
    $(error Unknown BUILD_PLATFORM "$(BUILD_PLATFORM)", must be x86 or x64)
endif


# Flags
STD := c++17

COMPILATION_FLAGS := -I$(INCLUDE_DIR) -I$(TRACELOGGER_SRC_DIR) -I$(SRC_DIR) -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -Wall -I$(INCLUDE_DIR) -I$(SOURCE_DIR) -I../..
      
COMPILATION_PREPROCESSOR_DEFS := -DTARGET_LINUX -DDEJA_TARGET_LINUX -D__linux -DCOMPILING_DLL
LINKING_LIBRARIES := -loleaut32 -lws2_32 -lws -lz
LDFLAGS := -static -shared -Wl,--gc-sections -s -L$(BASE)/shared/ 

# parallel compilation explicitely

ifeq ($(PARALLEL_BUILD),yes)
	COMPILATION_FLAGS += -flto=$(NUM_PROCS)
	LDFLAGS += -flto=$(NUM_PROCS)
	COMPILATION_PREPROCESSOR_DEFS += -DENABLE_PARALLEL_BUILD=yes -DENABLE_PARALLEL_LINK=yes -DPARALLEL_BUILD_PROCS=$(NUM_PROCS)
else 
	COMPILATION_PREPROCESSOR_DEFS += -DENABLE_PARALLEL_BUILD=no -DENABLE_PARALLEL_LINK=no
endif

ifeq ($(COMPILER),c)
    COMPILR_BIN := $(CC)
	COMPILATION_FLAGS += -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
else ifeq ($(COMPILER),cpp)
    COMPILR_BIN := $(CCXX)
	COMPILATION_FLAGS += -Wno-int-to-pointer-cast
else
    $(error Unknown COMPILER "$(COMPILER)", must be c or cpp)
endif

# FORCE DEJA_DISABLED, no libs...
ifeq ($(BUILD_PLATFORM),x86)
	DEJAINSIGHT_ENABLED := no
	PLATFORM_DEFINE := __PLATFORM_X86__
	COMPILATION_PREPROCESSOR_DEFS += -DWIN32 -D_WIN32 -D$(PLATFORM_DEFINE)
else ifeq ($(BUILD_PLATFORM),x64)
	PLATFORM_DEFINE := __PLATFORM_X64__
	COMPILATION_PREPROCESSOR_DEFS += -DWIN64 -D_WIN64 -D_AMD64_ -D__MINGW64__ -D$(PLATFORM_DEFINE)
else
    $(error Unknown BUILD_PLATFORM "$(BUILD_PLATFORM)", must be x86 or x64)
endif

ifeq ($(BUILD_CONFIGURATION),debug)
	COMPILATION_FLAGS += -g -Og -fvar-tracking -fno-eliminate-unused-debug-symbols -femit-class-debug-always
else ifeq ($(BUILD_CONFIGURATION),release)
	COMPILATION_FLAGS += -Os -flto
else ifeq ($(BUILD_CONFIGURATION),relfast1)
	COMPILATION_FLAGS += -fprofile-arcs -ffast-math -fwhole-program -faggressive-loop-optimizations -finline-functions -foptimize-crc -foptimize-strlen
else ifeq ($(BUILD_CONFIGURATION),relfast2)
	COMPILATION_FLAGS += -fbranch-probabilities -ffast-math -fwhole-program -faggressive-loop-optimizations -finline-functions -foptimize-crc -foptimize-strlen
else
    $(error Unknown BUILD_PLATFORM "$(BUILD_PLATFORM)", must be x86 or x64)
endif

ifeq ($(DEJAINSIGHT_ENABLED),yes)
	COMPILATION_FLAGS += -I$(DEJA_INCLUDES)
	LINKING_LIBRARIES += -lDejaInsight.linux64
	COMPILATION_PREPROCESSOR_DEFS += -DDEJA_ENABLED
	LDFLAGS += -L$(DEJA_LIBS)
else ifeq ($(DEJAINSIGHT_ENABLED),no)
	COMPILATION_PREPROCESSOR_DEFS += -DDEJA_DISABLED
else
    $(error Unknown DEJAINSIGHT_ENABLED "$(DEJAINSIGHT_ENABLED)", must be yes or no)
endif

COMPILATION_FLAGS += $(COMPILATION_PREPROCESSOR_DEFS)
LDFLAGS += $(LINKING_LIBRARIES)

LOGGER_CPP_FILES := $(wildcard $(TRACELOGGER_SRC_DIR)/*.cpp)
LOGGER_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LOGGER_CPP_FILES))

SOURCES := $(foreach FILE,$(SOURCE),$(FILE).cpp)
SOURCES += $(LOGGER_OBJS)

log: 
	@echo "\033[2;91m [x] plugin $(PLUGIN_FULL_PATH)\033[0m"
	$(MAKE) $(PLUGIN_FULL_PATH)
	
$(PLUGIN_FULL_PATH):
	$(COMPILR_BIN) $(SOURCES) $(COMPILATION_FLAGS)  $(LDFLAGS) -o $(PLUGIN_FULL_PATH)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILR_BIN) -c $(COMPILATION_FLAGS) $(SRC_DIR)/$*.cpp -o $@

$(SRC_DIR)/%.cpp: $(SRC_DIR)/%.h


clean:
	rm -f $(PLUGIN_FULL_PATH)
