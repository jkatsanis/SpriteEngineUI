// light_shader.frag

uniform sampler2D texture; // The texture of the object being rendered
uniform vec2 lightPositions[2]; // An array of positions for up to 2 light sources
uniform float lightRadii[2]; // An array of radii for up to 2 light sources
uniform float lightIntensities[2]; // An array of intensities for up to 2 light sources
uniform vec3 lightColors[2]; // An array of colors for up to 2 light sources

void main()
{
    vec2 fragmentPosition = gl_FragCoord.xy;
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < 2; i++) {

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
