// #version 330
//
// // Pixel coordinates in [0.0, 1.0]
// in vec2 fragTexCoord;
// // uniform float windowWidth, windowHeight;
// // Output color
// out vec4 fragColor;
//
// // Source: https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set
//
// // Config
// // "when the sum of the squares of the real and imaginary parts exceed 4, the point has reached escape."
// const float escapeVal = 4.0;
// const int maxIter = 50;
// // Y scaled to [-1.25, 1.25]
// // const float minY = -1.25;
// // const float maxY = 1.25;
// // X scaled to [-2.5, 1.0]
// // const float minX = -2.5;
// // const float maxX = 1.0;
// // Length to provide correction
// // const float lengthY = maxY - minY;
//
// void main() {
//   // Correction based on the Y axis is applied to keep the window proportions
//   // float correctionX = (windowWidth / windowHeight) * lengthY; // width / height == len(x) / len(y)
//   // f_c(z) = z^2 + c
//   vec2 c = vec2(
//       // mix(minX, maxX, fragTexCoord.x) /** correctionX*/ ,
//       // mix(minY, maxY, (1.0 - fragTexCoord.y)) // y has to be inverted
//       (fragTexCoord.x - 0.5) * 2,
//       (fragTexCoord.y - 0.5) * 2
//     );
//   vec2 z = vec2(0.0);
//   vec2 z2 = vec2(0.0);
//
//   // Optimized escape algorithm
//   int iter = 0;
//   while (z2.x + z2.y <= escapeVal && iter < maxIter) {
//     z2.x = z.x * z.x;
//     z2.y = z.y * z.y;
//     z.y = 2.0 * z.x * z.y + c.y;
//     z.x = z2.x - z2.y + c.x;
//     iter++;
//   }
//
//   float iterVal = float(iter) / float(maxIter);
//   fragColor = vec4(fragTexCoord.x, fragTexCoord.y, 0.0, 1.0);
// }

#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

const float max = 4.0;
const int maxIter = 500;

void main() {
  vec2 pos0 = vec2(
      (fragTexCoord.x - 0.5) * 2,
      (fragTexCoord.y - 0.5) * 2
    );
  vec2 pos = vec2(0.0);
  vec2 pos2 = vec2(0.0);
  int iter = 0;

  while (pos2.x + pos2.y <= max && iter < maxIter) {
    pos2.x = pos.x * pos.x;
    pos2.y = pos.y * pos.y;
    pos.y = 2.0 * pos.x * pos.y + pos0.y;
    pos.x = pos2.x - pos2.y + pos0.x;
    iter++;
  }

  float t = float(iter) / float(maxIter);
  fragColor = vec4(fragTexCoord.x, fragTexCoord.y, 0.0, 1.0);
}
