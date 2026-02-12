#version 330

in vec2 fragTexCoord; // Pixel coordinates in [0.0, 1.0]

out vec4 fragColor; // Output color

uniform sampler2D uColorPalette; // Color palette
uniform int maxIter; // Maximum number of iterations per point
uniform int bailoutPower; // Escape radius 2^bailoutPower
uniform int paletteSize; // Number of colors in the palette texture
uniform float colorDetail; // Multiplier controlling palette cycling
uniform float zoom; // Zoom factor
uniform int useSmoothing; // Smooth iteration count for gradient coloring
uniform int useColorBoost; // true = continuous palette, false = index-based palette

// Source: https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set

int bailoutRadius = (1 << bailoutPower); // Escape value
// Y scaled to [-1.25, 1.25]
const float minY = -1.25;
const float maxY = 1.25;
// X scaled to [-2.5, 1.0]
const float minX = -2.5;
const float maxX = 1.0;
// Bool values
bool useSmoothingBool = bool(useSmoothing);
bool useColorBoostBool = bool(useColorBoost);

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

  // Iter value as float
  float iterFloat;

  // Smooth iteration count
  if (useSmoothingBool) {
    float logZn = log(z.x * z.x + z.y * z.y) / 2.0;
    float nu = log(logZn / log(2.0)) / log(2.0);
    iterFloat = float(iter) + 1.0 - nu;
  } else {
    iterFloat = float(iter);
  }

  // Handle points inside the set
  if (iter == maxIter) {
    fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    return;
  }

  // Output color
  vec3 color;

  // Map iterations to palette coordinate
  if (useColorBoostBool) {
    // Normalize iteration count to palette range
    float u = iterFloat / float(maxIter);
    u = fract(u * colorDetail); // Apply colorDetail multiplier and wrap around
    color = texture(uColorPalette, vec2(u, 0.5)).rgb;
  } else {
    // Use discrete indices with interpolation
    int indexVal = int(floor(iterFloat));
    int index1 = indexVal;
    int index2 = min(index1 + 1, paletteSize - 1);

    // Map to texture coord
    float u1 = mod(float(index1) * colorDetail, float(paletteSize)) / float(paletteSize);
    float u2 = mod(float(index2) * colorDetail, float(paletteSize)) / float(paletteSize);

    // Sample colors
    vec3 color1 = texture(uColorPalette, vec2(u1, 0.5)).rgb;
    vec3 color2 = texture(uColorPalette, vec2(u2, 0.5)).rgb;

    float frac = fract(iterFloat);
    color = mix(color1, color2, frac);
  }

  // Output final fragment color
  fragColor = vec4(color, 1.0);
}
