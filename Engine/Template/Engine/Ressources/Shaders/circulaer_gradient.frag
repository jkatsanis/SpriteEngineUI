#define MAX_LIGHTS 255

uniform sampler2D texture;
uniform vec2 lightPositions[MAX_LIGHTS];
uniform float lightRadii[MAX_LIGHTS];
uniform float lightIntensities[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform int lightAmount;
uniform vec2 cameraPosition;
uniform float cameraZoom;
uniform vec2 screenSize;

void main()
{
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    vec2 fragScreen = gl_FragCoord.xy;
    fragScreen.y = screenSize.y - fragScreen.y; // Flip Y because gl_FragCoord is bottom left

    // convert to world space
    vec2 screenCenter = screenSize / 2.0;

    vec2 fragWorld;
    fragWorld.x = (fragScreen.x - screenCenter.x) * cameraZoom + cameraPosition.x;

    // y is inverted
    fragWorld.y = (screenCenter.y - fragScreen.y) * cameraZoom - cameraPosition.y;

    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;


    // Early discard for transparent pixels
    if (texColor.a < 0.1)
    {
        discard;
    }

    if (lightAmount <= 0)
    {
        gl_FragColor = finalColor;
        return;
    }

    for (int i = 0; i < lightAmount; i++)
    {
        vec2 lightPos = lightPositions[i];
        float lightRadius = lightRadii[i];

        float distanceToLight = length(fragWorld - lightPos);

        float intensity = 1.0 - smoothstep(0.0, lightRadius, distanceToLight);
        intensity *= lightIntensities[i];
        intensity = clamp(intensity, 0.0, 1.0);

        finalColor += texColor * 2.0 * (vec4(lightColors[i], 1.0) * intensity);
    }

    gl_FragColor = finalColor;
}
