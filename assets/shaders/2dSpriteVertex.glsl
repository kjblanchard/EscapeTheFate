#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 srcRect;      // x, y, w, h — pixel coordinates
uniform vec2 textureSize;  // texture->Width, texture->Height
                           // uniform vec4 uvRect;  // texture->Width, texture->Height

void main()
{
    vec2 base = vertex.zw;
    // Convert the srcRect from pixel space to normalized texture coordinates
    vec2 uv0 = srcRect.xy / textureSize;
    vec2 uv1 = (srcRect.xy + vec2(srcRect.z, srcRect.w)) / textureSize;
    TexCoords = mix(vec2(uv0.x, uv0.y), vec2(uv1.x, uv1.y), vertex.zw);
    // When using uv rect from CPU, don't use the other
    // TexCoords = mix(uvRect.xy, uvRect.zw, vertex.zw);

    // Regular transform
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}

