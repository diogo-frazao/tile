# Detect operating system
UNAME_S := $(shell uname -s)

CXX = g++

# ----------------------------
# Build configuration
# ----------------------------
BUILD ?= Debug   # set BUILD=Release for release build

ifeq ($(BUILD),Debug)
    OPTFLAGS = -g -O0
else ifeq ($(BUILD),Release)
    OPTFLAGS = -O2 -DRELEASE_BUILD
endif

# ----------------------------
# Platform-specific settings
# ----------------------------
ifeq ($(UNAME_S),Darwin)    # macOS
    CXXFLAGS = -Wall -std=c++17 $(OPTFLAGS) \
      $(shell sdl2-config --cflags) \
      -Ibin/dependencies/imgui
    LDFLAGS = -lSDL2main $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image
    TARGET = build/tile_game
    CLEAN_CMD = rm -rf build
else                        # Windows / MSYS2
    CXXFLAGS = -Wall -std=c++17 $(OPTFLAGS) \
      -Ibin/dependencies/SDL2/include \
      -Ibin/dependencies/imgui
    LDFLAGS = -Lbin/dependencies/SDL2_gcc/lib \
      -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
    ifeq ($(BUILD),Release)
      LDFLAGS += -mwindows
    endif
    TARGET = build/tile.exe
    CLEAN_CMD = rm -rf build
endif

# ----------------------------
# Sources
# ----------------------------
SRC_TILE  := $(wildcard tile/src/**/*.cpp) $(wildcard tile/src/*.cpp)
SRC_IMGUI := $(wildcard bin/dependencies/imgui/*.cpp)
SRC = $(SRC_TILE) $(SRC_IMGUI)

# Object files -> build/intermediaries/<BUILD>/
OBJ = $(patsubst %.cpp,build/intermediaries/$(BUILD)/%.o,$(SRC))

# ----------------------------
# Default target
# ----------------------------
all: $(TARGET)

# Link step
$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile step (ensure per-file directory exists)
build/intermediaries/$(BUILD)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	$(CLEAN_CMD)

.PHONY: all clean
