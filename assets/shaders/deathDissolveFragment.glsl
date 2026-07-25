#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;

void main()
{
    float stretch = 1.0 + time * 3.0;
    float normalizedY = TexCoords.y / srcRect.w;
    float stretchedY = normalizedY * stretch - (stretch - 1.0);

    if (stretchedY < 0.0 || stretchedY > 1.0) discard;

    int samples = 8;
    float blurSpread = time * 12.0;
    vec4 accumulated = vec4(0.0);
    float totalWeight = 0.0;

    for (int i = 0; i < samples; i++) {
        float offset = (float(i) / float(samples - 1) - 0.5) * blurSpread;
        float sampleY = stretchedY * srcRect.w + offset;
        ivec2 texel = ivec2(srcRect.xy) + ivec2(TexCoords.x, sampleY);
        ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
        texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
        float weight = 1.0 - abs(float(i) / float(samples - 1) - 0.5) * 2.0;
        accumulated += texelFetch(image, texel, 0) * weight;
        totalWeight += weight;
    }

    vec4 sampled = accumulated / totalWeight;
    if (sampled.a < 0.01) discard;

    float redMix = clamp(time * 2.0, 0.0, 0.85);
    vec3 redTinted = mix(sampled.rgb, vec3(sampled.r * 1.5, sampled.g * 0.1, sampled.b * 0.05), redMix);

    float alpha = sampled.a * (1.0 - smoothstep(0.3, 1.0, time));

    color = spriteColor * vec4(redTinted, alpha);
}
