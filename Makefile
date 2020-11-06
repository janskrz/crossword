CXX = g++

TARGET_DIR ?= ./bin
TARGET_EXEC ?= generator
TARGET_CONFIG ?= config.ini
TARGET_EXAMPLE_WORDLIST ?= examplewordlist.csv

BUILD_DIR ?= ./build
RES_DIR ?= ./res
SRC_DIRS ?= ./src
INC_DIRS ?= $(SRC_DIRS) ./lib/inih

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INCS := $(shell find $(INC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INCS))

CPPFLAGS ?= $(INC_FLAGS) -Werror -Wall -Wextra -pedantic -std=c++17

all: $(TARGET_DIR)/$(TARGET_EXEC) $(TARGET_DIR)/$(TARGET_CONFIG) $(TARGET_DIR)/$(TARGET_EXAMPLE_WORDLIST)

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TARGET_DIR)/$(TARGET_CONFIG): $(RES_DIR)/config.ini.default
	cp $< $@

$(TARGET_DIR)/$(TARGET_EXAMPLE_WORDLIST): $(RES_DIR)/$(TARGET_EXAMPLE_WORDLIST)
	cp $< $@

.PHONY: clean all

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
