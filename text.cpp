#include "text.h"

text::text(std::string content, float x, float y, GLFWwindow* window)
    : _content(content), _x(x), _y(y), _window(window)
{
    FT_Library library;

    if (FT_Init_FreeType(&library))
    {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

}

void text::Render()
{
    RenderText(_content, _x, _y);
}

void text::RenderText(std::string text, float x, float y)
{

    // Render text to window using FontType


    // Print text to console
    std::cout << text << std::endl;
}
