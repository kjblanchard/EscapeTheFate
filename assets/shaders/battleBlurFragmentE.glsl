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

    // Shift UVs to the left over time so the image appears to pull right
    float pullStrength = time * time * 0.4;
    uv.x -= pullStrength;

    // Heavy directional blur toward the right that ramps aggressively
    vec2 blurDir = vec2(1.0, 0.0);
    float blurAmount = time * time * 0.25;

    vec4 accum = vec4(0.0);
    for (int i = 0; i < SAMPLES; ++i) {
        float t = float(i) / float(SAMPLES - 1);
        vec2 offset = blurDir * blurAmount * t;
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
