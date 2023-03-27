#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    vec4 texColor = texture(texture0, TexCoord);
    float monoColor = (texColor.r + texColor.g + texColor.b) / 3.0;
    FragColor = vec4(monoColor, monoColor, monoColor, texColor.a);
}