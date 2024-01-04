#include "text.h"


text::text(std::string content, float x, float y, float r, float g, float b, float scale )
    : _content(content), _x(x), _y(y), _scale(scale), _r(r), _g(g), _b(b)
{
    FT_Library library;

    // Initialize FreeType
    if (FT_Init_FreeType(&library))
    {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

    // Load the font
    FT_Face face;
    if (FT_New_Face(library, "Roboto-Black.ttf", 0, &face))
    {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

    // Set the font's width and height (0 means dynamic to height)
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Try and load glyph
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

    }

    // Create a map to hold all the characters
    Characters;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    // Load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    // Add to make sure there are no alignment issues
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Clear FreeTypes resources
    FT_Done_Face(face);
    FT_Done_FreeType(library);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(600), 0.0f, static_cast<float>(800));
    //glm::mat4 projection = glm::perspective((float)glm::radians(45), 800.0f / 600.0f, 0.1f, 1000.0f);
    text_shader.use();
    text_shader.setMat4("projection", projection);

    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void text::Render()
{
    // Add Blending
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RenderText(_content, _x, _y, Characters, text_shader, _scale, _r, _g, _b);

    // Add Blending
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void text::RenderText(std::string text, float x, float y, std::map<char, Character> Characters, shader text_shader, float scale, float r, float g, float b)
{

    text_shader.use();

    glUniform3f(glGetUniformLocation(text_shader.ID, "textColor"), r, g, b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x;
		float ypos = y - (ch.Size.y - ch.Bearing.y);

		float w = ch.Size.x;
		float h = ch.Size.y;

		// Update VBO for each character
        float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

