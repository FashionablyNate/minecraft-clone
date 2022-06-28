#include <glm/geometric.hpp>
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "../include/shader.hpp"

#include <iostream>

// starting window width and height
const unsigned int window_width = 1920;
const unsigned int window_height = 1080;

glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame  = 0.0f; // time of last frame

float lastX = 960, lastY = 540;
float yaw, pitch, fov = 45.0f;

bool firstMouse = true;

// prints description of last thrown error
void display_glfw_error_message();

// called upon window resize, changes the viewport size
void framebuffer_size_callback(GLFWwindow * window, int width, int height);

// function for taking in user input
void processInput(GLFWwindow *window);

void mouseCallback(GLFWwindow * window, double xpos, double ypos);

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

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

    // glViewport(0,               // set the x coord of the lower left corner
    //            0,               // set the y coord of the lower left corner
    //            window_width,    // set the x coord of the upper right corner
    //            window_height);  // set the y coord of the upper right corner

    // register a callback function for when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetScrollCallback(window, scrollCallback);

    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    /* Vertex Input */
    /* ---------------------------------------------------------------------- */

     // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // here we tell openGL how to interpret the data we give it
    glVertexAttribPointer(
        0,                  // specify which vertex attribute (location 0)
        3,                  // size of vertex attribute (vec3)
        GL_FLOAT,           // type of data (vec* uses floats)
        GL_FALSE,           // whether we want the data normalized (floating point conversion)
        5 * sizeof(float),  // the stride (how long is one vertex in memory)
        (void *)0           // the offset (where to start reading)
    );

    // now we enable the vertex attribute at location 0
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,                          // specify which vertex attribute (location 0)
        2,                          // size of vertex attribute (vec3)
        GL_FLOAT,                   // type of data (vec* uses floats)
        GL_FALSE,                   // whether we want the data normalized (floating point conversion)
        5 * sizeof(float),          // the stride (how long is one vertex in memory)
        (void *)(3 * sizeof(float)) // the offset (where to start reading)
    );

    // now we enable the vertex attribute at location 1
    glEnableVertexAttribArray(1);

    /* Texture Loading */
    /* ---------------------------------------------------------------------- */

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    // load and create a texture 
    unsigned int texture1;

    unsigned char *data = stbi_load("img/blocks.png", &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data) {
        // texture 1
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); 

        // No interpolation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
        glBindTexture(GL_TEXTURE_2D, texture1);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

    /* Camera Setup */
    /* ---------------------------------------------------------------------- */

    glm::mat4 trans = glm::mat4(1.0f);

    // orthographic projection matrix, which defines the clipping space
    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

    // arg1, sets FOV
    // arg2, aspect ratio
    // arg3, near plane of frustum
    // arg4, far plane of frustum
    glm::mat4 projection = glm::perspective((float)(M_PI / 4), (float)width / (float)height, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);

    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glEnable(GL_DEPTH_TEST);

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
  		               glm::vec3(0.0f, 0.0f, 0.0f), 
  		               glm::vec3(0.0f, 1.0f, 0.0f));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    yaw = -90.0f;
    glm::vec3 direction;

    /* Render Loop */
    /* ---------------------------------------------------------------------- */

    // checks if window should close at every iteration of the loop
    while(!glfwWindowShouldClose(window)) {

        // check for user input
        processInput(window);

        direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
        cameraFront = glm::normalize(direction);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        // set background to a blue-ish color
        glClearColor(0.4314f, 0.6941f, 1.0f, 1.0f);
        
        // clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render container
        shader.use();
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);

        trans = glm::rotate(trans, 0.0f, glm::vec3(1.0, 0.0, 0.0));
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        int texOffsetLoc = glGetUniformLocation(shader.ID, "texPos");
        glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(1, 15)));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(1, 15)));
        glDrawArrays(GL_TRIANGLES, 6, 6);

        trans = glm::rotate(trans, (float)(M_PI / 2), glm::vec3(1.0, 0.0, 0.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(1, 15)));
        glDrawArrays(GL_TRIANGLES, 12, 6);

        glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(1, 15)));
        glDrawArrays(GL_TRIANGLES, 18, 6);
        trans = glm::rotate(trans, (float)(-1.0f * M_PI / 2), glm::vec3(1.0, 0.0, 0.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        texOffsetLoc = glGetUniformLocation(shader.ID, "texPos");
        glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(2, 15)));
        glDrawArrays(GL_TRIANGLES, 24, 6);

        texOffsetLoc = glGetUniformLocation(shader.ID, "texPos");
        glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(0, 15)));
        glDrawArrays(GL_TRIANGLES, 30, 6);

        // swaps the color buffer from the back buffer to the front buffer
        glfwSwapBuffers(window);

        // checks if events have occurred (like window state update, or inputs)
        // and calls the appropriate callback function.
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
/* END OF MAIN */


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
    
    const float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

void mouseCallback(GLFWwindow * window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.07f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}