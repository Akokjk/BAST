#include "common.hpp"



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


struct vec3d {
		float x,y,z;
	};

struct triangle {
	vec3d p[3]; 
};

struct mesh{
	std::vector<triangle> tris; 
};

struct mat4x4{
	float m[4][4] = {0};  
};

void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m){
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z* m.m[2][0] + m.m[3][0]; 
	o.y = i.y * m.m[0][1] + i.y * m.m[1][1] + i.z* m.m[2][1] + m.m[3][1]; 
	o.z = i.z * m.m[0][2] + i.y * m.m[1][2] + i.z* m.m[2][2] + m.m[3][2]; 
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z* m.m[2][3] + m.m[3][3];

	if (w != 0.0f){
		o.x /= w; o.y /= w; o.z /= w; 
	}
}

struct Quaternion {
    float w, x, y, z;

    // Create from Axis-Angle (The intuitive way)
    static Quaternion fromAxisAngle(vec3d axis, float angleDegrees) {
        float angleRad = angleDegrees * 3.14159f / 180.0f;
        float s = sinf(angleRad / 2.0f);
        return { cosf(angleRad / 2.0f), axis.x * s, axis.y * s, axis.z * s };
    }

    // Standard Quaternion Multiplication (To combine rotations)
    Quaternion operator*(const Quaternion& q) const {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        };
    }
};

