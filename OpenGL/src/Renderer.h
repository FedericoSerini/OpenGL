#pragma once

#include <iostream>
#include <GL/glew.h>


#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

std::string GLTranslateError(unsigned int error);

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);
