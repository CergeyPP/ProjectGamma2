#version 450 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 ProjectionView;

uniform vec3 viewPos;

out vec3 ViewPos;

out vec4 screenPos;

out vec4 globalPos;

void main(){
	vec4 pos = ProjectionView * Model * vec4(position, 1);
	gl_Position = pos;

	globalPos = Model * vec4(position, 1);

	screenPos = pos;

	ViewPos = viewPos;
}
