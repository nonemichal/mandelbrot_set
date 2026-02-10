#version 330

// Pixel coordinates in [0.0, 1.0]
in vec2 fragTexCoord;
// Output color
out vec4 fragColor;
// Color palette
uniform sampler2D uColorPalette;

uniform float zoom; // Zoom value
uniform int maxIter; // Max iteration value
uniform int bailoutPower; // Escape value is 2^(bailoutPower)
uniform int paletteSize; // Color palette size
uniform bool scaleIter; // Should iteration value be scaled based on color palette size

// Source: https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set

int bailoutRadius = (1 << bailoutPower); // Escape value
// Y scaled to [-1.25, 1.25]
const float minY = -1.25;
const float maxY = 1.25;
// X scaled to [-2.5, 1.0]
const float minX = -2.5;
const float maxX = 1.0;

void main() {

  // f_c(z) = z^2 + c
  vec2 c = vec2(
      mix(minX, maxX, fragTexCoord.x),
      mix(minY, maxY, fragTexCoord.y)
    );
  vec2 z = vec2(0.0);
  vec2 z2 = vec2(0.0);

  // Optimized escape algorithm
  int iter = 0;
  while (z2.x + z2.y <= bailoutRadius && iter < maxIter) {
    z2.x = z.x * z.x;
    z2.y = z.y * z.y;
    z.y = 2.0 * z.x * z.y + c.y;
    z.x = z2.x - z2.y + c.x;
    iter++;
  }

  // Avoid floating point issues
  float logZn = log(z.x * z.x + z.y * z.y) / 2.0;
  float nu = log(logZn / log(2.0)) / log(2.0);
  float fixedIter = float(iter) + 1.0 - nu;

  int indexVal; // Color palette index
  if (scaleIter) {
    // Scale iter value based on palette size
    float t = fixedIter / float(maxIter);
    int paletteIter = int(floor(t * paletteSize));
    indexVal = paletteIter;
  } else {
    // Use not scaled value
    indexVal = int(floor(fixedIter));
  }

  // Two nearest palette indices provides gradient
  int index1 = indexVal;
  int index2 = min(index1 + 1, maxIter);

  // Fetch colors
  vec3 color1 = texture(uColorPalette, vec2(float(index1) / float(maxIter), 0.5)).rgb;
  vec3 color2 = texture(uColorPalette, vec2(float(index2) / float(maxIter), 0.5)).rgb;

  // Interpolate color
  float frac = fixedIter - floor(fixedIter);
  vec3 color = mix(color1, color2, frac);
  fragColor = vec4(color, 1.0); // Output color
}
