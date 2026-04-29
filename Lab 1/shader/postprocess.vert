#version 460

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 TexCoordIn;

out vec2 TexCoord;

void main(){
	TexCoord = TexCoordIn;
	gl_Position = vec4(Position, 0.0, 1.0);
}