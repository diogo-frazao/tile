CXX = g++
CXXFLAGS = -g -Wall \
  -Ibin/dependencies/SDL2/include \
  -Ibin/dependencies/imgui

LDFLAGS = -Lbin/dependencies/SDL2_gcc/lib \
  -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

# Source files
SRC_TILE = \
  tile/src/addSpritesScreen.cpp \
  tile/src/main.cpp \
  tile/src/mainScreen.cpp \
  tile/src/mouseScreen.cpp \
  tile/src/settingsScreen.cpp \
  tile/src/spritePreviewer.cpp \
  tile/src/core/app.cpp \
  tile/src/core/input.cpp \
  tile/src/core/spriteHandler.cpp \
  tile/src/core/ui.cpp \
  tile/src/core/widgetLink.cpp \
  tile/src/layerInspectorScreen.cpp

SRC_IMGUI = \
  bin/dependencies/imgui/imgui.cpp \
  bin/dependencies/imgui/imgui_demo.cpp \
  bin/dependencies/imgui/imgui_draw.cpp \
  bin/dependencies/imgui/imgui_impl_sdl2.cpp \
  bin/dependencies/imgui/imgui_impl_sdlrenderer2.cpp \
  bin/dependencies/imgui/imgui_tables.cpp \
  bin/dependencies/imgui/imgui_widgets.cpp

SRC = $(SRC_TILE) $(SRC_IMGUI)

# Object files go in build/
OBJ = $(patsubst %.cpp,build/%.o,$(SRC))

# Final binary
TARGET = tile.exe

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile rule: ensures build/ directories exist before compiling
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule (works on Windows with PowerShell or CMD)
clean:
	@if exist build rd /s /q build
	@if exist $(TARGET) del /q $(TARGET)

.PHONY: all clean
