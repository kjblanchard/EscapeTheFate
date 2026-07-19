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

    float cellScale = 12.0;
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

    float shardRand = hash1(nearestCell);
    float fallDelay = shardRand * 0.4;
    float fallSpeed = 1.5 + shardRand * 2.0;
    float rotSpeed = (shardRand - 0.5) * 4.0;

    float crackLine = smoothstep(0.02, 0.0, edge);
    float crackFlash = clamp(1.0 - time * 3.0, 0.0, 1.0);

    float fallTime = max(time - fallDelay * 0.3, 0.0);
    float gravity = fallTime * fallTime * fallSpeed;
    float rotation = fallTime * rotSpeed;

    vec2 shardCenter = (nearestCell + 0.5) / cellScale;
    vec2 toCenter = uv - shardCenter;
    float cosR = cos(rotation);
    float sinR = sin(rotation);
    vec2 rotatedOffset = vec2(
        toCenter.x * cosR - toCenter.y * sinR,
        toCenter.x * sinR + toCenter.y * cosR
    );
    vec2 fallingUV = shardCenter + rotatedOffset + vec2(0.0, gravity * 0.3);

    vec4 sampled = texture(image, fallingUV);

    float shardVisible = step(fallingUV.y, 1.3) * step(-0.3, fallingUV.x) * step(fallingUV.x, 1.3);
    float shardFade = clamp(1.0 - fallTime * 1.2, 0.0, 1.0);

    float tintStrength = time * 0.8;
    vec3 tinted = mix(sampled.rgb, sampled.rgb * tintColor, tintStrength);

    vec3 shardColor = tinted * shardVisible * shardFade;
    vec3 crackColor = vec3(1.0) * crackLine * crackFlash;
    vec3 finalColor = shardColor + crackColor * shardVisible * shardFade;

    FragColor = spriteColor * vec4(finalColor, 1.0);
}
