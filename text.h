#include "shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <glm/glm.hpp>


class text
{
private:

    std::string _content;
    float _x;
    float _y;
    float _scale;
    float _r;
    float _g;
    float _b;

    shader text_shader = shader("text_shader.vs", "text_shader.fs");

    struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Offset to advance to next glyph
	};

    std::map<char, Character> Characters;

    void RenderText(std::string text, float x, float y, std::map<char, Character> Characters, shader text_shader, float scale, float r, float g, float b);

    unsigned int VAO, VBO;


public:
    text(std::string content, float x, float y, float r, float g, float b, float scale);
    void Render();
};




