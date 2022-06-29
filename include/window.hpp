#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdint.h>
#include "./config.hpp"
#include "./shader.hpp"

const unsigned int width = SCR_WIDTH, height = SCR_HEIGHT;

class Window {
public:

    GLFWwindow *handle;
    glm::vec2 size;

    uint64_t last_second;
    uint64_t frames, fps, last_frame, frame_delta;
    uint64_t ticks, tps, tick_remainder;

    unsigned int VAO, texture;

    Window();

    void windowLoop();

    void init();

    void destroy();

    void tick();
    
    void update();

    void render();

};

void _key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods);
void _mouse_callback(GLFWwindow * window, double xpos, double ypos);
void _scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void _framebuffer_size_callback(GLFWwindow * window, int width, int height);

// prints description of last thrown error
void display_glfw_error_message();

// called upon window resize, changes the viewport size

#endif