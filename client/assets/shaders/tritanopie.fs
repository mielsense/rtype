#version 120

uniform sampler2D texture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    float red   = color.r;
    float green = color.g * 0.95 + color.b * 0.05;
    float blue  = color.g * 0.433 + color.b * 0.567;

    gl_FragColor = vec4(red, green, blue, color.a);
}
