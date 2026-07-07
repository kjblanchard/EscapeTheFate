#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;

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
    FragColor = spriteColor * texelFetch(image, texel, 0);
}
