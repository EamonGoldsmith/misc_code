#version 330 core

uniform float iTime;

vec2 iResolution = vec2(800.0, 600.0);

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 aspect_uv = uv * 2.0 - 1.0;

    aspect_uv.x *= iResolution.x / iResolution.y;

    float r = length(aspect_uv);
    float a = atan(aspect_uv.y, aspect_uv.x);


    float spiral_freq = 15.0;
    float spiral_speed = 0.1;
    float pattern_base = sin(a * spiral_freq + iTime * spiral_speed);

    float radial_freq = 30.0;
    float radial_speed = 3.0;
    float pattern_radial = sin(r * radial_freq - iTime * radial_speed);

    float warp = pow(r, 0.8) * 1.5;

    float pattern_value = fract(pattern_base * 0.5 + pattern_radial * 0.5 + warp);

    float hue = a / (2.0 * 3.14159) + iTime * 0.05;
    hue = fract(hue);

    float sat = 0.8 - r * 0.2;

    float val = pattern_value;

    vec3 color = mod(hue + vec3(0.0, 0.66, 0.33), 1.0);

    float f = step(val, color.r) * step(val, color.g) * step(val, color.b);

    vec3 final_color = mix(
        vec3(0.5, 0.5, 0.5), 
        0.5 + 0.5 * cos(color * 6.28318 + iTime * 0.3 + val * 2.0), 
        clamp(pattern_value * 2.0 * (1.0 - r), 0.0, 1.0)
    );

    final_color *= (0.8 + 0.2 * r);

    gl_FragColor = vec4(final_color, 1.0);
}