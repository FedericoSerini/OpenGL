#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream stringStream[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) { // Se questa stringa non viene trovata allora line.find restituisce npos (no position)
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } 
        else {
            stringStream[(int)type] << line << '\n';
        }
    }

    return { stringStream[0].str(), stringStream[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
 
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram(); 
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // i dati relativi a vs e fs sono stati memorizzati in 'program', quindi dereferenziamo
    glDeleteShader(vs); 
    glDeleteShader(fs);
    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1; 

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
   
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cout << glewGetErrorString(err) << std::endl;
    }
        
    std::cout << glGetString(GL_VERSION) << std::endl;

   float trianglePositions[6] = {
        -0.5f,  -0.5,
         0.0f,  0.5f,
         0.5f,  -0.5f
     };

     unsigned int buffer;
     glGenBuffers(1, &buffer); // vertex buffer
     glBindBuffer(GL_ARRAY_BUFFER, buffer); // Collego il vertex buffer al contesto 

     // Dichiaro quali sono i dati che il buffer utilizzerà
     glBufferData(GL_ARRAY_BUFFER, // un buffer di vertex
         6 * sizeof(float),  // 6 posizioni di tipo float
         trianglePositions, 
         GL_STATIC_DRAW); 

     glEnableVertexAttribArray(0); // 

     // questo indica alla GPU come interpretare i dati
     glVertexAttribPointer(0, // componente 0 
         2, // abbiamo un triangolo 2d
         GL_FLOAT, // i dati sono di tipo float
         GL_FALSE, // non normalizzati
         sizeof(float) * 2, // stride di tipo * immagine 2d, quantità di dati riservata tra un vertex e l'altro
         0);
   

     glBindBuffer(GL_ARRAY_BUFFER, 0);

     ShaderProgramSource source = ParseShader("res/shaders/Object.shader");
   
     unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
     glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        // disegno i vertici prendendoli da un vertex buffer
        glDrawArrays(GL_TRIANGLES, // di tipo triangolare
            0,
            3); // con 3 vertici distinti

        /* Legacy Triangle 
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd(); */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}