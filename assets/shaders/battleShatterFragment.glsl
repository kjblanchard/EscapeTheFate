#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;

vec2 hash2(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453);
}

void main() {
    vec2 uv = TexCoords;

    // Sample the FBO texture directly
    vec3 col = texture(image, uv).rgb;

    // Voronoi cracks
    vec2 scaledUV = uv * 8.0;
    vec2 cellID = floor(scaledUV);
    vec2 cellUV = fract(scaledUV);
    float minDist = 10.0;
    float secondDist = 10.0;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 neighbor = vec2(float(x), float(y));
            vec2 point = hash2(cellID + neighbor);
            vec2 diff = neighbor + point - cellUV;
            float d = length(diff);
            if (d < minDist) {
                secondDist = minDist;
                minDist = d;
            } else if (d < secondDist) {
                secondDist = d;
            }
        }
    }
    float edge = secondDist - minDist;
    float crackLine = smoothstep(0.05, 0.0, edge);

    // White crack lines fade in over the texture
    vec3 finalColor = mix(col, vec3(1.0), crackLine * smoothstep(0.0, 0.3, time));

    color = spriteColor * vec4(finalColor, 1.0);
}
