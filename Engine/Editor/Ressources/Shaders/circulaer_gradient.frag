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

    // 1. Get Fragment Screen Position (Pixels, Top-Left Origin)
    vec2 fragScreen = gl_FragCoord.xy;
    fragScreen.y = screenSize.y - fragScreen.y; // Flip Y because gl_FragCoord is Bottom-Left

    // 2. Convert to World Space
    vec2 screenCenter = screenSize / 2.0;

    vec2 fragWorld;
    // Multiply by cameraZoom because cameraZoom scales the View Size (Zoom Out)
    // Zoom 2.0 = View is 2x larger = 1 Pixel covers 2 World Units
    fragWorld.x = (fragScreen.x - screenCenter.x) * cameraZoom + cameraPosition.x;

    // Y is inverted because Camera.Position.Y stores the inverted World Y (Screen Offset)
    fragWorld.y = (screenCenter.y - fragScreen.y) * cameraZoom - cameraPosition.y;

    // 3. Sample Texture
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;

    // Early discard
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
        // Light Position is already in World Space
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
