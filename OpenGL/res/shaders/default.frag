#version 460 core
#define PI 3.141592653589793
#define TAU 6.283185307179586

out vec4 FragColor;

in vec3 color;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

uniform vec3 uCameraPos;    // Camera position
uniform vec3 uCameraDir;    // Camera direction (orientation)
uniform vec3 uCameraRight;  // Right vector
uniform vec3 uCameraUp;     // Up vector
uniform float uFOV;         // Field of View
uniform mat4 uCameraMatrix;

const int MAX_STEPS = 256;
const float MAX_DIST = 500;
const float EPSILON = 0.001f;

// HG_SDF.GLSL
// --------------------------------------------

vec3 pMod3(inout vec3 p, vec3 size) {
	vec3 c = floor((p + size*0.5)/size);
	p = mod(p + size*0.5, size) - size*0.5;
	return c;
}

float vmax(vec2 v) {
  return max(v.x, v.y);
}

float vmax(vec3 v) {
  return max(max(v.x, v.y), v.z);
}

float vmax(vec4 v) {
  return max(max(v.x, v.y), max(v.z, v.w));
}

float fSphere(vec3 p, float r) {
	return length(p) - r;
}

float fPlane(vec3 p, vec3 n, float distanceFromOrigin) {
  return dot(p, n) + distanceFromOrigin;
}

float fBox(vec3 p, vec3 b) {
  vec3 d = abs(p) - b;
  return length(max(d, vec3(0))) + vmax(min(d, vec3(0)));
}

void pR(inout vec2 p, float a) {
  p = cos(a) * p + sin(a) * vec2(p.y, -p.x);
}

// --------------------------------------------

vec2 fOpUnionID(vec2 res1, vec2 res2) {
  return (res1.x < res2.x) ? res1 : res2;
}

vec2 fOpDifferenceID(vec2 res1, vec2 res2)
{
  return (res1.x > -res2.x) ? res1 : vec2(-res2.x, res2.y);
}

mat3 rotateX(float angle)
{
  float c = cos(angle);
  float s = sin(angle);
  return mat3(
    1.0, 0.0, 0.0,
    0.0, c, -s,
    0.0, s, c
  );
}

vec2 map(vec3 p)
{
  float angleX = u_time * 0.5;

  mat3 rotationMatrixX = rotateX(angleX);

  // plane
  float planeDist = fPlane(p, vec3(0, 1, 0), 1.0);
  float planeID = 2.0;
  vec2 plane = vec2(planeDist, planeID);

  // sphere
  float sphereDist = fSphere(p, 0.7);
  float sphereID = 1.0f;
  vec2 sphere = vec2(sphereDist, sphereID);

  // box
  float boxDist = fBox(p * rotationMatrixX, vec3(0.5, 0.5, 0.5));
  float boxID = 3.0;
  vec2 box = vec2(boxDist, boxID);

  vec2 res;
  res = fOpUnionID(box, sphere);
  res = fOpUnionID(res, plane);
  return res;
}

vec2 rayMarch(vec3 ro, vec3 rd) {
  vec2 hit, object;
  for (int i = 0; i < MAX_STEPS; ++i)
  {
    vec3 p = ro + object.x * rd;
    hit = map(p);
    object.x += hit.x;
    object.y  = hit.y;
    if (abs(hit.x) < EPSILON || object.x > MAX_DIST) break;
  }
  return object;
}

vec3 getNormal(vec3 p)
{
  vec2 e = vec2(EPSILON, 0.0);
  vec3 n = vec3(map(p).x) - vec3(map(p - e.xyy).x, map(p - e.yxy).x, map(p - e.yyx).x);
  return normalize(n);
}

float mapDist(vec3 p) {
  return map(p).x;
}

float softshadow(in vec3 ro, in vec3 rd, float mint, float maxt, float k)
{
  float res = 2.0;
  float t = mint;
  for (int i = 0; i < 256 && t < maxt; i++) {
    float h = mapDist(ro + t * rd);
    res = min(res, k * h / t);
    t += clamp(h, 0.005, 0.5);
    if (h < EPSILON) break;
  }
  return clamp(res, 0.0, 1.0);
}

vec3 getLight(vec3 p, vec3 rd, vec3 color) {
  vec3 lightPos = vec3(20.0, 40.0, -30.0);
  vec3 L = normalize(lightPos - p);
  vec3 N = getNormal(p);
  vec3 V = -rd;
  vec3 R = reflect(-L, N);

  vec3 specColor = vec3(0.5);
  vec3 specular = specColor * pow(clamp(dot(R, V), 0.0, 1.0), 10.0);
  vec3 diffuse = color * clamp(dot(L, N), 0.0, 1.0);
  vec3 ambient = color * 0.05;
  vec3 fresnel = 0.25 * color * pow(1.0 + dot(rd, N), 3.0);

  float shadow = softshadow(p + N * 0.02, normalize(lightPos - p), 0.02, length(lightPos - p), 16.0);
  return (diffuse * shadow) + ambient + specular + fresnel;
}

vec3 getMaterial(vec3 p, float id)
{
  vec3 m;
  switch (int(id))
  {
  case 1:
    m = vec3(0.9, 0.9, 0.0); break;
  case 2:
    m = vec3(0.2 + 0.4 * mod(floor(p.x) + floor(p.z), 2.0)); break;
  case 3:
    m = vec3(0.7, 0.8, 0.9); break;
  }
  return m;
}

mat3 getCam(vec3 ro, vec3 lookAt)
{
  vec3 camF = normalize(vec3(lookAt - ro));
  vec3 camR = normalize(cross(vec3(0, 1, 0), camF));
  vec3 camU = cross(camF, camR);
  return mat3(camR, camU, camF);
}

void mouseControl(inout vec3 ro)
{
  vec2 m = u_mouse / u_resolution;
  pR(ro.yz, m.y * PI * 0.5 - 0.5);
  pR(ro.xz, m.x * TAU);
}

void render(inout vec3 col, in vec2 uv)
{
  vec3 ro = uCameraPos;

  vec3 rd = normalize(
    uCameraDir +
    uv.x * uCameraRight * tan(radians(uFOV / 2.0)) +
    uv.y * uCameraUp * tan(radians(uFOV / 2.0))
  );

  vec2 object = rayMarch(ro, rd);

  vec3 background = vec3(0.5, 0.8, 0.9);
  if (object.x < MAX_DIST) {
    vec3 p = ro + object.x * rd;
    vec3 material = getMaterial(p, object.y);
    col += getLight(p, rd, material);

    //fog
    col = mix(col, background, 1.0 - exp(-0.0008 * object.x * object.x));
  }
  else {
    col += background - max(0.95 * rd.y, 0.0);
  }
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5f * u_resolution.xy) / u_resolution.y;

    vec3 col = vec3(0.0f);
    
    render(col, uv);
    // gamma correction
    col = pow(col, vec3(0.4545));
    FragColor = vec4(col, 1.0f);
}
