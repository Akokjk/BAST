 #include "common.hpp"

//menu state resource creation 
void Loaders::loadMenu(Game& game) {
   

    // 2. Create a text object
    sf::Text text(game.roboto_font, "test");
    text.setString("Buy Auction Sell Trade");
    text.setCharacterSize(20); // in pixels
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition({50.f, 50.f});

    game.push("title", text, State::Menu);   
    
}


 