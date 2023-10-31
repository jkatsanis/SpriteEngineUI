// light_shader.frag

#define MAX_LIGHTS 255

uniform sampler2D texture; // The texture of the object being rendered
uniform vec2 lightPositions[MAX_LIGHTS]; // An array of positions for up to 2 light sources
uniform float lightRadii[MAX_LIGHTS]; // An array of radii for up to 2 light sources
uniform float lightIntensities[MAX_LIGHTS]; // An array of intensities for up to 2 light sources
uniform vec3 lightColors[MAX_LIGHTS]; // An array of colors for up to 2 light sources
uniform int lightAmount;

void main()
{
    vec2 fragmentPosition = gl_FragCoord.xy;
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < lightAmount; i++) {

        vec2 lightPosition = lightPositions[i];
        float lightRadius = lightRadii[i];
        float lightIntensity = lightIntensities[i];
        vec3 lightColor = lightColors[i];

        float distanceToLight = length(fragmentPosition - lightPosition);
        float intensity = 1.0 - smoothstep(0.0, lightRadius, distanceToLight);
        intensity *= lightIntensity;
        intensity = clamp(intensity, 0.0, 1.0);

        vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
        finalColor += texColor * vec4(lightColor, 1.0) * intensity;
    }

    gl_FragColor = finalColor;
}
