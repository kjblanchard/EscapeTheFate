#version 330 core
in vec2 TexCoords;
out vec4 color;

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

    // --- Phase 1: Cracks appear (time 0.0 - 0.3) ---
    // --- Phase 2: Shards fall (time 0.2 - 1.0) ---

    float cellScale = 10.0;
    vec2 scaledUV = uv * cellScale;
    vec2 cellID = floor(scaledUV);
    vec2 cellUV = fract(scaledUV);

    // Voronoi to find which shard this pixel belongs to
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

    // Crack edge detection
    float edge = secondDist - minDist;
    float crackLine = smoothstep(0.05, 0.0, edge);

    // Per-shard properties from hash
    float shardRand = hash1(nearestCell);
    // Staggered fall start: shards begin falling at different times between 0.15 and 0.5
    float fallStart = 0.15 + shardRand * 0.35;
    float fallSpeed = 2.0 + shardRand * 3.0;

    // How far along this shard's fall are we
    float fallTime = max(time - fallStart, 0.0);
    // Gravity: accelerating downward
    float fallDist = fallTime * fallTime * fallSpeed;

    // The shard is "gone" once it's fallen far enough
    float shardGone = step(0.8, fallDist);

    // Sample the original screen image at this pixel's actual position
    vec4 sampled = texture(image, uv);

    // Crack flash — bright white lines that appear early
    float crackIntensity = crackLine * clamp(time * 5.0, 0.0, 1.0) * clamp(1.0 - fallTime * 2.0, 0.0, 1.0);

    // Slight tint on the shards
    float tintStrength = time * 0.5;
    vec3 tinted = mix(sampled.rgb, sampled.rgb * tintColor, tintStrength);

    // Final: show the textured shard with cracks, or black if shard has fallen away
    vec3 shardWithCracks = tinted + vec3(crackIntensity);
    vec3 finalColor = mix(shardWithCracks, vec3(0.0), shardGone);

    color = spriteColor * vec4(finalColor, 1.0);
}