mat4x4 QuaternionToMatrix(Quaternion q) {
    mat4x4 m;
    // Standard formula to convert Quat to Rotation Matrix
    m.m[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    m.m[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
    m.m[0][2] = 2.0f * (q.x * q.z - q.w * q.y);

    m.m[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
    m.m[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    m.m[1][2] = 2.0f * (q.y * q.z + q.w * q.x);

    m.m[2][0] = 2.0f * (q.x * q.z + q.w * q.y);
    m.m[2][1] = 2.0f * (q.y * q.z - q.w * q.x);
    m.m[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);

    m.m[3][3] = 1.0f;
    return m;
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
	sf::Texture inner_texture; 
	game.loadTexture("card_texture", "resources/card_draft.png");
	sf::Sprite card_pic(*game.textures["card_texture"]);
	//scale card to fit inner_picture rect 
	sf::Vector2u texSize = game.textures["card_texture"]->getSize();
    sf::Vector2f targetSize = inner_picture.getSize();
    float scaleX = targetSize.x / static_cast<float>(texSize.x);
    float scaleY = targetSize.y / static_cast<float>(texSize.y);

    card_pic.setScale({scaleX, scaleY});
	card_pic.setPosition(inner_picture.getPosition()); 

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




	mesh card_plane; 
	float offset = 10.0f; 
	card_plane.tris = {
		
		{0.f + offset, 0.f + offset, 0.f, 
		card.getSize().x + offset, 0.f + offset, 0.f, 
		0.f + offset,  card.getSize().y + offset, 0.f }, 


		{card.getSize().x + offset, 0.f + offset, 0.f, 
		card.getSize().x + offset, card.getSize().y + offset, 0.f,  
		0.f + offset, card.getSize().y + offset, 0.f }, 


	};

	// float hw = game.width / 2.0f;
	// float hh = game.height / 2.0f;

	// card_plane.tris = {
	//     // Triangle 1
	//     {-hw, -hh, 0.f, hw, -hh, 0.f, -hw, hh, 0.f},
	//     // Triangle 2
	//     {hw, -hh, 0.f, hw, hh, 0.f, -hw, hh, 0.f }
	// };

	float fNear = 0.1f; 
	float fFar = 1000.f;
	float fFOV = 90; 
	float fAspectRatio = static_cast<float>(game.height) / static_cast<float>(game.width);
	float fFovRad = 1.0f / tan(fFOV * 0.5f / 180.0f * PI); 

	mat4x4 matProjection; 
   for(int i=0; i<4; i++) for(int j=0; j<4; j++) matProjection.m[i][j] = 0.0f;

	matProjection.m[0][0] = fAspectRatio * fFovRad; 
	matProjection.m[1][1] = fFovRad; 
	matProjection.m[2][2] = fFar / (fFar - fNear); 
	matProjection.m[3][2] = (-fFar * fNear) / (fFar - fNear); 
	matProjection.m[2][3] = 1.0f; 
	matProjection.m[3][3] = 0.0f; 

	// 1. Define an axis (e.g., a diagonal flip)
	vec3d diagonalAxis = {1.f, 0.0f, 0.0f}; 

	// 2. Create the quaternion using your game's theta/timer
	Quaternion q = Quaternion::fromAxisAngle(diagonalAxis, game.fTheta);

	// 3. Convert to your existing matrix type
	mat4x4 matRot = QuaternionToMatrix(q);

	int place = 0; 
	for(auto tri: card_plane.tris){
		triangle projected, rotate ; 
		std::string spot = "3dcard" + std::to_string(place); 
		
	   
	    for(int i=0; i<3; i++) { rotate.p[i].z += 600.0f; } // Adjust 600 to zoom in/out
	    // 3. PROJECT LAST

	    rotate = tri;
		rotate.p[0].z = tri.p[0].z + 3.0f; 
		rotate.p[1].z = tri.p[1].z + 3.0f; 
		rotate.p[2].z = tri.p[2].z + 3.0f; 
		MultiplyMatrixVector(tri.p[0], rotate.p[0], matRot);
		MultiplyMatrixVector(tri.p[1], rotate.p[1], matRot);
		MultiplyMatrixVector(tri.p[2], rotate.p[2], matRot);
	    MultiplyMatrixVector(rotate.p[0], projected.p[0], matProjection);
	    MultiplyMatrixVector(rotate.p[1], projected.p[1], matProjection);
	    MultiplyMatrixVector(rotate.p[2], projected.p[2], matProjection);

	    // // 4. SCALE TO SCREEN SPACE
	    // for (int i = 0; i < 3; i++) {
	    //     // Move from [-1, 1] to [0, 1]
	    //     projected.p[i].x += 1.0f; 
	    //     projected.p[i].y += 1.0f;

	    //     // Scale to pixels
	    //     projected.p[i].x *= 0.5f * (float)game.width;
	    //     projected.p[i].y *= 0.5f * (float)game.height;
	    // }

		// SFML 3 Designated Initializers (Cleanest way)
	    sf::Vertex v1{.position = {projected.p[0].x, projected.p[0].y}, .color = sf::Color::Red};
	    sf::Vertex v2{.position = {projected.p[1].x, projected.p[1].y}, .color = sf::Color::Green};
	    sf::Vertex v3{.position = {projected.p[2].x, projected.p[2].y}, .color = sf::Color::Blue};
	    sf::VertexArray va(sf::PrimitiveType::TriangleStrip  );	
	    // std::cout << "Projected cords X: " << projected.p[0].x << " , Y: " << projected.p[0].y << std::endl; 
	    // std::cout << "Projected cords X: " << projected.p[1].x << " , Y: " << projected.p[1].y << std::endl; 
	    // std::cout << "Projected cords X: " << projected.p[2].x << " , Y: " << projected.p[2].y << std::endl; 
	    va.append(v1);
	    va.append(v2);
	    va.append(v3);
    	game.push(spot, va, State::Buy);
    	place += 1; 
	}
	game.push("ThetaCheck", sf::Text(game.roboto_font, std::to_string(game.fTheta), 30), State::Buy);
	
	// 3. Create SFML Vertices
   

	//make a function to push all these 
	// game.push("Dev_Card", card, State::Buy);
	// game.push("Dev_Card_Inner_Picture", inner_picture, State::Buy); 
	// game.push("Dev_Card_Header", header, State::Buy); 
	// //game.push("Hornk_u_mother_trucker", desc_box, State::Buy); 
	// game.push("Hornk_u_mother_trucker1", desc_text, State::Buy); 
	// game.push("Hornk_u_mother_trucker2", btm_logo, State::Buy); 
	// game.push("Hornk_u_mother_trucker3", card_pic, State::Buy); 



}