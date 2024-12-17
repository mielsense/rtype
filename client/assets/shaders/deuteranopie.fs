#version 120

uniform sampler2D texture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec4 color = texture2D(texture, uv);

    float red = color.r * 0.625 + color.g * 0.375;
    float green = color.r * 0.3 + color.g * 0.7;
    float blue = color.b;

    gl_FragColor = vec4(red, green, blue, color.a);
}
