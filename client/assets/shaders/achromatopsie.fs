#version 120

uniform sampler2D texture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    gl_FragColor = vec4(gray, gray, gray, color.a);
}
