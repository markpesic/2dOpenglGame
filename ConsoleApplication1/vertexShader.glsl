#version 420 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;

out vec4 ourColor;
out vec4 pos;
uniform mat4 projection;
uniform mat4 model;
uniform float time;
uniform int sceltaFS;

void main(){
	gl_Position = projection*model*vec4(aPos, 1.0);
	ourColor = aColor;
	pos = model*vec4(aPos, 1.0);
}