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