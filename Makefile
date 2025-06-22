# =================================
# gplante: multi platform makefile
# =================================
TARGET := ws
TARGET_TYPE := dll
PLUGIN_TYPE := dll
BUILD_PLATFORM ?= x64
BUILD_CONFIGURATION ?= debug
DEJAINSIGHT_ENABLED ?= no
COMPILER ?= cpp
PARALLEL_BUILD := yes
SOURCE := ws misc plugins
NUM_PROCS := $(shell nproc)

# Paths
PARENT := $(realpath ..)
BASE := $(PARENT)/wsniff
BUILD := $(BASE)/build
PLUGINS := $(BASE)/plugins
SHARED := $(BASE)/shared
SRC := $(BASE)/source
LOGGERSRC := $(SRC)/tracelogger
LOGGEROUT := $(BUILD)/tracelogger

BIN_ROOT := $(BASE)/bin 
BIN_OUT_X86 := $(BASE)/bin/x86
BIN_OUT_X64 := $(BASE)/bin/x64

# Was initially used to add a path like x86/ before the lib, but not used anymore, 
# I keep it in case I change my mind again
SHARED_X86 := $(SHARED)
SHARED_X64 := $(SHARED)
PLUGINS_X86 := $(PLUGINS)
PLUGINS_X64 := $(PLUGINS)

TARGET_OUT_X86 := $(BIN_OUT_X86)/$(TARGET).dll
TARGET_OUT_X64 := $(BIN_OUT_X64)/$(TARGET).dll

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
	SHARED := $(SHARED_X86)
	PLUGINS := $(PLUGINS_X86)
	TARGET_OUT := $(TARGET_OUT_X86)
	BIN_OUT := $(BIN_OUT_X86)
else ifeq ($(BUILD_PLATFORM),x64)
    CC := $(CC64)
	CCXX := $(CCXX64)
    INCLUDE_DIR := $(INCLUDE_X64)
    LIB_DIR := $(LIB_X64)
	SHARED := $(SHARED_X64)
	PLUGINS := $(PLUGINS_X64)
	TARGET_OUT := $(TARGET_OUT_X64)
	BIN_OUT := $(BIN_OUT_X64)
else
    $(error Unknown BUILD_PLATFORM "$(BUILD_PLATFORM)", must be x86 or x64)
endif

# Flags
STD := c++17

COMPILATION_FLAGS := -std=$(STD) -I$(LOGGERSRC) -I$(INCLUDE_DIR) -fdata-sections -ffunction-sections -flto  -Wall -shared \
        -fkeep-inline-functions -fkeep-static-functions -fkeep-static-consts
        
COMPILATION_PREPROCESSOR_DEFS += -DTARGET_LINUX -DDEJA_TARGET_LINUX -D__linux -DCOMPILING_DLL
LINKING_LIBRARIES := -lwsock32 -liphlpapi -lpsapi -loleaut32
LDFLAGS := -L$(LIB_DIR) -shared -Wl,--gc-sections -Wl,--out-implib,shared/lib$(TARGET).a -s

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
	COMPILATION_FLAGS += -Os
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

SOURCES := $(foreach FILE,$(SOURCE),$(FILE).cpp)
O_SOURCE := $(foreach FILE,$(SOURCES),$(SRC)/$(FILE))

OBJ := $(foreach FILE,$(SOURCE),$(FILE).o)
O_OBJS := $(foreach FILE,$(OBJ),$(BUILD)/$(FILE))

LOGGER_CPP_FILES := $(wildcard $(LOGGERSRC)/*.cpp)
LOGGER_OBJS := $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(LOGGER_CPP_FILES))


#Confusing, I know, this is to build every plugin subdirectory
PLUGINSRC := $(wildcard $(SRC)/$(PLUGINS)/*/.)

.PHONY: all printerror $(PLUGINSRC) #Gotta run this regardless of timestamp on the folder, let the plugin Makefile handle things 

debug:
	@$(MAKE) all || { $(MAKE) printerror; exit 1; }

release:
	$(MAKE) BUILD_CONFIGURATION=release all || { $(MAKE) printerror; exit 1; }

x64:
	$(MAKE) clean all

x86:
	$(MAKE) BUILD_PLATFORM=x86 all || { $(MAKE) printerror; exit 1; }

all: $(BIN_OUT) $(BUILD) $(SHARED) $(PLUGINS) printbeginbuild $(TARGET_OUT) $(PLUGINSRC)

printbeginbuild:
	@echo  "\n"
	@echo "\033[5;32m ===== BUILD STARTED! =====\033[0m"
	@echo "\033[2;93m platform..........: $(BUILD_PLATFORM)\033[0m"
	@echo "\033[2;93m configuration.....: $(BUILD_CONFIGURATION)\033[0m"
	@echo "\033[2;94m deja_insight......: $(DEJAINSIGHT_ENABLED)\033[0m"
	@echo "\033[2;93m compiled in ......: $(COMPILER) using $(COMPILR_BIN) \033[0m"
	@echo "\033[2;96m link with.........: $(LINKING_LIBRARIES)\033[0m"
	@echo "\033[2;96m parallel builds...: $(PARALLEL_BUILD), $(NUM_PROCS) processors\033[0m"
	@echo "\033[2;96m defines...........: $(COMPILATION_PREPROCESSOR_DEFS)\033[0m"
	@echo  "\n"

printclean:
	@echo  "\n\n"
	@echo "\033[2;36m==== ===================================== ====\033[0m"
	@echo "\033[2;36m==== CLEANING BINARIES and TEMPORARY FILES ====\033[0m"
	@echo  "\n"

printerror:
	@echo  "\n\n"
	@echo "\033[5;31m==== COMPILATION ERROR OCCURED ====\033[0m"
	@echo  "\n"


plugin: $(PLUGINS) $(PLUGINSRC)

ws: $(BUILD) $(SHARED) $(TARGET_OUT)

clean:
	$(MAKE) printclean
	rm -rf $(BIN_ROOT) $(BUILD) $(PLUGINS) $(SHARED) $(TARGET_OUT) *.dll


$(TARGET_OUT): $(O_OBJS) $(LOGGER_OBJS)
	$(COMPILR_BIN) -o $@ $(O_OBJS) $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(COMPILR_BIN) -c $(COMPILATION_FLAGS) $(SRC)/$*.cpp -o $@

$(SRC)/%.cpp: $(SRC)/%.h

#Plugins
$(PLUGINSRC):
	$(MAKE) -C $@ COMPILR_BIN="$(COMPILR_BIN)" COMPILATION_FLAGS="$(COMPILATION_FLAGS)" BUILD_PLATFORM="$(BUILD_PLATFORM)" BUILD_CONFIGURATION="$(BUILD_CONFIGURATION)" DEJAINSIGHT_ENABLED="$(DEJAINSIGHT_ENABLED)"


#Make directories if necessary
$(BUILD):
	mkdir -p $(BUILD)
	mkdir -p $(LOGGEROUT)

$(PLUGINS):
	mkdir -p $(PLUGINS)
	echo "# These libraries are build for the $(BUILD_PLATFORM) platform" > $(PLUGINS_BUILD_INFO)

listdlls:
	@echo "\033[5;31m==== COMPILED DLLS ====\033[0m"
	@find "$(BASE)" -type f -iname "*.dll" | sed 's|$(BASE)/||'

$(SHARED):
	mkdir -p $(SHARED)

$(BIN_OUT):
	mkdir -p $(BIN_OUT)
	
