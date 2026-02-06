#include "common.hpp"

bool GLSurface::create(unsigned int width, unsigned int height) {
    m_cubeData = generatePrismData(100.0f, 150.0f, 5.0f);
    m_width = width;
    m_height = height;

    // Request a depth buffer specifically for the RenderTexture
    sf::ContextSettings settings;
    settings.depthBits = 24;

    if (!m_renderTexture.resize({width, height}, settings)) return false;

    if (m_renderTexture.setActive(true)) {
        if (!gladLoadGL()) {
            std::cerr << "GLAD failed to load" << std::endl;
            m_cubeData = generatePrismData(100.0f, 150.0f, 5.0f);
            return false;
        }

        // Global OpenGL Settings (from your example)
        glClearDepth(1.f);
        glClearColor(0.f, 0.f, 0.f, 1.f);

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);   // Ensure pixels are drawn based on distance
        glEnable(GL_CULL_FACE);    // Enable the culling logic
        glCullFace(GL_BACK);       // Tell OpenGL to hide the back faces
        glFrontFace(GL_CCW);       // Counter-Clockwise vertices are "front"
        //glDisable(GL_LIGHTING);
        //glDisable(GL_TEXTURE_2D);
    }
    
    return true;
}

void GLSurface::renderScene(float elapsedSeconds) {
    if (m_renderTexture.setActive(true)) {
        // Ensure depth and culling are active every frame
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        // Configure Viewport and Projection for the Texture size
        glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = static_cast<float>(m_width) / static_cast<float>(m_height);
        glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Client State setup (Pointer to our array)
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), m_cubeData.data());
        glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), m_cubeData.data() + 3);

        // Apply Transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        //glRotatef(elapsedSeconds * 50.f, 1.f, 0.f, 0.f); //x
        glRotatef(elapsedSeconds * 30.f, 0.f, 1.f, 0.f); //y
        //glRotatef(elapsedSeconds * 90.f, 0.f, 0.f, 1.f); //x

        // Draw the Cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Clean up state
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        m_renderTexture.display();
    }
}

const sf::Texture& GLSurface::getTexture() const {
    return m_renderTexture.getTexture();
}
GLSurface::~GLSurface() {
    // We don't have shaders or buffers to delete anymore, 
    // but the linker still needs to find this function.
}