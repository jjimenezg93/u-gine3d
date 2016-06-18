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
	if(lightingEnabled == true) {
		vec3 combinedDiffuse = ambient;
		vec3 combinedSpecular = vec3(0.0, 0.0, 0.0);
		vec3 L;
		vec3 N;		
		for (int l = 0; l < MAX_LIGHTS; ++l) {
			if (lightEnabled[l]) {
				vec3 nAux = (normalMatrix * vec4(vnormal, 0)).xyz;
				//N = normalize(normalMatrix * vec4(vnormal, 0)).xyz;
				N = normalize(nAux);
				L = lightPos[l].xyz;
				vec3 vertPos = vec3(modelView * vec4(vpos, 1));
				float attRate = 1.0;
				if (lightPos[l].w != 0.0) {
					L = L - vertPos;
					attRate = 1.0 / (1.0 + lightAtt[l] * length(L));
				}
				L = normalize(L);
				float NdotL = max(0.0, dot(N, L));
				combinedDiffuse += NdotL * lightColor[l] * attRate;
				if (shininess > 0 && NdotL > 0.0) {
					vec3 hAux = normalize(vertPos);
					//vec3 H = L - normalize(modelView * vec4(vpos.x, vpos.y, vpos.z, 1)).xyz;
					vec3 H = L - hAux;
					H = normalize(H);
					float NdotH = max(0.0, dot(N, H));
					combinedSpecular += pow(NdotH, float(shininess)) * attRate;
				}
			}
		}
		vec3 color = (diffuse * combinedDiffuse) + combinedSpecular;
		//color = combinedSpecular;
		fcolor = vec4(color, 1.0);
	} else {
		fcolor = vec4(1.0, 1.0, 1.0, 1.0);
	}

	gl_Position = MVP * vec4(vpos, 1);
	fuv = vuv;
}