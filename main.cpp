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
        game.window.clear(sf::Color::Black);
        
        game.drawAll(); 
        game.window.display();
    }

    return 0;
}