#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>
#include <iomanip>
#include <sstream>

std::string GLTranslateError(unsigned int error) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << error;
    std::string hexError(stream.str());
    return hexError;
}

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << GLTranslateError(error) << ") "
            << "on func: " << function
            << " on file: " << file
            << " @line: "
            << line
            << std::endl;
        return false;
    }

    return true;
}
