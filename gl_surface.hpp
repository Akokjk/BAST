#ifndef GL_SURFACE_HPP
#define GL_SURFACE_HPP

#include "common.hpp"


class GLSurface {
public:
    std::vector<float> generatePrismData(float dx, float dy, float dz) {
        float x = dx / 2.0f;
        float y = dy / 2.0f;
        float z = dz / 2.0f;

        // Setup random number generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(50, 255); // Keep colors somewhat bright

        // 8 Unique Corners
        struct Pos { float x, y, z; };
        Pos p[8] = {
            {-x, -y,  z}, { x, -y,  z}, { x,  y,  z}, {-x,  y,  z}, // Front (0,1,2,3)
            {-x, -y, -z}, { x, -y, -z}, { x,  y, -z}, {-x,  y, -z}  // Back  (4,5,6,7)
        };

        // Faces defined by vertex indices (2 triangles per face)
        std::vector<std::vector<int>> faces = {
            {0, 1, 2, 2, 3, 0}, // Front
            {1, 5, 6, 6, 2, 1}, // Right
            {7, 6, 5, 5, 4, 7}, // Back
            {4, 0, 3, 3, 7, 4}, // Left
            {4, 5, 1, 1, 0, 4}, // Bottom
            {3, 2, 6, 6, 7, 3}  // Top
        };

        std::vector<float> data;
        data.reserve(252); // 36 vertices * 7 components

        for (const auto& faceIndices : faces) {
            // Generate one random color per face
            float r = dis(gen) / 255.0f;
            float g = dis(gen) / 255.0f;
            float b = dis(gen) / 255.0f;

            for (int idx : faceIndices) {
                // Position
                data.push_back(p[idx].x);
                data.push_back(p[idx].y);
                data.push_back(p[idx].z);
                // Color
                data.push_back(r);
                data.push_back(g);
                data.push_back(b);
                data.push_back(1.0f); // Alpha
            }
        }
        return data;
    }
    ~GLSurface();
    bool create(unsigned int width, unsigned int height);
    void renderScene(float elapsedSeconds);
    const sf::Texture& getTexture() const;

private:
    sf::RenderTexture m_renderTexture;
    unsigned int m_width = 0;   // Store width
    unsigned int m_height = 0;  // Store height
    unsigned int m_shaderProgram = 0;
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;


   std::vector<float> m_cubeData;
    

    void initGfx(); // To set up Shaders and Buffers
};

#endif