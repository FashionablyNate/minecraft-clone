#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

unsigned int window_width = 800;
unsigned int window_height = 600;

// prints description of last thrown error
void display_glfw_error_message();

// called upon window resize, changes the viewport size
void framebuffer_size_callback(GLFWwindow * window, int width, int height);

// function for taking in user input
void processInput(GLFWwindow *window);

int main() {

    /* Initializing GLFW */
    /* ---------------------------------------------------------------------- */

    // attempt to initialize glfw
    if (glfwInit() == GLFW_FALSE) {
        display_glfw_error_message();
        return -1;
    }

    // specify the version number for opengl, opengl version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // specify the opengl profile, opengl core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Creating the window */
    /* ---------------------------------------------------------------------- */

    // here we create a window object which stores all windowing data
    GLFWwindow * window = glfwCreateWindow(window_width,
                                           window_height,
                                           "Minecraft-Clone", // window title
                                           NULL, // NULL specifies windowed mode
                                           NULL);
    
    // if the window wasn't created, we print an error and quit
    if (window == NULL) {
        display_glfw_error_message();
        glfwTerminate();
        return -1;
    }

    // Makes the OpenGL context of the specified window current on the calling thread
    glfwMakeContextCurrent(window);
    

    /* Initializing GLAD */
    /* ---------------------------------------------------------------------- */

    // query glad for the opengl function pointers, if it fails we print an error
    // and quit.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to intialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Creating the viewport */
    /* ---------------------------------------------------------------------- */

    glViewport(0,               // set the x coord of the lower left corner
               0,               // set the y coord of the lower left corner
               window_width,    // set the x coord of the upper right corner
               window_height);  // set the y coord of the upper right corner

    // register a callback function for when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Vertex Input */
    /* ---------------------------------------------------------------------- */

    // here we specify the normalized device coordinates of a triangle, we keep all
    // the z coordinates on the same plane to generate a 2D image
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // We create a vertex buffer object that allows us to pass large chunks of
    // vertices to the GPU's memory all at once, we just hold onto the ID for it
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // here we bind the vertex buffer object, now all calls we make on
    // GL_ARRAY_BUFFER will configure the VBO until something else is bound
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // here we copy our vertex array into the VBO, GL_STATIC_DRAW specifies that
    // our data is immutable and will be used many times
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Render Loop */
    /* ---------------------------------------------------------------------- */
    
    // checks if window should close at every iteration of the loop
    while(!glfwWindowShouldClose(window)) {

        // check for user input
        processInput(window);

        // set background to a blue-ish color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        
        // clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // swaps the color buffer from the back buffer to the front buffer
        glfwSwapBuffers(window);

        // checks if events have occurred (like window state update, or inputs)
        // and calls the appropriate callback function.
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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
// called upon window resize, changes the viewport size
void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    
    glViewport(0,       // set the x coord of the lower left corner
               0,       // set the y coord of the lower left corner
               width,   // set the x coord of the upper right corner
               height); // set the y coord of the upper right corner
}

/* -------------------------------------------------------------------------- */
// called every frame to check for user input
void processInput(GLFWwindow *window) {
    
    // if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

        // set the window should close flag to true
        glfwSetWindowShouldClose(window, true);
    }
}