#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"


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
    
    // scope limitato
    {
        float trianglePositions[6] = {
             -0.5f,  -0.5,
              0.0f,  0.5f,
              0.5f,  -0.5f
        };

        /* considerato che i vertici sono ripetuti, duplicando le informazioni stiamo buttando via memoria
         * quindi useremo un IndexBuffer
        float squarePositions[]{
             -0.5f,  -0.5f,
              0.5f,  -0.5f,
              0.5f,   0.5f,

              0.5f,   0.5f,
             -0.5f,   0.5f,
             -0.5f,  -0.5f,
        };  */

        float squarePositions[]{
             -0.5f,  -0.5f, // 0
              0.5f,  -0.5f, // 1
              0.5f,   0.5f, // 2
             -0.5f,   0.5f  // 3
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        VertexArray va;
        VertexBuffer vb(squarePositions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

   
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Object.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8, 0.3f, 0.8f, 1.0f);

        // reset 
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float red = 0.0f;
        float increment = 0.02f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (red > 1.0f) {
                increment = -0.02f;
            }
            else if (red < 0.0f) {
                increment = 0.02f;
            }

            red += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}