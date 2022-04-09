#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 depthRange;

const float cPi = 3.141593;

void main()
{
	vec4  viewPos = view * model * vec4( aPos, 1.0 );
    vec2  dirXY   = normalize( vec2( -viewPos.z, viewPos.x ) );
    vec2  dirZ    = normalize( vec2( length(viewPos.xz), viewPos.y ) );
    float posX    = asin( abs( dirXY.y ) ) * 2.0 / cPi;
    float posY    = asin( abs( dirZ.y ) ) * 2.0 / cPi;

    gl_Position = vec4(
            0.5 * sign( dirXY.y ) * mix(2.0-posX, posX, step(0.0, dirXY.x)),
            sign( dirZ.y ) * posY,
            2.0 * (length(viewPos.xyz)-depthRange.x) / (depthRange.y-depthRange.x) - 1.0,
            1.0 );
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}