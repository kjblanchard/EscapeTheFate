#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
uniform vec3 tintColor;

const float CELL_SCALE = 8.0;

vec2 hash2(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453);
}

float hash1(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void voronoi(vec2 uv, out vec2 nearestCell, out float minDist, out float secondDist) {
    vec2 scaledUV = uv * CELL_SCALE;
    vec2 cellID = floor(scaledUV);
    vec2 cellUV = fract(scaledUV);
    minDist = 10.0;
    secondDist = 10.0;
    nearestCell = cellID;
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
}

vec2 getShardDisplacement(vec2 cellID) {
    float shardRand = hash1(cellID);
    float fallStart = 0.1 + shardRand * 0.25;
    float fallSpeed = 4.0 + shardRand * 8.0;
    float fallTime = max(time - fallStart, 0.0);
    float fallDist = fallTime * fallTime * fallSpeed;
    float xDrift = (hash1(cellID + vec2(7.3, 13.1)) - 0.5) * 0.3 * fallTime;
    return vec2(xDrift, fallDist) / CELL_SCALE;
}

void main() {
    vec2 uv = TexCoords;

    // Find which shard this pixel belongs to
    vec2 cellID;
    float minDist, secondDist;
    voronoi(uv, cellID, minDist, secondDist);

    // Crack edge detection
    float edge = secondDist - minDist;
    float crackLine = smoothstep(0.05, 0.0, edge);
    float crackAppear = smoothstep(0.0, 0.25, time);

    // Per-shard fall
    vec2 disp = getShardDisplacement(cellID);
    float shardRand = hash1(cellID);
    float shardFallStart = 0.1 + shardRand * 0.25;
    float shardFalling = step(shardFallStart, time);

    // Source UV: where this shard pixel originally was before it fell here
    vec2 sourceUV = uv - disp;

    // If source is off-screen, the shard has left the viewport — show black
    bool offScreen = sourceUV.x < 0.0 || sourceUV.x > 1.0 ||
                     sourceUV.y < 0.0 || sourceUV.y > 1.0;

    // Sample the original screen at the source position
    vec4 sampled = texture(image, sourceUV);

    // Tint
    float tintStrength = time * 0.5;
    vec3 tinted = mix(sampled.rgb, sampled.rgb * tintColor, tintStrength);

    // Crack highlight on the shard edges
    float crackFade = clamp(1.0 - (time - shardFallStart) * 3.0, 0.0, 1.0);
    float crackIntensity = crackLine * crackAppear * crackFade;

    vec3 shardColor = tinted + vec3(crackIntensity);

    // Before falling starts, show original image with cracks
    // After falling starts, show the displaced textured shard
    vec3 finalColor;
    if (shardFalling < 0.5) {
        // Shard hasn't started falling yet — show static image with cracks
        vec3 original = texture(image, uv).rgb;
        finalColor = original + vec3(crackLine * crackAppear);
    } else if (offScreen) {
        // Shard has fallen off — black void
        finalColor = vec3(0.0);
    } else {
        finalColor = shardColor;
    }

    color = spriteColor * vec4(finalColor, 1.0);
}
