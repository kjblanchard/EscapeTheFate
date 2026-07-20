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

    // --- Phase 1: Cracks appear (time 0.0 - 0.25) ---
    vec2 crackCell;
    float crackMin, crackSecond;
    voronoi(uv, crackCell, crackMin, crackSecond);
    float edge = crackSecond - crackMin;
    float crackLine = smoothstep(0.05, 0.0, edge);
    float crackAppear = smoothstep(0.0, 0.25, time);
    float crackIntensity = crackLine * crackAppear;

    // --- Phase 2: Shards fall (time > 0.1) ---
    // Reverse-map: find which shard has fallen to cover this pixel.
    // A shard originally at sourceUV has displacement D, so it's now at sourceUV + D.
    // We want sourceUV + D = uv, i.e., sourceUV = uv - D.
    // Iterate twice to converge.

    // Iteration 1: assume no displacement, find local shard
    vec2 cell1;
    float min1, sec1;
    voronoi(uv, cell1, min1, sec1);
    vec2 disp1 = getShardDisplacement(cell1);

    // Iteration 2: look at the source position
    vec2 sourceUV = uv - disp1;
    vec2 cell2;
    float min2, sec2;
    voronoi(sourceUV, cell2, min2, sec2);
    vec2 disp2 = getShardDisplacement(cell2);

    // Final source: where the shard covering this pixel originally was
    vec2 finalSource = uv - disp2;
    vec2 finalCell;
    float finalMin, finalSec;
    voronoi(finalSource, finalCell, finalMin, finalSec);

    // Check if this shard actually reaches our pixel
    vec2 finalDisp = getShardDisplacement(finalCell);
    vec2 expectedPos = finalSource + finalDisp;
    float error = length(expectedPos - uv) * CELL_SCALE;

    // If source is out of bounds or error is too large, no shard covers this pixel
    bool hasShard = finalSource.x >= 0.0 && finalSource.x <= 1.0 &&
                    finalSource.y >= 0.0 && finalSource.y <= 1.0 &&
                    error < 0.6;

    // Edge detection on the fallen shard
    float shardEdge = finalSec - finalMin;
    float shardCrack = smoothstep(0.05, 0.02, shardEdge);

    // Sample texture at the shard's original position
    vec4 sampled = texture(image, finalSource);

    // Tint
    float tintStrength = time * 0.5;
    vec3 tinted = mix(sampled.rgb, sampled.rgb * tintColor, tintStrength);

    // Compose: before shards start moving, show cracks on original image
    // After movement begins, show displaced shards with edges
    float fallPhase = smoothstep(0.1, 0.3, time);
    float shardRand = hash1(finalCell);
    float shardFallStart = 0.1 + shardRand * 0.25;
    float shardFalling = step(shardFallStart, time);

    vec3 staticImage = texture(image, uv).rgb + vec3(crackIntensity);
    vec3 fallenShard = tinted * (1.0 - shardCrack * 0.5) + vec3(shardCrack * 0.3);

    vec3 finalColor;
    if (!hasShard && fallPhase > 0.0) {
        finalColor = mix(staticImage, vec3(0.0), fallPhase);
    } else if (shardFalling > 0.0 && fallPhase > 0.0) {
        finalColor = mix(staticImage, fallenShard, fallPhase);
    } else {
        finalColor = staticImage;
    }

    color = spriteColor * vec4(finalColor, 1.0);
}
