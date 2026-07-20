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

    // Grid: 6 columns, 4 rows
    vec2 cell = floor(uv * vec2(6.0, 4.0));

    // Per-cell random delay using simple math (no trig)
    float rand = fract((cell.x * 3.0 + cell.y * 7.0 + cell.x * cell.y * 0.5) * 0.1317);
    float fallStart = 0.15 + rand * 0.35;
    float fallTime = max(time - fallStart, 0.0);
    float fallDist = fallTime * fallTime * 2.5;

    // Sample the texture offset by the fall distance
    vec2 sourceUV = vec2(uv.x, uv.y - fallDist);

    // If source went above the screen, this shard is gone
    if (sourceUV.y < 0.0) {
        FragColor = spriteColor * vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        vec4 tex = texture(image, sourceUV);

        // Grid edge lines (cracks)
        vec2 cellUV = fract(uv * vec2(6.0, 4.0));
        float edgeX = min(cellUV.x, 1.0 - cellUV.x);
        float edgeY = min(cellUV.y, 1.0 - cellUV.y);
        float crack = 1.0 - smoothstep(0.0, 0.02, min(edgeX, edgeY));
        vec3 finalColor = mix(tex.rgb, vec3(1.0), crack * smoothstep(0.0, 0.2, time));

        FragColor = spriteColor * vec4(finalColor, tex.a);
    }
}
