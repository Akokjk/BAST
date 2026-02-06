# --- Paths ---
SFML_DIR = C:/Users/shaba/Documents/c++/SFML-master
BAST_INC = ./include
LIB_DIR = $(SFML_DIR)/build/lib

# --- Compiler Settings ---
CXX = g++
CXXFLAGS = -g -I$(SFML_DIR)/include -I$(BAST_INC) -DSFML_STATIC -std=c++17 -Wall -Wextra

# --- Linker Settings ---
LDFLAGS = -L$(LIB_DIR)
LIBS = -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-network-s -lsfml-system-s \
       -lopengl32 -lwinmm -lgdi32 -lws2_32 -lfreetype -lharfbuzz -lflac \
       -lvorbisenc -lvorbisfile -lvorbis -logg

# --- Build Targets ---
SRC = main.cpp game.cpp menu.cpp buy.cpp card.cpp gl_surface.cpp glad.c
# This logic converts .cpp -> .o AND .c -> .o
OBJ = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SRC)))
EXE = app.exe

# --- Rules ---

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /f *.o $(EXE)