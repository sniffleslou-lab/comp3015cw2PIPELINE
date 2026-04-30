#version 460

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D SceneTex;
uniform int PostMode;

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

//gussian
vec3 gaussianBlur(vec2 uv){
	float o = 1.0 / 800.0;

	vec2 offsets[9] = vec2[](
		vec2(-o, o), vec2(0.0, o), vec2(o, o),
		vec2(-o, 0.0), vec2(0.0, 0.0), vec2(o, 0.0),
		vec2(-o, -o), vec2(0.0, -o), vec2(o, -o)
	);

	float kernel[9] = float[](
		1,2,1,
		2,4,2,
		1,2,1
	);

	float norm = 16.0;
	vec3 sum = vec3(0.0);

	for(int i = 0; i < 9; i++)
		sum += texture(SceneTex, uv + offsets[i]).rgb * kernel[i];

	return sum/norm;
}

void main(){
	vec3 col = texture(SceneTex, TexCoord).rgb;


	//if edge mode is active. override everything
	if(PostMode == 1){
		vec3 edges = sobelEdge(TexCoord);
		FragColor = vec4(edges, 1.0);
		return;
	}
	
	//grayscale
	if(PostMode == 2){
		float g = dot(col, vec3(0.299, 0.587, 0.114));
		FragColor = vec4(vec3(g), 1.0);
		return;
	}

	//gussian blur
	if(PostMode == 3){
		vec3 blurred = gaussianBlur(TexCoord);
		FragColor = vec4(blurred, 1.0);
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
	finalCol *= vignette;

	FragColor = vec4(finalCol, 1.0);

}