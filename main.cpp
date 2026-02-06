#include "common.hpp"
int main() {

    Game game(500,500,"BAST"); 

    sf::Clock clock; 
    sf::Clock clock1; //to get dt correctly 
    // Main loop
    
    GLSurface mySurface;
    mySurface.create(500, 500); // Create a 400x300 OpenGL "sub-window"

    
    //mySurface.renderScene();
    sf::Sprite glSprite(mySurface.getTexture());
  
    //glSprite.setTextureRect(sf::IntRect({10, 10}, {32, 32}));
    glSprite.setPosition({100, 100}); // Move the OpenGL surface anywhere!
   
    // 1. Update the OpenGL Surface need to put into draw stage 
    
    while (game.window.isOpen()) {
        float dt = clock1.restart().asSeconds() ;
        //poll events 
        game.events(); 
        game.update(dt);
        // Render logic
        game.window.clear(sf::Color({36, 42, 51, 255}));
       // game.glWindow.clear(sf::Color({36, 42, 51, 255}));
        mySurface.renderScene(clock.getElapsedTime().asSeconds());
        glSprite.setTexture(mySurface.getTexture());
        game.window.resetGLStates();
        game.window.draw(glSprite); 
        game.drawAll(); 
        
        game.window.display();
        //game.glWindow.display();

    }

    return 0;
}