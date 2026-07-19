#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
uniform vec3 tintColor;

vec2 hash2(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453);
}

float hash1(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main() {
    vec2 uv = TexCoords;

    float cellScale = 10.0;
    vec2 scaledUV = uv * cellScale;
    vec2 cellID = floor(scaledUV);
    vec2 cellUV = fract(scaledUV);

    float minDist = 10.0;
    float secondDist = 10.0;
    vec2 nearestCell = cellID;

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 neighbor = vec2(float(x), float(y));
            vec2 point = hash2(cellID + neighbor);
            vec2 diff = neighbor + point - cellUV;
            float d = length(diff);
            if (d < minDist) {
                secondDist = minDist;
                minDist = d;
                nearestCell = cellID + neighbor;
            } else if (d < secondDist) {
                secondDist = d;
            }
        }
    }

    float edge = secondDist - minDist;
    float crackLine = smoothstep(0.05, 0.0, edge);

    float shardRand = hash1(nearestCell);
    float fallStart = 0.15 + shardRand * 0.35;
    float fallSpeed = 2.0 + shardRand * 3.0;

    float fallTime = max(time - fallStart, 0.0);
    float fallDist = fallTime * fallTime * fallSpeed;

    float shardGone = step(0.8, fallDist);

    vec4 sampled = texture(image, uv);

    float crackIntensity = crackLine * clamp(time * 5.0, 0.0, 1.0) * clamp(1.0 - fallTime * 2.0, 0.0, 1.0);

    float tintStrength = time * 0.5;
    vec3 tinted = mix(sampled.rgb, sampled.rgb * tintColor, tintStrength);

    vec3 shardWithCracks = tinted + vec3(crackIntensity);
    vec3 finalColor = mix(shardWithCracks, vec3(0.0), shardGone);

    FragColor = spriteColor * vec4(finalColor, 1.0);
}
