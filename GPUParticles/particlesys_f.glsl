#version 330

in vec2 UV;

uniform sampler2D Pos;
uniform sampler2D Velo;
uniform sampler1D Emit;
uniform sampler1D EmitColor;

uniform float TimeStep;
uniform float RandSeed;

uniform int EmitterCount;

uniform mat4 MVI;

uniform int ParticleSysSize=1024;

layout(location=0) out vec4 oPos;
layout(location=1) out vec4 oVelo;
layout(location=2) out vec4 oColor;

const vec3 Gravity=vec3(0.0, -9.81, 0.0);

const float TwoPi=6.28318;
const float SeedRadius=30.0;

uint hash(uint x)
{
	x+=(x<<10u);
	x^=(x>>6u);
	x+=(x<<3u);
	x^=(x>>11u);
	x+=(x<<15u);

	return x;
}

float random(float v)
{
    return uintBitsToFloat(hash(floatBitsToUint(v))&0x007FFFFFu|0x3F800000u)-1.0;
}

void main()
{
	// Fragment position integration over the 2D area
	float i=(UV.y*ParticleSysSize)*ParticleSysSize+(UV.x*ParticleSysSize);
	// Input position and velocity textures
	vec4 Position=texture(Pos, UV);
	vec4 Velocity=texture(Velo, UV);
	vec4 Emitter=texelFetch(Emit, int(i)%EmitterCount, 0);
	vec4 EmitterColor=texelFetch(EmitColor, int(i)%EmitterCount, 0);

	oColor=EmitterColor;

	// Reduce the particle's life.
	// Multiplier on the time step will change how fast particles will cycle,
	// higher multiplier = shorter life = faster cycle.
	// However, the higher the multiplier, the higher FPS will be needed, otherwise it does weird things.
	oPos.w=Position.w-TimeStep*0.75;

	// If it's dead, crank out a new one!
	if(Position.w<0.0)
	{
		float theta=random(i+RandSeed+1)*TwoPi;
		float r=random(i+RandSeed+2)*SeedRadius;

		oPos=vec4(
			Emitter.x*300.0,
			Emitter.y*300.0,
			Emitter.z*300.0,
			
			random(i+RandSeed)*0.999+0.001); // Random life
		oVelo=MVI*vec4(r*sin(theta), random(i+RandSeed+3)*100.0, r*cos(theta), 0.0); // Random cylindrical velocity on x/z, random straight pull on y
	}
	// Otherwise apply velocity/gravity and update position.
	else
	{
		oPos.xyz=Position.xyz+Velocity.xyz*TimeStep;
		oVelo=Velocity+(MVI*vec4(Gravity, 0.0))*TimeStep;
	}
}
