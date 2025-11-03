#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, TexCoords).r);
    FragColor = vec4(1.0,1.0,1.0, 1.0) * sampled;
}  
