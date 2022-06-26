#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // Program ID
    unsigned int ID;

    // constructor, takes in file paths to shader source code files, then
    // compiles them, puts them in a program, and links that program
    Shader(const char * vertexPath, const char * fragmentPath);

    // use/activate the shader program
    void use();

    // utility functions for manipulating uniforms
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, bool value) const;
    void setFloat(const std::string &name, bool value) const;

private:

    // takes an ID for openGL object in, and has a flag that should be set to true
    // when error checking after linking. Prints out the error log after compilation/
    // linking and returns false upon error. Otherwise outputs nothing and returns
    // true.
    bool errorCheck(unsigned int ID, bool link = false);

    // takes in a file path, opens the file (if it exists), reads it into a string,
    // and returns that string
    std::string filePathToString(const char * path);
};

#endif