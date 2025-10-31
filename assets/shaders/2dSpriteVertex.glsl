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





// Old, where it does not use texture coordinates
// #version 330 core
// layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

// out vec2 TexCoords;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// void main()
// {
//     TexCoords = vertex.zw;
//     gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
// }

// testing
    // gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    // gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    // gl_Position = vec4(vertex.xy, 0.0, 1.0);


// #version 330 core
// layout(location = 0) in vec4 vertex;

// void main() {
//     // Map 0..1 quad coordinates to normalized device coordinates (-1..1)
//     gl_Position = vec4(vertex.xy * 2.0 - 1.0, 0.0, 1.0);
// }

// #version 330 core
// layout(location = 0) in vec4 vertex;
// uniform mat4 projection;

// void main() {
//     // Apply only projection
//     gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
// }

