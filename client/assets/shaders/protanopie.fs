#version 120

uniform sampler2D texture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    float red   = color.r * 0.567 + color.g * 0.433;
    float green = color.r * 0.558 + color.g * 0.442;
    float blue  = color.b;

    gl_FragColor = vec4(red, green, blue, color.a);
}
