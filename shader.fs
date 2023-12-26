#version 330 core
out vec4 FragColor;  
in vec4 ourColor;
in vec2 TexCoord;
 
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = (ourColor);   
    FragColor = vec4(1.0f,1.0f,1.0f,1.0f) * mix(texture(texture1, TexCoord),texture(texture2,vec2(TexCoord.x,TexCoord.y)),0.3);
}