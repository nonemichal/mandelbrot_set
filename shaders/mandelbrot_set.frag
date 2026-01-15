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
  fragColor = vec4(t, t * t, t * t * t, 1.0);
}
