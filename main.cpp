#include "common.hpp"
int main() {

    Game game(500,500,"BAST"); 

    sf::Clock clock; 
    // Main loop
    while (game.window.isOpen()) {
        float dt = clock.restart().asSeconds();
        //poll events 
        game.events(); 
        game.update(dt);
        // Render logic
        game.window.clear(sf::Color({36, 42, 51, 255}));
        
        game.drawAll(); 
        game.window.display();
    }

    return 0;
}