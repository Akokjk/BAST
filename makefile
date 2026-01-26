# --- Paths ---
SFML_DIR = C:/Users/shaba/Documents/c++/SFML-master
INCLUDE_DIR = $(SFML_DIR)/include
LIB_DIR = $(SFML_DIR)/build/lib

# --- Compiler Settings ---
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR) -DSFML_STATIC -std=c++17 -Wall -Wextra

# --- Linker Settings ---
LDFLAGS = -L$(LIB_DIR)
# Note: The order of these libraries is critical for static linking
LIBS = -lsfml-graphics-s \
       -lsfml-window-s \
       -lsfml-audio-s \
       -lsfml-network-s \
       -lsfml-system-s \
       -lopengl32 \
       -lwinmm \
       -lgdi32 \
       -lws2_32 \
       -lfreetype \
       -lharfbuzz \
       -lflac \
       -lvorbisenc \
       -lvorbisfile \
       -lvorbis \
       -logg

# --- Build Targets ---
SRC = main.cpp game.cpp menu.cpp buy.cpp card.cpp
OBJ = $(SRC:.cpp=.o)
EXE = app.exe

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $(EXE) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	del /f $(OBJ) $(EXE)