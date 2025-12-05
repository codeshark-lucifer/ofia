#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float gammaCorrected; // typically 2.2 for sRGB correction

void main()
{
    vec3 color = texture(screenTexture, TexCoords).rgb;
    // Apply gamma correction
    color = pow(color, vec3(1.0 / gammaCorrected));
    FragColor = vec4(color, 1.0);
}
