#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;

float hash2(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{
    ivec2 texel = ivec2(srcRect.xy) + ivec2(TexCoords);
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
    vec4 sampled = texelFetch(image, texel, 0);

    if (sampled.a < 0.01) discard;

    vec2 localPixel = floor(TexCoords);
    float noise = hash2(localPixel);

    float vertBias = (TexCoords.y / srcRect.w) * 0.15;
    float threshold = noise * 0.85 + vertBias;

    if (threshold < time) discard;

    float edgeWidth = 0.10;
    float edgeFactor = 1.0 - smoothstep(0.0, edgeWidth, threshold - time);

    float redMix = clamp(time * 1.5, 0.0, 0.65);
    vec3 redTinted = mix(sampled.rgb, vec3(sampled.r * 1.2, sampled.g * 0.15, sampled.b * 0.05), redMix);

    vec3 glowColor = vec3(1.0, 0.35, 0.0);
    float glowDecay = 1.0 - smoothstep(0.5, 1.0, time);
    vec3 finalRgb = mix(redTinted, glowColor, edgeFactor * glowDecay * 0.9);

    FragColor = spriteColor * vec4(finalRgb, sampled.a);
}
