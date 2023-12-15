#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Function to read points from a text file and store them in a vector
std::vector<glm::vec3> readPointsFromFile(const std::string& filename) {
    std::vector<glm::vec3> points;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return points; // Return an empty vector on failure
    }

    float x, y, z;
    while (file >> x >> y >> z) {
        points.push_back(glm::vec3(x, y, z));
    }

    file.close();
    return points;
}

// Function to calculate points on the Catmull-Rom Spline
std::vector<glm::vec3> calculateCatmullRomSpline(const std::vector<glm::vec3>& controlPoints) {
    std::vector<glm::vec3> splinePoints;

    for (int i = 1; i < controlPoints.size() - 2; ++i) {
        for (float t = 0.0; t <= 1.0; t += 0.01) {
            glm::vec3 point = calculateCatmullRomPoint(t, controlPoints[i - 1], controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]);
            splinePoints.push_back(point);
        }
    }

    return splinePoints;
}

// Function to calculate a point on the Catmull-Rom spline
glm::vec3 calculateCatmullRomPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    float t2 = t * t;
    float t3 = t2 * t;

    float b0 = 0.5 * (-t3 + 2 * t2 - t);
    float b1 = 0.5 * (3 * t3 - 5 * t2 + 2);
    float b2 = 0.5 * (-3 * t3 + 4 * t2 + t);
    float b3 = 0.5 * (t3 - t2);

    return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}

// Function to load a texture from file and bind it to OpenGL
GLuint loadTexture(const std::string& filename) {
    int width, height, channels;
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb);

    if (!image) {
        std::cerr << "Error loading texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

    return textureID;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Catmull-Rom Spline", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set up OpenGL rendering properties
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Read points from file (dummy data for now)
    std::vector<Point> controlPoints = readPointsFromFile("path/to/points.txt");

    // Calculate spline points
    std::vector<Point> splinePoints = calculateCatmullRomSpline(controlPoints, 100);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render spline (dummy rendering for now)
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);

        for (const auto& point : splinePoints) {
            glVertex3f(point.x, point.y, point.z);
        }

        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
