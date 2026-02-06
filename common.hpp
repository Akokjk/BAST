#ifndef COMMON_HPP
#define COMMON_HPP

// 1. Essential Macro for Static Linking
#ifndef SFML_STATIC
#define SFML_STATIC
#endif

enum class State { Global, Menu, Buy, Sell, Auction,  Trade };

#define PI 3.14159265



// 2. SFML 3 Includes
#include <glad/glad.h> // Include GLAD before everything else
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


// 3. Standard Library Includes
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <optional>
#include <any>
#include <fstream>
#include <algorithm>
#include <random>
#include <math.h>
#include <sstream>


#include "game.hpp"
#include "state_loader.hpp"
#include "card.hpp"
#include "gl_surface.hpp"






#endif