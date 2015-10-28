#version 330

uniform sampler2D texmap0;
uniform sampler2D texmap1;

uniform int texMode;

out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

const int numberOfLights = 3;

uniform struct Light {
	vec4 l_pos;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec4 spotDirection;
	bool isActive;
} lights[numberOfLights];

uniform Materials mat;


//uniform Light lights[numberOfLights];
//uniform Light light0;

in Data {
	vec4 pos;
	vec3 normal;
	vec3 eye;
	//vec3 lightDir;
	vec2 tex_coord;
} DataIn;

void main() {

	//lights[0] = light0;
	
	//vec3 l = normalize(DataIn.lightDir);
	vec3 n = normalize(DataIn.normal);
	vec3 e = normalize(DataIn.eye);
	vec3 l;
	float attenuation;

	// Initialize total lighting with ambient lighting;
	//vec4 totalLighting = mat.ambient;
	vec4 totalDiffuse = vec4(0.0);
	vec4 totalSpecular = vec4(0.0);

	// For all light sources
	for(int i = 0; i < numberOfLights; i++) {
		
		if(lights[i].isActive) {
			if(lights[i].l_pos.w == 0.0) // Directional light?
			{
				attenuation = 1.0;
				l = normalize(vec3(lights[i].l_pos));
			}
			else // Pointlight or Spotlight
			{
				vec3 l_dir = vec3(lights[i].l_pos - DataIn.pos);
				float distance = length(l_dir);
				l = normalize(l_dir);

				attenuation = 1.0 / 
				(lights[i].constantAttenuation +
				 (lights[i].linearAttenuation * distance) +
				 (lights[i].quadraticAttenuation * distance * distance));

				if(lights[i].spotCutoff < 90.0) // Spotlight?
				{
					vec3 sd = normalize(vec3(-lights[i].spotDirection));
					float spotCosine = max(dot(sd, l), 0.0);
					if(spotCosine >= cos(radians(lights[i].spotCutoff))) {
						attenuation = attenuation * pow(spotCosine, lights[i].spotExponent);
					}
					else
					{
						attenuation = 0.0;
					}
				}
			}

			float intensity = max(dot(n,l), 0.0);
			vec4 diff = lights[i].diffuse * intensity;
			vec4 spec = vec4(0.0);
		
			if (intensity > 0.0) {
				vec3 h = normalize(l + e);
				float intSpec = max(dot(h,n), 0.0);
				spec = lights[i].specular * pow(intSpec, mat.shininess);
			}

			totalDiffuse = totalDiffuse + attenuation * diff;
			totalSpecular = totalSpecular + attenuation * spec;
			//totalLighting = totalLighting + attenuation * (diff + spec);
		}
	}

	vec4 texel;
	vec4 texel1;

	if(texMode == 0) // No textures
	{
		colorOut = max(totalDiffuse * mat.diffuse + totalSpecular * mat.specular, mat.ambient);
		//totalDiffuse = totalDiffuse + mat.ambient;
		//colorOut = min(totalDiffuse * mat.diffuse + totalSpecular * mat.specular, vec4(1.0));
	}
	else if (texMode == 1) // Modulate diffuse color with texel color
	{
		texel = texture(texmap0, DataIn.tex_coord);
		colorOut = max(totalDiffuse * mat.diffuse * texel + totalSpecular * mat.specular, mat.ambient * texel);
	}
	else if (texMode == 2) // Diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
	{
		texel = texture(texmap0, DataIn.tex_coord);
		colorOut = max(totalDiffuse * texel + totalSpecular * mat.specular, 0.1*texel);
	}
	else {
		texel = texture(texmap0, DataIn.tex_coord);
		texel1 = texture(texmap1, DataIn.tex_coord);
		colorOut = max(totalDiffuse * texel * texel1 + totalSpecular * mat.specular, 0.1 * texel * texel1);
	}
}
