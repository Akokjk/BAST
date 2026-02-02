#include "common.hpp"



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
        else if (type == typeid(sf::VertexArray)) {
            window.draw(std::any_cast<sf::VertexArray&>(item.object));
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
    cardRotation = cardRotation + 10.0f * dt;
    // 2. Keep the angle within 0-360 range for precision
    if (cardRotation >= 360.0f) {
        cardRotation -= 360.0f;
    }
    fTheta = cardRotation ;
    

    mesh card_plane; 
    float offset = 10.0f; 
    card_plane.tris = {
        
        {0.f + offset, 0.f + offset, 0.f, 
        100.f + offset, 0.f + offset, 0.f, 
        0.f + offset,  200.0f + offset, 0.f }, 


        {100.f + offset, 0.f + offset, 0.f, 
        100.f + offset, 200.0f + offset, 0.f,  
        0.f + offset, 200.0f + offset, 0.f }, 


    };

    float hw = width / 2.0f;
    float hh = height / 2.0f;

    card_plane.tris = {
        // Triangle 1
        {-hw, -hh, 0.f, hw, -hh, 0.f, -hw, hh, 0.f},
        // Triangle 2
        {hw, -hh, 0.f, hw, hh, 0.f, -hw, hh, 0.f }
    };

    float fNear = 0.1f; 
    float fFar = 1000.f;
    float fFOV = 90; 
    float fAspectRatio = static_cast<float>(height) / static_cast<float>(width);
    float fFovRad = 1.0f / tan(fFOV * 0.5f / 180.0f * PI); 

    mat4x4 matProjection; 
    for(int i=0; i<4; i++) for(int j=0; j<4; j++) matProjection.m[i][j] = 0.0f;

    matProjection.m[0][0] = fAspectRatio * fFovRad; 
    matProjection.m[1][1] = fFovRad; 
    matProjection.m[2][2] = fFar / (fFar - fNear); 
    matProjection.m[3][2] = (-fFar * fNear) / (fFar - fNear); 
    matProjection.m[2][3] = 1.0f; 
    matProjection.m[3][3] = 0.0f; 

    // 2. Define the Y-axis for "up and down" rotation
    vec3d yAxis = {0.0f, 1.0f, 0.0f}; 
    Quaternion q = Quaternion::fromAxisAngle(yAxis,fTheta);
    mat4x4 matRot = QuaternionToMatrix(q);

    int place = 0; 
    for(auto& tri : card_plane.tris) {
        triangle projected, rotated;
        for(int i = 0; i < 3; i++) {
            // STEP A: Rotate around the center (0,0,0)
            MultiplyMatrixVector(tri.p[i], rotated.p[i], matRot);

            // STEP B: Move it into the screen AND center it in the view
            // 3.0f is very close; try 400.0f or similar if using pixel-sized mesh
            rotated.p[i].z += 500.0f; 

            // STEP C: Project to 2D
            MultiplyMatrixVector(rotated.p[i], projected.p[i], matProjection);

            // STEP D: Scale to Screen Space (CRITICAL)
            projected.p[i].x += 1.0f; 
            projected.p[i].y += 1.0f;
            projected.p[i].x *= 0.5f * (float)width;
            projected.p[i].y *= 0.5f * (float)height;
        }

        sf::Vertex v1{.position = {projected.p[0].x, projected.p[0].y}, .color = sf::Color::Red};
        sf::Vertex v2{.position = {projected.p[1].x, projected.p[1].y}, .color = sf::Color::Green};
        sf::Vertex v3{.position = {projected.p[2].x, projected.p[2].y}, .color = sf::Color::Blue};

        sf::VertexArray va(sf::PrimitiveType::Triangles); 
        va.append(v1); va.append(v2); va.append(v3);
        
        push("3dcard" + std::to_string(place++), va, State::Buy);
    }
    // 2. Example: Logic based on Data
    // If you stored a "isHovered" boolean for a card:
    // bool hovered = getData<bool>("Card_Hovered");
    // if (hovered) { ... scale up the sprite ... }
    //push("dtCheck", sf::Text(roboto_font, std::to_string(fTheta), 30), State::Buy);
}

void Game::events(){
    while (const std::optional event = window.pollEvent()) {
            //Global Events 
            if (event->template is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->template getIf<sf::Event::KeyPressed>()) {
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