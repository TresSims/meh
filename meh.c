#include <GLFW/glfw3.h>

int main(int argc, char **argv){
    if(!glfwInit()){

    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "meh", NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
    }

    glfwTerminate();
}
