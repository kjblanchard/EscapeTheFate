#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;
uniform float seed;

float hash(float n) {
    return fract(sin(n) * 43758.5453);
}

void main()
{
    float stripId = floor(TexCoords.y / 3.0);
    float stripSeed = hash(stripId * 17.3 + seed * 91.7);

    float direction = sign(stripSeed - 0.5);
    float speed = 1.0 + stripSeed * 3.0;

    float delay = hash(stripId * 53.1 + seed * 7.3) * 0.3;
    float localTime = max(0.0, time - delay);

    float offset = direction * speed * localTime * localTime * srcRect.z;

    float sampleX = TexCoords.x - offset;
    if (sampleX < 0.0 || sampleX >= srcRect.z) discard;

    ivec2 texel = ivec2(srcRect.xy) + ivec2(int(sampleX), int(TexCoords.y));
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
    vec4 sampled = texelFetch(image, texel, 0);

    if (sampled.a < 0.01) discard;

    vec3 redColor = vec3(1.0, 0.3, 0.3);
    float redMix = clamp(time * 2.5, 0.0, 0.85);
    vec3 tinted = mix(sampled.rgb, redColor, redMix);

    float alpha = sampled.a * (1.0 - smoothstep(0.2, 0.9, time));

    FragColor = spriteColor * vec4(tinted, alpha);
}
