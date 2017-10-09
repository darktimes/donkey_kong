#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform vec2 scale;
uniform float time;

void main()
{

	float alpha = 1.0f;

	vec2 reversedTexCoord = vec2(TexCoord.x * scale.x, (1.0f - TexCoord.y) * scale.y);
	vec4 textColor = texture(texture1, reversedTexCoord);
	float upTreshhold = 0.4f;
	if (textColor.r >= upTreshhold && textColor.g >= upTreshhold && textColor.b >= upTreshhold) {
		alpha = 0.0f;
	}


	FragColor = vec4(texture(texture1, reversedTexCoord).xyz, alpha);
}
