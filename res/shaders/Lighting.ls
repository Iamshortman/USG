struct BaseLight
{
    vec3 color;
    float intensity;
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct PointLight
{
    BaseLight base;
    vec3 atten;
	vec3 position;
    float range;
};

struct SpotLight
{
    PointLight point;
    vec3 direction;
    float cutoff;
};

vec4 CalcLight(BaseLight base, vec3 direction, vec3 normal, vec3 worldPos)
{
    float diffuseFactor = dot(normal, -direction);
    
    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    if(diffuseFactor > 0.0)
    {
        diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
        
		vec3 eyePos = vec3(0.0, 0.0, 0.0); //EyePos is always the origin
        vec3 directionToEye = normalize(eyePos - worldPos);
        //vec3 reflectDirection = normalize(reflect(direction, normal));
        vec3 halfDirection = normalize(directionToEye - direction);
        
        float specularFactor = dot(halfDirection, normal);
        //specularFactor = pow(specularFactor, specularPower);
		specularFactor = pow(specularFactor, 96.078431);
        
        if(specularFactor > 0.0)
        {
            //specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
			specularColor = vec4(base.color, 1.0) * 0.5 * specularFactor;
        }
    }
    
    return diffuseColor + specularColor;
}


vec4 CalcDirectionalLight(DirectionalLight directional, vec3 normal, vec3 worldPos)
{
    return CalcLight(directional.base, -directional.direction, normal, worldPos);
}

vec4 CalcPointLight(PointLight point, vec3 normal, vec3 worldPos)
{
    vec3 lightDirection = worldPos - point.position;
    float distanceToPoint = length(lightDirection);
    
    if(distanceToPoint > point.range)
        return vec4(0.0, 0.0, 0.0, 0.0);
    
    lightDirection = normalize(lightDirection);
    
    vec4 color = CalcLight(point.base, lightDirection, normal, worldPos);
    
    float attenuation = point.atten.x + //Constant
                         point.atten.y * distanceToPoint + //Linear
                         point.atten.z * distanceToPoint * distanceToPoint + //Quadratic
                         0.0001;
                         
    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 worldPos)
{
    vec3 lightDirection = normalize(worldPos - spotLight.point.position);
    float spotFactor = dot(lightDirection, spotLight.direction);
    
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    
    if(spotFactor > spotLight.cutoff)
    {
        color = CalcPointLight(spotLight.point, normal, worldPos) *
                (1.0 - (1.0 - spotFactor)/(1.0 - spotLight.cutoff));
    }
    
    return color;
}

float CalcShadow(sampler2D shadow_map, vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = ((fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5) + 0.5;

	float currentDepth = projCoords.z;
	float closestDepth = texture2D(shadow_map, projCoords.xy).r;
	
	float bias = 0.005;
	float shadow = currentDepth - bias > closestDepth  ? 0.0 : 1.0;  

    return shadow;
}  