#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float opacity;
void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(1.0 -TexCoord.x, TexCoord.y)), opacity);
}
