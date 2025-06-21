TARGET := ffxiv_pkt_log
SOURCE := log
LWS := -lws
BASE := ../../..
SOURCE_DIR := $(BASE)/source

STD ?= c++17
COMPILATION_FLAGS ?= -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -Wall
LDFLAGS ?= -loleaut32 -lws2_32 -static -shared -Wl,--gc-sections -s -L$(BASE)/shared/ $(LWS) -lz

SOURCES := $(foreach FILE,$(SOURCE),$(FILE).cpp)


all: $(BASE)/plugins/$(TARGET).dll

$(BASE)/plugins/$(TARGET).dll: $(SOURCES)
	$(COMPILR_BIN) $(SOURCES) $(COMPILATION_FLAGS) $(LDFLAGS) -I$(SOURCE_DIR) -I../.. -o $(BASE)/plugins/$(TARGET).dll

clean:
	rm -f $(BASE)/plugins/$(TARGET).dll
