# gplante: multi platform makefile
TARGET := ws
TARGET_TYPE := dll
PLUGIN_TYPE := dll
BUILD_CONFIGURATION ?= x64
SOURCE := ws misc plugins

BASE := .
BUILD := $(BASE)/build
PLUGINS := $(BASE)/plugins
SHARED := $(BASE)/shared
SRC := $(BASE)/source

BIN_ROOT := $(BASE)/bin 
BIN_OUT_X86 := $(BASE)/bin/x86
BIN_OUT_X64 := $(BASE)/bin/x64

SHARED_X86 := $(SHARED)
SHARED_X64 := $(SHARED)
PLUGINS_X86 := $(PLUGINS)
PLUGINS_X64 := $(PLUGINS)

TARGET_OUT_X86 := $(BIN_OUT_X86)/$(TARGET).dll
TARGET_OUT_X64 := $(BIN_OUT_X64)/$(TARGET).dll

PLUGINS_BUILD_INFO := $(BASE)/plugins/$(BUILD_CONFIGURATION).nfo

CC64 = x86_64-w64-mingw32-gcc
CC32 = i686-w64-mingw32-gcc

INCLUDE_X86 = /usr/i686-w64-mingw32/include
LIB_X86 = /usr/i686-w64-mingw32/lib

INCLUDE_X64 = /usr/x86_64-w64-mingw32/include
LIB_X64 = /usr/x86_64-w64-mingw32/lib

# Determine compiler, include, and lib dirs based on build configuration
ifeq ($(BUILD_CONFIGURATION),x86)
    CC := $(CC32)
    INCLUDE_DIR := $(INCLUDE_X86)
    LIB_DIR := $(LIB_X86)
	SHARED := $(SHARED_X86)
	PLUGINS := $(PLUGINS_X86)
	TARGET_OUT := $(TARGET_OUT_X86)
	BIN_OUT := $(BIN_OUT_X86)
	PLATFORM_DEFINE := __PLATFORM_X86__
else ifeq ($(BUILD_CONFIGURATION),x64)
    CC := $(CC64)
    INCLUDE_DIR := $(INCLUDE_X64)
    LIB_DIR := $(LIB_X64)
	SHARED := $(SHARED_X64)
	PLUGINS := $(PLUGINS_X64)
	TARGET_OUT := $(TARGET_OUT_X64)
	BIN_OUT := $(BIN_OUT_X64)
	PLATFORM_DEFINE := __PLATFORM_X64__
else
    $(error Unknown BUILD_CONFIGURATION "$(BUILD_CONFIGURATION)", must be x86 or x64)
endif

# Flags
STD := c99
CFLAGS := -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -DLOGGING_ENABLED -D$(PLATFORM_DEFINE) -DEXPORT -Wall -shared \
          -I$(INCLUDE_DIR) -L$(LIB_DIR) \
          -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LDFLAGS := -L$(LIB_DIR) -lwsock32 -liphlpapi -lpsapi -static -shared -Wl,--gc-sections -Wl,--out-implib,shared/lib$(TARGET).a -s


SOURCES := $(foreach FILE,$(SOURCE),$(FILE).c)
O_SOURCE := $(foreach FILE,$(SOURCES),$(SRC)/$(FILE))

OBJ := $(foreach FILE,$(SOURCE),$(FILE).o)
O_OBJS := $(foreach FILE,$(OBJ),$(BUILD)/$(FILE))

#Confusing, I know, this is to build every plugin subdirectory
PLUGINSRC := $(wildcard $(SRC)/$(PLUGINS)/*/.)

.PHONY: $(PLUGINSRC) #Gotta run this regardless of timestamp on the folder, let the plugin Makefile handle things 

x64:
	$(MAKE) all

x86:
	$(MAKE) BUILD_CONFIGURATION=x86 all

all: $(BIN_OUT) $(BUILD) $(SHARED) $(PLUGINS) print-config $(TARGET_OUT) $(PLUGINSRC)

print-config:
	@echo  "\n"
	@echo "\033[3;33m==== Building for $(BUILD_CONFIGURATION) using $(CC) ====\033[0m"
	@echo  "\n"

plugin: $(PLUGINS) $(PLUGINSRC)

ws: $(BUILD) $(SHARED) $(TARGET_OUT)

clean:
	rm -rf $(BIN_ROOT) $(BUILD) $(PLUGINS) $(SHARED) $(TARGET_OUT) *.dll


$(TARGET_OUT): $(O_OBJS)
	$(CC) -o $@ $(O_OBJS) $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) $(SRC)/$*.c -o $@

$(SRC)/%.c: $(SRC)/%.h

#Plugins
$(PLUGINSRC):
	$(MAKE) -C $@ CC="$(CC)" CFLAGS="$(CFLAGS)"


#Make directories if necessary
$(BUILD):
	mkdir $(BUILD)

$(PLUGINS):
	mkdir -p $(PLUGINS)
	echo "# These libraries are build for the $(BUILD_CONFIGURATION) platform" > $(PLUGINS_BUILD_INFO)

$(SHARED):
	mkdir -p $(SHARED)

$(BIN_OUT):
	mkdir -p $(BIN_OUT)
	