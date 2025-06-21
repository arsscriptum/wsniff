TARGET := log
SOURCE := log
LWS := -lws
BASE := ../../..
SOURCE_DIR := $(BASE)/source
PLUGIN_SRC_DIR := $(SOURCE_DIR)/plugins/$(TARGET)
LOGGERSRC := $(SOURCE_DIR)/tracelogger

LOGGER_CPP_FILES := $(wildcard $(LOGGERSRC)/*.cpp)
LOGGER_OBJS := $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(LOGGER_CPP_FILES))

STD ?= c++17
COMPILATION_FLAGS ?= -std=$(STD) -O3 -fdata-sections -ffunction-sections -flto -Wall
LDFLAGS ?= -loleaut32 -lws2_32 -static -shared -Wl,--gc-sections -s -L$(BASE)/shared/ $(LWS) -lz

SOURCES := $(foreach FILE,$(SOURCE),$(FILE).cpp)
SOURCES += $(LOGGER_OBJS)

all: $(BASE)/plugins/$(TARGET).dll

$(BASE)/plugins/$(TARGET).dll: $(SOURCES)
	@echo "\033[2;91m[X]Plugin $(BASE)/plugins/$(TARGET).dll\033[0m"
	$(COMPILR_BIN) $(SOURCES) $(COMPILATION_FLAGS) -DRECORD_TO_FILE $(LDFLAGS) -I$(LOGGERSRC) -I$(SOURCE_DIR) -I../.. -o $(BASE)/plugins/$(TARGET).dll

clean:
	rm -f $(BASE)/plugins/$(TARGET).dll
