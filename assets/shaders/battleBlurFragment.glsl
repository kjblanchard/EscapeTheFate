#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
uniform vec3 tintColor;

void main() {
    const int SAMPLES = 12;
    vec2 uv = TexCoords;

    vec2 dir = uv - vec2(0.5);
    vec2 shatterDir = vec2(1.0, 0.0) * time * 0.3;

    vec4 accum = vec4(0.0);
    float blurStrength = time * 0.08;

    for (int i = 0; i < SAMPLES; ++i) {
        float t = float(i) / float(SAMPLES - 1) - 0.5;
        vec2 offset = (dir * blurStrength + shatterDir * float(i) / float(SAMPLES)) * t;
        accum += texture(image, uv + offset);
    }
    accum /= float(SAMPLES);

    float fade = clamp(1.0 - time * 1.1, 0.0, 1.0);
    // Strong color tint that ramps with time
    float tintStrength = time * 1.2;
    vec3 shifted = mix(accum.rgb, accum.rgb * tintColor, tintStrength);
    // Boost brightness and contrast
    shifted *= (1.0 + time * 0.5);
    shifted = pow(shifted, vec3(1.0 - time * 0.2));

    color = spriteColor * vec4(shifted * fade, accum.a * fade);
}
