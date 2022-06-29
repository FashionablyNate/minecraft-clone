#include "../include/window.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>

Window::Window() {

    this->last_frame = glfwGetTime();
    this->last_second = glfwGetTime();
    this->size = glm::vec2(SCR_WIDTH, SCR_HEIGHT);

    /* Initializing GLFW */
    /* ---------------------------------------------------------------------- */

    // attempt to initialize glfw
    if (glfwInit() == GLFW_FALSE) {
        display_glfw_error_message();
        exit(1);
    }

    // specify the version number for opengl, opengl version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // specify the opengl profile, opengl core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Creating the window */
    /* ---------------------------------------------------------------------- */

    // here we create a window object which stores all windowing data
    this->handle = glfwCreateWindow(this->size.x,
                                    this->size.y,
                                    "Minecraft-Clone", // window title
                                    NULL, // NULL specifies windowed mode
                                    NULL);
    
    
    // if the window wasn't created, we print an error and quit
    if (this->handle == NULL) {
        display_glfw_error_message();
        glfwTerminate();
        exit(1);
    }

    // Makes the OpenGL context of the specified window current on the calling thread
    glfwMakeContextCurrent(this->handle);

    glfwSetInputMode(this->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // // register a callback function for when the window is resized
    // glfwSetFramebufferSizeCallback(this->handle, framebuffer_size_callback);
    // // register a callback function for when the mouse is moved
    // glfwSetCursorPosCallback(this->handle, mouseCallback);
    // // register a callback function for when the mouse scrolls
    // glfwSetScrollCallback(this->handle, scrollCallback);
    glfwSetKeyCallback(this->handle, _key_callback);

    /* Initializing GLAD */
    /* ---------------------------------------------------------------------- */

    // query glad for the opengl function pointers, if it fails we print an error
    // and quit.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to intialize GLAD" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwSwapInterval(1);
}

void Window::windowLoop() {
    this->init();

    while (!glfwWindowShouldClose(this->handle)) {
        const uint64_t now = glfwGetTime();

        this->frame_delta = now - this->last_frame;
        this->last_frame = now;

        this->update();
        this->render();
        
        glfwSwapBuffers(this->handle);
        glfwPollEvents();
    }
}

void Window::init() {
    Shader shader("../shaders/shader.vert", "../shaders/shader.frag");
    
}

void Window::update() {
    // TODO
}

void Window::render() {
    glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* -------------------------------------------------------------------------- */
// called upon a key press, triggers camera movement
void _key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) {
    // const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(handle, true);
    
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     cameraPos += cameraSpeed * cameraFront;

    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     cameraPos -= cameraSpeed * cameraFront;

    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    //     cameraPos += cameraSpeed * cameraUp;

    // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    //     cameraPos -= cameraSpeed * cameraUp;
}

/* -------------------------------------------------------------------------- */
// called upon mouse movement, moves camera
void _mouse_callback(GLFWwindow * window, double xpos, double ypos) {
    // if (firstMouse) {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;
    // lastX = xpos;
    // lastY = ypos;

    // const float sensitivity = 0.07f;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // yaw += xoffset;
    // pitch += yoffset;

    // if (pitch > 89.0f) pitch = 89.0f;
    // if (pitch < -89.0f) pitch = -89.0f;
}

/* -------------------------------------------------------------------------- */
// called upon scroll wheel usage, changes fov
void _scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // fov -= (float)yoffset;
    // if (fov < 1.0f)
    //     fov = 1.0f;
    // if (fov > 45.0f)
    //     fov = 45.0f; 
}

/* -------------------------------------------------------------------------- */
// called upon window resize, changes the viewport size
void _framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    
    glViewport(0,       // set the x coord of the lower left corner
               0,       // set the y coord of the lower left corner
               width,   // set the x coord of the upper right corner
               height); // set the y coord of the upper right corner
}

/* -------------------------------------------------------------------------- */
// function for printing out the last glfw error message
void display_glfw_error_message() {

    // create memory space for the description
    const char * description;

    // retrieve the last error thrown, and store it in the description memory address
    int code = glfwGetError(&description);

    // if anything got stored in description, we print it out
    if (description) {
        std::cout << "Error: " << description << std::endl;
    }
}
/* -------------------------------------------------------------------------- */