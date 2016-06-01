#define MAX_LIGHTS 8
uniform mat4 MVP;
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform bool lightingEnabled;
uniform bool lightEnabled[MAX_LIGHTS];
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform float lightAtt[MAX_LIGHTS];
uniform vec3 diffuse;
uniform vec3 ambient;
uniform int shininess;
uniform bool usesTexture;
attribute vec3 vpos;
attribute vec2 vuv;
attribute vec3 vnormal;
varying vec2 fuv;
varying vec4 fcolor;

void main() {
	//gl_Position = lightPos[0]; //debug
	//gl_Position = vec4(lightColor[0], 1);
	gl_Position = MVP * vec4(vpos, 1);
	fuv = vuv;
	
	if(lightingEnabled == true) {
		vec3 combinedDiffuse = ambient;
		vec3 combinedSpecular = vec3(0.0, 0.0, 0.0);
		
		for (int l = 0; l < MAX_LIGHTS; ++l) {
			if (lightEnabled[l] == true) {
				vec3 N = normalize(normalMatrix * vec4(vnormal.x, vnormal.y, vnormal.z, 0)).xyz;
				vec3 L = lightPos[l].xyz;
				float attRate = 1.0;
				if (lightPos[l].w == 1.0) {
					vec3 dist = L - (modelView * vec4(vpos.x, vpos.y, vpos.z, 1)).xyz;
					attRate = 1.0 / (lightAtt[l] * length(dist));
				}
				float NdotL = max(0.0, dot(N, normalize(L)));
				combinedDiffuse += NdotL * lightColor[l] * attRate;
				if (shininess > 0 && NdotL > 0.0) {
					vec3 H = -normalize(modelView * vec4(vpos.x, vpos.y, vpos.z, 1)).xyz - L;
					float NdotH = max(0.0, dot(N, H));
					combinedSpecular += pow(NdotH, float(shininess)) * attRate;
				}
			}
		}
		vec3 color = ambient + (combinedDiffuse + combinedSpecular);
		//vec3 color = diffuse;
		fcolor = vec4(color.x, color.y, color.z, 1.0);
	}
}