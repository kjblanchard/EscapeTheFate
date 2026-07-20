#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
const float COLS = 7.0;
const float ROWS = 5.0;

float hash1(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

vec2 hash2(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453);
}

float getCellFallDist(vec2 cell) {
    float r = hash1(cell);
    float fallStart = 0.15 + r * 0.35;
    float fallSpeed = 2.5 + hash1(cell + vec2(3.7, 9.2)) * 3.0;
    float t = max(time - fallStart, 0.0);
    return t * t * fallSpeed;
}

float getCellXDrift(vec2 cell) {
    return (hash1(cell + vec2(7.3, 13.1)) - 0.5) * 0.15;
}

void main() {
    vec2 uv = TexCoords;
    float cellW = 1.0 / COLS;
    float cellH = 1.0 / ROWS;

    float col = floor(uv.x * COLS);

    vec3 finalColor = vec3(0.0);
    bool found = false;

    for (int r = 0; r < 5; r++) {
        vec2 cell = vec2(col, float(r));
        float fallDist = getCellFallDist(cell);
        float xDrift = getCellXDrift(cell) * fallDist;

        float cellTop = float(r) * cellH + fallDist;
        float cellBot = float(r + 1) * cellH + fallDist;
        float cellLeft = col * cellW + xDrift;
        float cellRight = (col + 1.0) * cellW + xDrift;

        if (uv.y >= cellTop && uv.y < cellBot && uv.x >= cellLeft && uv.x < cellRight) {
            vec2 sourceUV = vec2(uv.x - xDrift, uv.y - fallDist);

            if (sourceUV.y >= 0.0 && sourceUV.y <= 1.0) {
                finalColor = texture(image, sourceUV).rgb;
                found = true;
            }
            break;
        }
    }

    float crackPhase = smoothstep(0.0, 0.2, time) * smoothstep(0.5, 0.3, time);
    if (crackPhase > 0.0) {
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
        finalColor += vec3(crackLine * crackPhase);
    }

    if (found) {
        float edgeX = min(fract(uv.x * COLS), 1.0 - fract(uv.x * COLS));
        float edgeY = min(fract(uv.y * ROWS), 1.0 - fract(uv.y * ROWS));
        float gridEdge = 1.0 - smoothstep(0.0, 0.03, min(edgeX, edgeY));
        finalColor = mix(finalColor, vec3(0.8), gridEdge * 0.6);
    }

    FragColor = spriteColor * vec4(finalColor, 1.0);
}
