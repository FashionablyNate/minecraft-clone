#include "../include/shader.hpp"
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;

// constructor, takes in file paths to shader source code files, then compiles
// them, puts them in a program, and links that program
Shader::Shader(const char * vertexPath, const char * fragmentPath) {

    // we call a function to read the shader files, and then we store
    // the source code returned for later
    string vShaderString = filePathToString(vertexPath);
    string fShaderString = filePathToString(fragmentPath);

    // we convert the strings into const char *'s for the glShaderSource function
    const char * vShaderCode = vShaderString.c_str();
    const char * fShaderCode = fShaderString.c_str();

    // we create a vertex shader object and store it's reference ID
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    // we load the source code into the object, compile it, and check for errors
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    if (!errorCheck(vertex)) exit(1);

    // we create a fragment shader object and store it's reference ID
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    // we load the source code into the object, compile it, and check for errors
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    if (!errorCheck(fragment)) exit(1);

    // we create a program object and sotre it's reference ID
    unsigned int ID = glCreateProgram();
    // we attach our compiled shaders to the program
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    // we link the program and check for errors
    glLinkProgram(ID);
    if (!errorCheck(ID, true)) exit(1);

    // we call the program into use once before deleting the shaders (or else the
    // fragment shader doesn't work)
    glUseProgram(ID);

    // we delete the shaders to free memory
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// use/activate the shader program
void Shader::use() {
    glUseProgram(ID);
}

// takes an ID for openGL object in, and has a flag that should be set to true
// when error checking after linking. Prints out the error log after compilation/
// linking and returns false upon error. Otherwise outputs nothing and returns
// true.
bool Shader::errorCheck(unsigned int ID, bool link) {
    
    // success indicates if compilation/linking failed
    int success;
    // infoLog stores the error message after we retrieve it from the log
    char infoLog[512];

    // if we're linking we use a different macro for retrieving the log
    (link) ? glGetProgramiv(ID, GL_LINK_STATUS, &success) :
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    
    // if compilation/linking failed
    if (!success) {
        
        // these are the two functions for getting the log when linking or compiling
        (link) ? glGetProgramInfoLog(ID, 512, NULL, infoLog) :
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
        
        // we print a message that gives context to what process failed
        (link) ? cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl :
            cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
        
        // return false so we can terminate the program
        return false;
    }

    // compilation/linking was successful
    return true;
}

// takes in a file path, opens the file (if it exists), reads it into a string,
// and returns that string
string Shader::filePathToString(const char * path) {

    // shaderCode will be the return value
    string shaderCode;
    // shaderFile will hold the input string from the file path
    ifstream shaderFile;

    // we ensure we can throw exceptions
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try {

        // open the file
        shaderFile.open(path);
        stringstream shaderStream;

        // stream the file into the shaderStream object
        shaderStream << shaderFile.rdbuf();

        // close the file
        shaderFile.close();

        // convert the stream to a string and store it in shaderCode
        shaderCode = shaderStream.str();

    } catch (ifstream::failure e) {

        // if file I/O fails we print this message
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    // we return the string
    return shaderCode;
}