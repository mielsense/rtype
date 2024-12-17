#version 120

uniform sampler2D texture;
uniform vec2 resolution;
uniform float time;

const float PI = 3.1415926535;

// Function to generate noise
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = dot(i, vec2(12.9898, 78.233));
    float b = dot(i + vec2(1.0, 0.0), vec2(12.9898, 78.233));
    float c = dot(i + vec2(0.0, 1.0), vec2(12.9898, 78.233));
    float d = dot(i + vec2(1.0, 1.0), vec2(12.9898, 78.233));

    float u = fract(sin(a) * 43758.5453123);
    float v = fract(sin(b) * 43758.5453123);
    float w = fract(sin(c) * 43758.5453123);
    float x = fract(sin(d) * 43758.5453123);

    vec2 smoothStep = f * f * (3.0 - 2.0 * f);
    return mix(mix(u, v, smoothStep.x), mix(w, x, smoothStep.x), smoothStep.y);
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 pixelCoord = uv * resolution;

    // Create an animated offset for dithering
    vec2 offset = vec2(sin(time * 1.0), cos(time * 1.0)) * 5.0;

    // Apply the noise function to create a dither pattern
    float n = noise(pixelCoord + offset);

    // Calculate chromatic aberration offsets
    float maxOffset = 0.0025; // Maximum offset for the chromatic aberration
    vec2 center = resolution * 0.5; // Center of the screen
    vec2 toCenter = center - pixelCoord;
    float distance = length(toCenter) / length(resolution * 0.5);
    float chromaticOffset = maxOffset * distance;

    // Calculate the texture coordinates for the red, green, and blue channels
    vec2 redTexCoord = uv + vec2(chromaticOffset, -chromaticOffset);
    vec2 greenTexCoord = uv;
    vec2 blueTexCoord = uv + vec2(-chromaticOffset, chromaticOffset);

    // Sample the texture at the displaced coordinates
    vec4 red = texture2D(texture, redTexCoord);
    vec4 green = texture2D(texture, greenTexCoord);
    vec4 blue = texture2D(texture, blueTexCoord);

    // Combine the color channels
    vec4 color = vec4(red.r, green.g, blue.b, 1.0);

    // Apply the dithering effect
    float threshold = 0.5; // Adjust threshold for dithering effect
    float dither = (n - threshold) * 0.1; // Adjust the impact of dithering
    color.r += dither;
    color.g += dither;
    // Optionally, you can reduce the impact on the blue channel to enhance the yellow effect
    color.b -= dither * 0.5;

    gl_FragColor = color;
}
