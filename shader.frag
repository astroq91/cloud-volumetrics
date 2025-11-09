#version 460 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform vec3 u_camera_pos;
uniform mat3 u_camera_rot;

#define MAX_STEPS 50
#define MAX_DIST 100.0
#define SURFACE_DIST 0.001

float sdSphere(vec3 p, float radius) {
  return length(p) - radius;
}

float scene(vec3 p) {
  return sdSphere(p, 1.0);
}

float raymarch(vec3 ro, vec3 rd) {
  float d0 = 0.0;
  vec3 color = vec3(0.0);

  for (int i = 0; i < MAX_STEPS; i++) {
    vec3 p = ro + rd * d0;
    float dS = scene(p);

    d0 += dS;

    if (d0 > MAX_DIST || dS < SURFACE_DIST) {
      break;
    }
  }
  return d0;
}

void main() {
  vec2 uv = gl_FragCoord.xy/u_resolution.xy - vec2(0.5);
  FragColor = vec4(uv, 1.0, 1.0); 

  vec3 ro = u_camera_pos;
  vec3 rd = normalize(u_camera_rot * vec3(-uv.x, -uv.y, -1.0));

  float d = raymarch(ro, rd);
  vec3 p = ro + rd * d;

  vec3 color = vec3(0.0);
  if (d < MAX_DIST) {
    color = vec3(max(dot(p, vec3(0.0, 0.0, 1.0)) - 0.2, 0.0));
  }

  FragColor = vec4(color, 1.0);
}
