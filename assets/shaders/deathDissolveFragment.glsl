#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;

float hash(float n) {
    return fract(sin(n) * 43758.5453);
}

void main()
{
    float row = floor(TexCoords.y);
    float rowSeed = hash(row * 17.3);
    float direction = sign(rowSeed - 0.5);
    float speed = 0.5 + rowSeed * 1.5;
    float offset = direction * speed * time * srcRect.z;

    float sampleX = TexCoords.x - offset;

    if (sampleX < 0.0 || sampleX >= srcRect.z) discard;

    ivec2 texel = ivec2(srcRect.xy) + ivec2(int(sampleX), int(TexCoords.y));
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
    vec4 sampled = texelFetch(image, texel, 0);

    if (sampled.a < 0.01) discard;

    vec3 redColor = vec3(1.0, 0.25, 0.25);
    float redMix = clamp(time * 3.0, 0.0, 0.9);
    vec3 tinted = mix(sampled.rgb, redColor, redMix);

    float alpha = sampled.a * (1.0 - smoothstep(0.3, 1.0, time));

    color = spriteColor * vec4(tinted, alpha);
}
