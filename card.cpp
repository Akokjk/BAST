#include "common.hpp"

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

// Helper to wrap text (reusing previous logic)
std::string wrapText(const std::string& str, const sf::Font& font, unsigned int size, float maxWidth) {
    std::string wrapped, line, word;
    std::stringstream ss(str);
    while (ss >> word) {
        sf::Text temp(font, line + (line.empty() ? "" : " ") + word, size);
        if (temp.getLocalBounds().size.x > maxWidth) {
            wrapped += line + "\n";
            line = word;
        } else {
            line += (line.empty() ? "" : " ") + word;
        }
    }
    return wrapped + line;
}

std::pair<unsigned int, std::string> getBestFit(const std::string& rawText, const sf::Font& font, sf::Vector2f boxSize) {
    unsigned int bestSize = 10;
    unsigned int minSize = 10;
    unsigned int maxSize = 60; // Lowered this; 100 is massive for a card desc
    std::string bestString = rawText;

    while (minSize <= maxSize) {
        unsigned int midSize = minSize + (maxSize - minSize) / 2;
        std::string wrapped = wrapText(rawText, font, midSize, boxSize.x);
        
        // Count the lines in the wrapped string
        size_t lines = std::count(wrapped.begin(), wrapped.end(), '\n') + 1;
        
        // Calculate the actual vertical space the lines will take
        // getLineSpacing returns the distance between baseline of two lines
        float totalHeight = lines * font.getLineSpacing(midSize);

        if (totalHeight <= boxSize.y) {
            bestSize = midSize;
            bestString = wrapped;
            minSize = midSize + 1;
        } else {
            maxSize = midSize - 1;
        }
    }
    return {bestSize, bestString};
}

void Card::create(Game& game){
	location = sf::Vector2f({100.f, 20.f});
	title = "Secret Dev Card";
	description = "This is the very first card in the game I hope you like it. -A";
	description = "This is testing a really long card description to make sure it fits in the given box. I hope it does! Hows the weather by chance. 1+1 = fish This is testing a really long card description to make sure it fits in the given box. I hope it does! Hows the weather by chance. 1+1 = fish This is testing a really long card description to make sure it fits in the given box. I hope it does! Hows the weather by chance. 1+1 = fish This is testing a really long card description to make sure it fits in the given box. I hope it does! Hows the weather by chance. 1+1 = fish";
	//description = "I Lug you, Gooby!";
	logo = "BAST";
	height = 440; //card have a 1.4:1 ration apprently?!
	width = 314; 
	font = game.roboto_font; 
	border_thickness = 5; 
	id = 1; 
	owner = "Aaron"; 
	price = centering = cornering = edges = surface = 0.f; 
	outer_border = sf::Color({178, 98, 38,255}); 
	inner_border = sf::Color({40,0,40,255});
	inner_body = sf::Color({244, 239, 219,255});
	center_bg =   sf::Color({255,255,255,255});

	sf::RectangleShape card; 
	card.setSize(sf::Vector2f(width, height)); 
	card.setOutlineColor(outer_border); 
	card.setFillColor(inner_body); 
	card.setOutlineThickness(border_thickness); 
	card.setPosition(location);

	sf::RectangleShape inner_picture; 
	inner_picture.setFillColor(center_bg); 
	inner_picture.setOutlineThickness(1); //need variable 
	inner_picture.setOutlineColor(sf::Color::Black); 
	inner_picture.setSize(sf::Vector2f(250, 150)); //i guess it should be a ratio needs a variable
	inner_picture.setPosition(sf::Vector2f({location.x + (width - 250)/2, location.y + 40.f}));

	//title 
	sf::Text header(game.roboto_font, title); 
	header.setCharacterSize(30); //need a variable
	header.setPosition(sf::Vector2f({location.x + (width - 250)/2, location.y}));  //the 5 would be the gap from the inner picture /2 so its 40px 30px so 10px /2 
	header.setFillColor(sf::Color::Black); 



	//description box and text, which uses helper function 
	sf::RectangleShape desc_box; 
	desc_box.setFillColor(sf::Color::Black); 
	desc_box.setSize(sf::Vector2f({250, 200})); 
	desc_box.setPosition(sf::Vector2f({location.x + (width - 250)/2, location.y+200.f})); 

	sf::Vector2f textArea = desc_box.getSize() - sf::Vector2f({10.f, 10.f});
	std::pair<unsigned int, std::string> result = getBestFit(description, game.roboto_font, textArea); 
	sf::Text desc_text(game.roboto_font, result.second); 
	desc_text.setCharacterSize(result.first);
	desc_text.setFillColor(sf::Color::Black); 
	desc_text.setPosition(desc_box.getPosition());  

	//bottom text logo 
	sf::Text btm_logo(game.roboto_font, logo); 
	btm_logo.setFillColor(sf::Color::Red); 
	btm_logo.setCharacterSize(40); 
	btm_logo.setLetterSpacing(10); 
	btm_logo.setOrigin({btm_logo.getLocalBounds().size.x / 2.f, btm_logo.getLocalBounds().size.y / 2.f});
	btm_logo.setPosition(sf::Vector2f({location.x + width/2, location.y + 400.f})); 

	//make a function to push all these 
	game.push("Dev_Card", card, State::Buy);
	game.push("Dev_Card_Inner_Picture", inner_picture, State::Buy); 
	game.push("Dev_Card_Header", header, State::Buy); 
	//game.push("Hornk_u_mother_trucker", desc_box, State::Buy); 
	game.push("Hornk_u_mother_trucker1", desc_text, State::Buy); 
	game.push("Hornk_u_mother_trucker2", btm_logo, State::Buy); 




}