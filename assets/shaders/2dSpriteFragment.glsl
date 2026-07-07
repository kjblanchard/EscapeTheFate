#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec2 textureSize;

void main()
{
    ivec2 texel = ivec2(TexCoords * textureSize);
    color = spriteColor * texelFetch(image, texel, 0);
}
