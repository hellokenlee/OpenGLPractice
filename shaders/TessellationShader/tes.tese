#version 400 core

layout(triangles, equal_spacing, cw) in;

in vec3 tcPosition[];

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main() {
	gl_PointSize = 10.0;
	vec3 interpolatedPosition = interpolate3D(tcPosition[0], tcPosition[1], tcPosition[2]);
	gl_Position = vec4(interpolatedPosition, 1.0);
}