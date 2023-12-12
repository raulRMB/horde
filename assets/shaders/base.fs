#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
//in vec4 fragColor;
in vec3 fragNormal;

uniform sampler2D texture0;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    gl_FragColor = texelColor;
}
