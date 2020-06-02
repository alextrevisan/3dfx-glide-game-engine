#EXECUTABLE NAME
TARGET := game.exe

##COMPILER
CC := g++
LD := g++

##FLAGS
CFLAGS:= -std=c++1y -O3 -Wall

#BUILD FOLDERS
MODULES := engine game
SRC_DIR := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES := -Iinclude $(addprefix -I,$(SRC_DIR))


vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs build/$(TARGET)

build/$(TARGET): $(OBJ)
	$(LD) $^ -o $@

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))