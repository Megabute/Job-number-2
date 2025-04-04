#define GLEW_DLL
#define GLFW_DLL

#include "Gl\glew.h"
#include "GLFW/glfw3.h"
#include <cmath>

GLfloat points[] =
{ 0.0, 0.5, 0.0,
-0.5, -0.5, 0.0,
0.5, -0.5, 0.0 };

GLuint index[] = { 0, 1, 2 };

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window;
    window = glfwCreateWindow(512, 512, "NOT_HEX", NULL, NULL);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char* vert_shader =
        "#version 410 core\n"
        "layout(location = 0) in vec3 vp;"
        "void main() {"
        "gl_Position = vec4(vp, 1.0);"
        "}";

    const char* frag_shader =
        "#version 410 core\n"
        "uniform float time;"
        "out vec4 Color;"
        "void main() {"
        "float red = (sin(time) + 1.0) / 2.0;"
        "float green = (cos(time) + 1.0) / 2.0;"
        "Color = vec4(red, green, 0.5, 1.0);"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vert_shader, NULL);
    glShaderSource(fs, 1, &frag_shader, NULL);

    glCompileShader(vs);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5, 0.2, 0.7, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        glUseProgram(shader_program);
        glUniform1f(glGetUniformLocation(shader_program, "time"), timeValue);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}