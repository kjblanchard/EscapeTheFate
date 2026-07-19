#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

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
    float luma = dot(accum.rgb, vec3(0.299, 0.587, 0.114));
    vec3 desaturated = mix(accum.rgb, vec3(luma), time * 0.6);
    FragColor = spriteColor * vec4(desaturated * fade, accum.a * fade);
}
