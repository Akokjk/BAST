#ifndef CARD_HPP
#define CARD_HPP
#include "common.hpp"

class Card{
public: 
	//drawable object
	//create a random card 
	//create a specific card 
	void create(Game& game); 
	//rastering engine for 3d cards 
	

private:
	sf::Vector2f location;
	std::string title, description, logo;
	unsigned int height, width; 
	sf::Font font; 
	unsigned int border_thickness; 
	unsigned int border_radius; 
	unsigned long long id; 
	std::string owner; 
	//edges and surface will be a float value injected into a noise function, that will generate those features which i guess will turned into sprites?  
	float price, centering, cornering, edges, surface; 
	sf::Color outer_border, inner_border, inner_body, center_bg;
	/** basic card features need to be added 
	position 
	grytation //3d rotation 
	weight in grams 
	center graphic 
	title 
	fonts 
	border thickness
	border radius 
	unique id 
	owner 
	current market price 
	owner histography
	perfect version copy  
	outer border color
	inner card color 
	inner border color 
	center grapic color background 

	dispcrition text  
	Bottom Logo
	backgraphic  
	centering 
	cornering 
	edges 
	surface 

	add later: varients and special features and counterfiet 
	*/
}; 
#endif