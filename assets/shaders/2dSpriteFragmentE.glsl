#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec2 textureSize;

void main()
{
    ivec2 texel = ivec2(TexCoords * textureSize);
    FragColor = spriteColor * texelFetch(image, texel, 0);
}
