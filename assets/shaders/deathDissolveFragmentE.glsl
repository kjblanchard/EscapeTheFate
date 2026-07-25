#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;

void main()
{
    float blurRadius = time * srcRect.z * 0.5;
    vec4 accumulated = vec4(0.0);
    float totalWeight = 0.0;
    int samples = 16;

    for (int i = 0; i < samples; i++) {
        float t = float(i) / float(samples - 1) - 0.5;
        float offset = t * 2.0 * blurRadius;
        float sampleX = TexCoords.x + offset;
        ivec2 texel = ivec2(srcRect.xy) + ivec2(int(sampleX), int(TexCoords.y));
        ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
        texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
        float weight = 1.0 - abs(t) * 2.0;
        weight = weight * weight;
        accumulated += texelFetch(image, texel, 0) * weight;
        totalWeight += weight;
    }

    vec4 sampled = accumulated / totalWeight;
    if (sampled.a < 0.01) discard;

    vec3 redColor = vec3(1.0, 0.3, 0.3);
    float redMix = clamp(time * 2.5, 0.0, 0.9);
    vec3 tinted = mix(sampled.rgb, redColor, redMix);

    float alpha = sampled.a * (1.0 - smoothstep(0.2, 1.0, time));

    FragColor = spriteColor * vec4(tinted, alpha);
}
