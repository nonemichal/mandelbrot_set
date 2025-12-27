#version 330

uniform vec3 u_color; // Color provided from CPU (RGB)

out vec4 fragColor; // Final color output to the framebuffer

void main() {
    // Set fragment color using uniform RGB and full opacity
    fragColor = vec4(u_color, 1.0);
}
