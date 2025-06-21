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

SHARED_X86 := $(SHARED)
SHARED_X64 := $(SHARED)
PLUGINS_X86 := $(PLUGINS)
PLUGINS_X64 := $(PLUGINS)
TARGET_OUT_X86 := $(TARGET)x86.$(TARGET_TYPE)
TARGET_OUT_X64 := $(TARGET)x64.$(TARGET_TYPE)



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
else ifeq ($(BUILD_CONFIGURATION),x64)
    CC := $(CC64)
    INCLUDE_DIR := $(INCLUDE_X64)
    LIB_DIR := $(LIB_X64)
	SHARED := $(SHARED_X64)
	PLUGINS := $(PLUGINS_X64)
	TARGET_OUT := $(TARGET_OUT_X64)
else
    $(error Unknown BUILD_CONFIGURATION "$(BUILD_CONFIGURATION)", must be x86 or x64)
endif

# Flags
STD := c99
CFLAGS := -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -DEXPORT -Wall -shared -I$(INCLUDE_DIR) -L$(LIB_DIR)
LDFLAGS := -L$(LIB_DIR) -lwsock32 -liphlpapi -lpsapi -static -shared -Wl,--gc-sections -Wl,--out-implib,shared/lib$(TARGET).a -s


SOURCES := $(foreach FILE,$(SOURCE),$(FILE).c)
O_SOURCE := $(foreach FILE,$(SOURCES),$(SRC)/$(FILE))

OBJ := $(foreach FILE,$(SOURCE),$(FILE).o)
O_OBJS := $(foreach FILE,$(OBJ),$(BUILD)/$(FILE))

#Confusing, I know, this is to build every plugin subdirectory
PLUGINSRC := $(wildcard $(SRC)/$(PLUGINS)/*/.)

.PHONY: $(PLUGINSRC) #Gotta run this regardless of timestamp on the folder, let the plugin Makefile handle things 

x86:
	$(MAKE) CC=$(CC32) BUILD_CONFIGURATION=x86 all

x64:
	$(MAKE) CC=$(CC64) BUILD_CONFIGURATION=x64 all

all: clean $(BUILD) $(SHARED) $(TARGET_OUT) $(PLUGINS) $(PLUGINSRC)

plugin: $(PLUGINS) $(PLUGINSRC)

ws: $(BUILD) $(SHARED) $(TARGET_OUT)

clean:
	rm -rf $(BUILD) $(PLUGINS) $(SHARED) $(TARGET_OUT) *.dll

$(TARGET_OUT): $(O_OBJS)
	$(CC) -o $@ $(O_OBJS) $(LDFLAGS)

$(BUILD)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) $(SRC)/$*.c -o $@

$(SRC)/%.c: $(SRC)/%.h

#Plugins
$(PLUGINSRC):
	$(MAKE) -C $@ CC="$(CC)"

#Make directories if necessary
$(BUILD):
	mkdir $(BUILD)

$(PLUGINS):
	mkdir -p $(PLUGINS)/$(BUILD_CONFIGURATION)

$(SHARED):
	mkdir -p $(SHARED)
