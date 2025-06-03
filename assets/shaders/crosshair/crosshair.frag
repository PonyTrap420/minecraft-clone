#version 330 core

out vec4 FragColor;
uniform vec2 resolution;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    vec2 center = vec2(0.5);

    float lineThickness = 1.0 / resolution.y;
    float lineLength = 0.01;

    float dx = abs(uv.x - center.x);
    float dy = abs(uv.y - center.y);

    bool vertical   = (dx < lineThickness) && (dy < lineLength);
    bool horizontal = (dy < lineThickness) && (dx < lineLength - 0.002);

    if (vertical || horizontal)
        FragColor = vec4(0.2, 0.2, 0.2, 1.0);
    else
        discard;
}