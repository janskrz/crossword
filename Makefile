CXX = g++

TARGET_DIR ?= ./bin
TARGET_EXEC ?= generator

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
INC_DIRS ?= $(SRC_DIRS) ./lib/inih

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INCS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INCS))

CPPFLAGS ?= $(INC_FLAGS) -Werror -Wall -Wextra -pedantic

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
