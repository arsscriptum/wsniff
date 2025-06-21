TARGET := log
SOURCE := log
LWS := -lws
BASE := ../../..

STD ?= c++17
COMPILATION_FLAGS ?= -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -Wall
LDFLAGS ?= -loleaut32 -lws2_32 -static -shared -Wl,--gc-sections -s -L$(BASE)/shared/ $(LWS) -lz

SOURCES := $(foreach FILE,$(SOURCE),$(FILE).c)

buildnotice:
	@echo  "\n\n"
	@echo "\033[2;94m==== COMPILATION of PLUGIN $(TARGET) ====\033[0m"
	@echo  "\n"


all: buildnotice $(BASE)/plugins/$(TARGET).dll


$(BASE)/plugins/$(TARGET).dll: $(SOURCES)
	$(COMPILR_BIN) $(SOURCES) $(COMPILATION_FLAGS) $(LDFLAGS) -o $(BASE)/plugins/$(TARGET).dll

clean:
	rm -f $(BASE)/plugins/$(TARGET).dll
