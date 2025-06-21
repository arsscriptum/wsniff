TARGET := ffxiv_pkt_log_2
SOURCE := log
LWS := -lws
BASE := ../../..
SOURCE_DIR := $(BASE)/source
STD ?= c++17


# systems Includes and Libraries for 32bits  platforms
INCLUDE_X86 = /usr/i686-w64-mingw32/include
LIB_X86 = /usr/i686-w64-mingw32/lib
# systems Includes and Libraries for 64bits platforms
INCLUDE_X64 = /usr/x86_64-w64-mingw32/include
LIB_X64 = /usr/x86_64-w64-mingw32/lib

# Determine compiler, include, and lib dirs based on build configuration
ifeq ($(BUILD_PLATFORM),x86)
    INCLUDE_DIR := $(INCLUDE_X86)
else ifeq ($(BUILD_PLATFORM),x64)
    INCLUDE_DIR := $(INCLUDE_X64)
else
    $(error Unknown BUILD_PLATFORM "$(BUILD_PLATFORM)", must be x86 or x64)
endif


COMPILATION_FLAGS ?= -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -Wall -I$(INCLUDE_DIR) -I$(SOURCE_DIR) -I../..
LDFLAGS := -loleaut32 -lws2_32 -static -shared -Wl,--gc-sections -s -L$(BASE)/shared/ $(LWS) -lz

SOURCES := $(foreach FILE,$(SOURCE),$(FILE).cpp)


all: $(BASE)/plugins/$(TARGET).dll

$(BASE)/plugins/$(TARGET).dll: $(SOURCES)
	$(COMPILR_BIN) $(SOURCES) $(COMPILATION_FLAGS) $(LDFLAGS) -I$(INCLUDE_DIR) -I$(SOURCE_DIR) -I../.. -o $(BASE)/plugins/$(TARGET).dll
	
clean:
	rm -f $(BASE)/plugins/$(TARGET).dll
