#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, TexCoord).rgb;

    // Optional: simple post-processing effect (e.g., invert colors)
    // color = vec3(1.0) - color;

    FragColor = vec4(color, 1.0);
}
