#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;

void main() {
    vec2 uv = TexCoords;
    vec2 grid = vec2(6.0, 4.0);

    // Which cell does this pixel belong to
    vec2 cell = floor(uv * grid);

    // Per-cell random for staggered timing
    float rand = fract((cell.x * 3.17 + cell.y * 7.23 + cell.x * cell.y * 0.53) * 0.1317);
    float fallStart = 0.1 + rand * 0.4;
    float fallTime = max(time - fallStart, 0.0);
    float fallDist = fallTime * fallTime * 3.0;

    // Slight rotation per shard
    float rotSpeed = (rand - 0.5) * 2.0;
    float angle = fallTime * rotSpeed;

    // Center of this cell in UV space
    vec2 cellCenter = (cell + 0.5) / grid;

    // Rotate around the cell center, then offset by fall
    vec2 offset = uv - cellCenter;
    float cs = cos(angle);
    float sn = sin(angle);
    vec2 rotated = vec2(offset.x * cs - offset.y * sn, offset.x * sn + offset.y * cs);
    vec2 sourceUV = rotated + cellCenter - vec2(0.0, fallDist);

    // If source is off screen, shard is gone
    if (sourceUV.y < 0.0 || sourceUV.y > 1.0 || sourceUV.x < 0.0 || sourceUV.x > 1.0) {
        FragColor = spriteColor * vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3 tex = texture(image, sourceUV).rgb;

    // Edge lines move with the shard (computed from source position)
    vec2 cellUV = fract(sourceUV * grid);
    float edgeX = min(cellUV.x, 1.0 - cellUV.x);
    float edgeY = min(cellUV.y, 1.0 - cellUV.y);
    float crack = 1.0 - smoothstep(0.0, 0.03, min(edgeX, edgeY));

    vec3 finalColor = mix(tex, vec3(1.0), crack * smoothstep(0.0, 0.15, time));

    FragColor = spriteColor * vec4(finalColor, 1.0);
}
