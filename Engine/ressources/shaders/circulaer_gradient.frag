// light_shader.frag



#define MAX_LIGHTS 255

uniform sampler2D texture; // The texture of the object being rendered
uniform vec2 lightPositions[MAX_LIGHTS]; // An array of positions for up to 2 light sources
uniform float lightRadii[MAX_LIGHTS]; // An array of radii for up to 2 light sources
uniform float lightIntensities[MAX_LIGHTS]; // An array of intensities for up to 2 light sources
uniform vec3 lightColors[MAX_LIGHTS]; // An array of colors for up to 2 light sources
uniform int lightAmount;
uniform vec2 cameraPosition;
uniform float cameraZoom;

void main()
{
    vec2 news = cameraPosition / cameraZoom;
    
    vec2 fragmentPosition = gl_FragCoord.xy;

    fragmentPosition.x += news.x;
    fragmentPosition.y -= news.y;

        
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    vec2 screenSize = vec2(1920, 1080); // Replace with your screen dimensions

    fragmentPosition = (screenSize - fragmentPosition);

    for(int i = 0; i < 1; i++)
    {
        vec2 lightPosition = lightPositions[i];
        float lightRadius = lightRadii[i] / cameraZoom;
        float lightIntensity = lightIntensities[i];
        vec3 lightColor = lightColors[i];

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