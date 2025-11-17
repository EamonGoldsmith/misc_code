#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main()
{
	vec4 bricks = texture(Texture1, TexCoord);
	vec4 face = texture(Texture2, vec2(-TexCoord.x, TexCoord.y));

    FragColor = mix(bricks, face, 0.2);
}