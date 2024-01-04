#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

class text
{
private:
    std::string _content;
    float _x;
    float _y;
    GLFWwindow* _window;
    void RenderText(std::string text, float x, float y);

public:
    text(std::string content, float x, float y, GLFWwindow* window);
    void Render();
};




