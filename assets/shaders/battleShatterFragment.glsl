#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
uniform vec3 tintColor;

// Hash functions for pseudo-random per-cell values
vec2 hash2(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453);
}

float hash1(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main() {
    vec2 uv = TexCoords;

    // Divide screen into a grid of cells for the Voronoi pattern
    float cellScale = 12.0;
    vec2 scaledUV = uv * cellScale;
    vec2 cellID = floor(scaledUV);
    vec2 cellUV = fract(scaledUV);

    // Find nearest Voronoi center and distance to edge
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

    // Edge distance for crack lines
    float edge = secondDist - minDist;

    // Per-shard random values based on which cell we belong to
    float shardRand = hash1(nearestCell);
    float fallDelay = shardRand * 0.4;
    float fallSpeed = 1.5 + shardRand * 2.0;
    float rotSpeed = (shardRand - 0.5) * 4.0;

    // Crack lines appear instantly, white flash
    float crackLine = smoothstep(0.02, 0.0, edge);
    // Cracks get brighter briefly then fade
    float crackFlash = clamp(1.0 - time * 3.0, 0.0, 1.0);

    // Shard falling — starts after fallDelay
    float fallTime = max(time - fallDelay * 0.3, 0.0);
    float gravity = fallTime * fallTime * fallSpeed;
    float rotation = fallTime * rotSpeed;

    // Offset the UV to simulate the shard falling and rotating
    vec2 shardCenter = (nearestCell + 0.5) / cellScale;
    vec2 toCenter = uv - shardCenter;
    float cosR = cos(rotation);
    float sinR = sin(rotation);
    vec2 rotatedOffset = vec2(
        toCenter.x * cosR - toCenter.y * sinR,
        toCenter.x * sinR + toCenter.y * cosR
    );
    vec2 fallingUV = shardCenter + rotatedOffset + vec2(0.0, gravity * 0.3);

    // Sample the texture at the falling shard position
    vec4 sampled = texture(image, fallingUV);

    // Shard becomes invisible once it falls off screen
    float shardVisible = step(fallingUV.y, 1.3) * step(-0.3, fallingUV.x) * step(fallingUV.x, 1.3);
    // Also fade out shards over time
    float shardFade = clamp(1.0 - fallTime * 1.2, 0.0, 1.0);

    // Tint the shards
    float tintStrength = time * 0.8;
    vec3 tinted = mix(sampled.rgb, sampled.rgb * tintColor, tintStrength);

    // Combine: shards with cracks, over black background
    vec3 shardColor = tinted * shardVisible * shardFade;
    vec3 crackColor = vec3(1.0) * crackLine * crackFlash;
    vec3 finalColor = shardColor + crackColor * shardVisible * shardFade;

    color = spriteColor * vec4(finalColor, 1.0);
}
