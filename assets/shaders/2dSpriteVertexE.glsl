#version 300 es
precision mediump float;

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 srcRect;      // x, y, w, h — pixel coordinates
uniform vec2 textureSize;  // texture->Width, texture->Height

void main()
{
    vec2 uv0 = srcRect.xy / textureSize;
    vec2 uv1 = (srcRect.xy + srcRect.zw) / textureSize;
    TexCoords = mix(uv0, uv1, vertex.zw);

    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}
