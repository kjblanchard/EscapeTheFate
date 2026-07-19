#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
uniform vec3 tintColor;

void main() {
    const int SAMPLES = 16;
    vec2 uv = TexCoords;

    // Per-scanline variation — different rows pull at different speeds
    float row = floor(uv.y * resolution.y);
    float lineNoise = fract(sin(row * 43.758) * 2137.193);
    float lineSpeed = 0.6 + lineNoise * 0.8;

    // Dramatic pull to the right — cubic ramp with per-line variation
    float pullStrength = time * time * time * 1.2 * lineSpeed;
    uv.x -= pullStrength;

    // Heavy directional blur that increases per line
    float blurAmount = time * time * 0.35 * lineSpeed;

    vec4 accum = vec4(0.0);
    for (int i = 0; i < SAMPLES; ++i) {
        float t = float(i) / float(SAMPLES - 1);
        vec2 offset = vec2(blurAmount * t, 0.0);
        accum += texture(image, uv + offset);
    }
    accum /= float(SAMPLES);

    float fade = clamp(1.0 - time * 1.1, 0.0, 1.0);
    float tintStrength = time * 1.2;
    vec3 shifted = mix(accum.rgb, accum.rgb * tintColor, tintStrength);
    shifted *= (1.0 + time * 0.5);
    shifted = pow(shifted, vec3(1.0 - time * 0.2));

    FragColor = spriteColor * vec4(shifted * fade, accum.a * fade);
}
