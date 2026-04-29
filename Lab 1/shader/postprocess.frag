#version 460

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D SceneTex;
uniform bool ShowEdge;

//kernals for edge detection
const float offset = 1.0 / 800.0;

vec3 sobelEdge(vec2 uv){
	float x[9] = float[](
	-1, 0, 1,
	-2, 0, 2,
	-1, 0, 1
	);

	float y[9] = float[](
	-1, -2, -1,
	 0,  0,  0,
	 1,  2,  1
	);

vec2 offsets[9] = vec2[](
	vec2(-offset, offset), vec2(0.0, offset), vec2(offset, offset),
	vec2(-offset, 0.0),	vec2(0.0, 0.0), vec2(offset, 0.0),
	vec2(-offset, -offset), vec2(0.0, -offset), vec2(offset, -offset)
	);

	float gx = 0.0;
	float gy = 0.0;

	for(int i = 0; i < 9; i++){
	float intensity = length(texture(SceneTex, uv + offsets[i]).rgb);
	gx += x[i] * intensity;
	gy += y[i] * intensity;
	}
	
	float g = sqrt(gx * gx + gy * gy);
	return vec3(g);
}

void main(){
	vec3 col = texture(SceneTex, TexCoord).rgb;

	//if edge mode is active. override everything
	if(ShowEdges){
		vec3 edge = sobelEdge(TexCoord);
		FragColor = vec4(edges, 1.0);
		return;
	}

	//dreamlike vignette
	float dist = distance(TexCoord, vec2(0.5));
	float vignette = smoothstep(0.8, 0.4, dist);

	//colour shift 
	vec3 tint = vec3(0.9, 0.8, 1.2); //purpish pink 
	vec3 shifted = col * tint;

	//combine
	vec3 finalCol = mix(col, shifted, 0.4); 
	finalCol += vignette;

	FragColor = vec4(finalCol, 1.0);

}