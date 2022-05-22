#version 330 core

in vec2 texCoord;
in vec4 pos;
in vec4 normal;
in vec4 pos_global;

uniform sampler2D texture_sampler;

out vec4 FragColor;

float hypdot(vec4 v1, vec4 v2)
{
	return dot(v1.xyz, v2.xyz) - v1.w * v2.w;
}

void main()
{
    float hypdot_pos_pos = hypdot(pos, pos);

    float depth_factor = 1.0 / sqrt(max(1e-3, -hypdot_pos_pos));
    gl_FragDepth = -pos_global.z / ((pos_global.w * depth_factor) + 1.0) * depth_factor;
    FragColor = vec4(texture(texture_sampler, texCoord).rgb * 0.5, 1.0);
}