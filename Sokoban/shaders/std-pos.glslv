//VERTEX SHADER
#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexUV;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_world;


out vec3 Position_world;
out vec3 Normal_cam;
out vec3 EyeDirection_cam;
out vec3 LightDirection_cam;
out vec3 fragmentColor;
out vec2 UV;

void main()
{
    UV = vertexUV;
    gl_Position = MVP * vec4(vertexPosition, 1);
    fragmentColor =vertexColor; // still passing colors for fun ;)

    Position_world = (M * vec4(vertexPosition,1)).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
    vec3 vertexPosition_cam = ( V * M * vec4(vertexPosition,1)).xyz;
    EyeDirection_cam = vec3(0,0,0) - vertexPosition_cam;

    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    vec3 LightPosition_cam = ( V * vec4(LightPosition_world,1)).xyz;
    LightDirection_cam = LightPosition_cam + EyeDirection_cam;
    // Normal of the the vertex, in camera space
    Normal_cam = ( V * M * vec4(vertexNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

}
