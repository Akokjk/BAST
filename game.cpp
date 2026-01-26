#include "common.hpp"

Game::Game(int w, int h, const std::string& title) : width(w), height(h) {
    window.create(sf::VideoMode({width, height}), title);
    if (!roboto_font.openFromFile("resources/Roboto-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Error loading roboto font!" << std::endl;
        exit(1); 
    }
    Loaders::loadMenu(*this);
    Loaders::loadBuy(*this);
    //Loaders::loadAuction(*this);
   
}

void Game::drawAll() {
    for (const auto& name : drawOrder) {
        auto& item = objects[name];
        // Filter: Only draw if Global or matches current state
        if (item.state != State::Global && item.state != currentState) {
            continue; 
        }

        const auto& type = item.object.type();

        // Check against known Drawable types from your inheritance diagram
        if (type == typeid(sf::Text)) {
            window.draw(std::any_cast<sf::Text&>(item.object));
        }
        else if (type == typeid(sf::Sprite)) {
            window.draw(std::any_cast<sf::Sprite&>(item.object));
        }
        else if (type == typeid(sf::RectangleShape)) {
            window.draw(std::any_cast<sf::RectangleShape&>(item.object));
        }
        else if (type == typeid(sf::CircleShape)) {
            window.draw(std::any_cast<sf::CircleShape&>(item.object));
        }
        // If it's a Card object or a Vector2f, this loop just skips it!
    }
}

void Game::update(float dt) {
    // 1. Example: Global animation (Rotating the title)
    try {
        auto& title = get<sf::Text>("title");
        title.rotate(sf::degrees(45.0f * dt)); // Rotate 45 degrees per second
    } catch (...) {
        // If "Title" doesn't exist, just keep going
    }

    // 2. Example: Logic based on Data
    // If you stored a "isHovered" boolean for a card:
    // bool hovered = getData<bool>("Card_Hovered");
    // if (hovered) { ... scale up the sprite ... }
}

void Game::events(){
    while (const std::optional event = window.pollEvent()) {
            //Global Events 
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                if (keyPressed->code == sf::Keyboard::Key::Num1) {
                    currentState = State::Menu; 
                    get<sf::Text>("title").setString("Herro"); //an annoying artifact of being safe for c++, always going to have recast to the correct type when modifying it 
                }
                if (keyPressed->code == sf::Keyboard::Key::Num2) {
                    currentState = State::Buy; 
                }
                if (keyPressed->code == sf::Keyboard::Key::Num3) {
                    currentState = State::Auction; 
                }
                if (keyPressed->code == sf::Keyboard::Key::Num4) {
                    currentState = State::Sell; 
                }
                if (keyPressed->code == sf::Keyboard::Key::Num5) {
                    currentState = State::Trade; 
                }
            }

            //state specific events 
            // --- STATE-SPECIFIC EVENTS ---
            switch(currentState){
                case State::Menu: 
                    break; 
                case State::Buy: 
                    break; 
                case State::Sell: 
                    break; 
                case State::Auction: 
                    break; 
                case State::Trade: 
                    break; 
                default: 
                    break; 
            }
            
        }
}