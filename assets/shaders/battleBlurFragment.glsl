#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;

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
    // Cool blue/purple color shift
    vec3 shifted = accum.rgb;
    shifted.r *= (1.0 - time * 0.5);
    shifted.g *= (1.0 - time * 0.3);
    shifted.b = min(shifted.b * (1.0 + time * 0.6), 1.0);
    // Increase contrast and brightness punch
    shifted = shifted * (1.0 + time * 0.4);
    float luma = dot(shifted, vec3(0.299, 0.587, 0.114));
    vec3 desaturated = mix(shifted, vec3(luma), time * 0.3);

    color = spriteColor * vec4(desaturated * fade, accum.a * fade);
}
