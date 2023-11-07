#define MAX_LIGHTS 255

uniform sampler2D texture;
uniform vec2 lightPositions[MAX_LIGHTS]; 
uniform float lightRadii[MAX_LIGHTS]; 
uniform float lightIntensities[MAX_LIGHTS]; 
uniform vec3 lightColors[MAX_LIGHTS];
uniform int lightAmount;
uniform vec2 cameraPosition;
uniform float cameraZoom;

void main()
{          
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    vec2 screenSize = vec2(1920, 1080); // Replace with your screen dimensions

    vec2 fragmentPosition = gl_FragCoord.xy;       
    fragmentPosition.y = screenSize.y - fragmentPosition.y; // Flip the y-coordinate

    vec2 adjustedCameraPosition = cameraPosition / cameraZoom; // Adjusted camera position

    fragmentPosition += adjustedCameraPosition;

    for(int i = 0; i < lightAmount; i++)
    {
        vec2 lightPosition = (lightPositions[i] - adjustedCameraPosition) / cameraZoom; // Adjusted light position
        float lightRadius = lightRadii[i] / cameraZoom;
        float lightIntensity = lightIntensities[i];
        vec3 lightColor = lightColors[i];
   
       lightPosition += adjustedCameraPosition / cameraZoom;

        float distanceToLight = length(fragmentPosition - lightPosition);

        float intensity = 1.0 - smoothstep(0.0, lightRadius, distanceToLight);
        intensity *= lightIntensity;
        intensity = clamp(intensity, 0.0, 1.0);

        vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
        finalColor += texColor * vec4(lightColor, 1.0) * intensity;
    
        if (texColor.a <= 0.01) {
            finalColor = texColor;
        }
    }   

    gl_FragColor = finalColor;
}