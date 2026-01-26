#include "common.hpp"
void Card::create(Game& game){
	location = sf::Vector2f({300.f, 100.f});
	title = "Secret Dev Card";
	description = "This is the very first card in the game I hope you like it. -A";
	logo = "BAST";
	height = 140; //card have a 1.4:1 ration apprently?!
	width = 100; 
	font = game.roboto_font; 
	border_thickness = 5; 
	id = 1; 
	owner = "Aaron"; 
	price = centering = cornering = edges = surface = 0.f; 
	outer_border = sf::Color({0,40,40,255}); 
	inner_border = sf::Color({40,0,40,255});
	inner_body = sf::Color({40,40,0,255});
	center_bg =   sf::Color({40,40,40,255});

	sf::RectangleShape card; 
	card.setSize(sf::Vector2f(width, height)); 
	card.setOutlineColor(outer_border); 
	card.setFillColor(inner_body); 
	card.setOutlineThickness(border_thickness); 
	card.setPosition(location);

	game.push("Dev_Card", card, State::Buy);


}