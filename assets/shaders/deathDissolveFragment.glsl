#version 330 core
in vec2 TexCoords;
out vec4 color;

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
    vec2 blockId = floor(TexCoords);
    float bh = hash(blockId.x * 127.1 + blockId.y * 311.7 + seed * 53.3);

    float angle = hash(bh * 71.9 + seed * 13.1) * 6.2832;
    float speed = 2.0 + hash(bh * 37.3 + seed * 97.1) * 4.0;
    float angularVel = (hash(bh * 23.7 + seed * 41.3) - 0.5) * 5.0;

    float delay = bh * 0.25;
    float localTime = max(0.0, time - delay);

    float curAngle = angle + angularVel * localTime;
    vec2 disp = vec2(cos(curAngle), sin(curAngle)) * speed * localTime * localTime * srcRect.z * 0.5;

    vec2 samplePos = TexCoords - disp;

    if (samplePos.x < 0.0 || samplePos.x >= srcRect.z ||
        samplePos.y < 0.0 || samplePos.y >= srcRect.w) discard;

    ivec2 texel = ivec2(srcRect.xy) + ivec2(int(samplePos.x), int(samplePos.y));
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
    vec4 sampled = texelFetch(image, texel, 0);

    if (sampled.a < 0.01) discard;

    vec3 redColor = vec3(1.0, 0.3, 0.25);
    float redMix = smoothstep(0.0, 0.15, time);
    vec3 tinted = mix(sampled.rgb, redColor, redMix * 0.9);

    float alpha = sampled.a * 0.6 * (1.0 - smoothstep(0.15, 0.85, time));

    color = spriteColor * vec4(tinted, alpha);
}
