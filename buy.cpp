 #include "common.hpp"

//buy state resource creation 
void Loaders::loadBuy(Game& game) {
    game.loadTexture("card_texture", "resources/card_draft.png");
    sf::Sprite card(*game.textures["card_texture"]);
   card.setScale({5.f, 5.f});
    card.setPosition({100.f, 100.f});
   game.push("Card", card, State::Buy);

   sf::Text buyLabel(game.roboto_font, "Buy Mode", 20);
    game.push("BuyLabel", buyLabel, State::Buy);
    Card card_test; 
    card_test.create(game); 
}


 