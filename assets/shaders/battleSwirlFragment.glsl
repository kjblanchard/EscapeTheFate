#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;

void main() {
    vec2 uv = TexCoords - vec2(0.5);
    float aspect = resolution.x / resolution.y;
    uv.x *= aspect;

    float dist = length(uv);
    float angle = atan(uv.y, uv.x);

    float swirl = time * 8.0;
    float shrink = 1.0 - time * 0.85;

    float rotated = angle + swirl * (1.0 - dist);
    vec2 swirlUV;
    swirlUV.x = dist * cos(rotated) / aspect + 0.5;
    swirlUV.y = dist * sin(rotated) + 0.5;

    float fade = clamp(1.0 - time * 1.4, 0.0, 1.0);
    vec4 sampled = texture(image, swirlUV);
    float centerFade = clamp(dist / (shrink * 0.5 + 0.01), 0.0, 1.0);

    // Warm red/orange tint that intensifies with the swirl
    vec3 tinted = sampled.rgb;
    tinted.r = min(tinted.r * (1.0 + time * 0.8), 1.0);
    tinted.g *= (1.0 - time * 0.4);
    tinted.b *= (1.0 - time * 0.6);
    // Boost contrast as the effect ramps up
    tinted = mix(tinted, tinted * tinted * 2.0, time * 0.5);

    color = spriteColor * vec4(tinted * fade * centerFade, sampled.a * fade * centerFade);
}
