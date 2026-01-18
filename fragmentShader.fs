#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform bool lightOn;

void main() {
    if (lightOn) {
        FragColor = vec4(color, 1.0);
    } else {
        FragColor = vec4(color * 0.35, 1.0);
    }
}
