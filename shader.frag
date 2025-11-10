#version 460 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec3 u_camera_pos;
uniform mat3 u_camera_rot;

#define MAX_STEPS 200
#define MAX_DIST 100.0
#define MARCH_STEP 0.08
#define SURFACE_DIST 0.001

float sdSphere(vec3 p, float radius) {
  return length(p) - radius;
}

float scene(vec3 p) {
  return -sdSphere(p, 1.0);
}

vec4 raymarch(vec3 ro, vec3 rd) {
  float depth = 0.0;
  vec3 p = ro + depth * rd;

  vec4 res = vec4(0.0);
  for (int i = 0; i < MAX_STEPS; i++) {
    float density = scene(p);
    if (density > 0.0) {
       
    }
    depth += MARCH_STEP;
    p = ro + depth * rd;
  }
  return res;
}

void main() {
  vec2 uv = gl_FragCoord.xy/u_resolution.xy - vec2(0.5);

  vec3 ro = u_camera_pos;
  vec3 rd = normalize(u_camera_rot * vec3(-uv.x, -uv.y, -1.0));

  vec4 color = raymarch(ro, rd);
  FragColor = vec4(color.rgb, 1.0);
}
