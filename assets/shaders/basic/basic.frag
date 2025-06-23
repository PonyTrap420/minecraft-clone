#version 330 core

uniform sampler2D texture1;

out vec4 FragColor;
in vec2 TexCoord;
flat in float FaceIndex;

void main()
{
    float shadow = 1.0;
    if (FaceIndex == 5.0)      shadow = 1.00;
    else if (FaceIndex == 4.0) shadow = 0.70;
    else shadow=0.86;
    vec4 texColor = texture(texture1, TexCoord);
    FragColor = vec4(texColor.rgb * shadow, texColor.a);
} 