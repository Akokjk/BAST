#ifndef GAME_HPP
#define GAME_HPP

#include "common.hpp"




class Game {
private:
   
    State currentState = State::Menu;

    struct RegistryItem {
        std::any object;
        State state;
    };

    std::unordered_map<std::string, RegistryItem> objects;
    float cardRotation = 0.0f;
    std::vector<std::string> drawOrder;

public:
    sf::RenderTexture cardCanvas;
    float fTheta = cardRotation * 3.14159f / 180.0f;
     unsigned int width, height;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    Game(int w, int h, const std::string& title);
    sf::RenderWindow window;
    sf::Font roboto_font; 
    // Helper function to get a texture safely
    void loadTexture(const std::string& name, const std::string& path) {
        auto tex = std::make_unique<sf::Texture>();
        if (tex->loadFromFile(path)) {
            tex->setSmooth(false); // Nearest neighbor for pixel art
            textures[name] = std::move(tex);
        } else {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        
    }
    // Creation: Stores the object and its state
    template <typename T>
    void push(const std::string& name, T object, State state = State::Global) {
        objects[name] = { std::make_any<T>(object), state };
        drawOrder.push_back(name); // Preserves insertion order for drawing
    }

    // Retrieval: Automatically recasts std::any to the requested type
    template <typename T>
    T& get(const std::string& name) {
        try {
            return std::any_cast<T&>(objects.at(name).object);
        } catch (const std::bad_any_cast&) {
            throw std::runtime_error("Type mismatch for object: " + name);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Object not found: " + name);
        }
    }

    void setState(State newState) { currentState = newState; }
    void drawAll();
    void update(float dt);
    void events();
};

#endif