//FRAGMENT SHADER
#version 330 core
layout(location = 0) out vec3 color;

in vec3 fragmentColor;
in vec2 UV;
in vec3 Position_world;
in vec3 Normal_cam;
in vec3 EyeDirection_cam;
in vec3 LightDirection_cam;

uniform sampler2D textureImg;
uniform vec3 LightPosition_world;

void main()
{
    // Light emission properties, TODO make this a uniform
    vec3 LightColor = vec3(1,1,1);
    float LightPower = 900.0f;

    // Material properties
    vec3 MaterialDiffuseColor = texture( textureImg, UV ).rgb * fragmentColor;
    vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

    float distance = length( LightPosition_world - Position_world );

    vec3 n = normalize( Normal_cam );

    vec3 l = normalize( LightDirection_cam );
    
    
    // Cosine of the angle between the normal and the light direction, 
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp( dot( n,l ), 0,1 );
    
    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_cam);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    
    color = 
    // Ambient : simulates indirect lighting
        MaterialAmbientColor +
        // Diffuse : "color" of the object
        MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
        // Specular : reflective highlight, like a mirror
        MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}
