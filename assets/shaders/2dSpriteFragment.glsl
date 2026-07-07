#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform vec2 textureSize;

void main()
{
    ivec2 texel = ivec2(TexCoords * textureSize);
    ivec2 minTexel = ivec2(srcRect.xy);
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, minTexel, maxTexel);
    color = spriteColor * texelFetch(image, texel, 0);
}
