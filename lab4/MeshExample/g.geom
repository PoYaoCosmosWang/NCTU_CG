#version 150 compatibility

uniform int level;
uniform float radius;
//uniform vec4 centroid;



layout(triangles) in;

layout(triangle_strip, max_vertices=85) out;

in Vertex{
	vec3 vertexEyeSpace;
	vec3 normal;    
	vec2 _texcoord;
	vec4 v;
}vertex[];
in mat4 ProjectionMatrix[];
in  vec4 center[];
out vec3 vertexEyeSpace;
out vec3 normal; 
out vec2 _texcoord;

vec4 p0,dp01, dp02;

vec2 t0,dt01, dt02;


float r;
vec4 cent;

void produceV(float s,float t)
{
	
	
	vec3 v= p0.xyz+s*dp01.xyz+t*dp02.xyz;
	v=v-center[0].xyz;
	v=normalize(v);
	normal= v;
	v=v*r+center[0].xyz;
	gl_Position= ProjectionMatrix[0]*vec4(v,1.0);
	vertexEyeSpace=v;
	
	_texcoord= t0 * s*dt01 + t*dt02;
	
	EmitVertex( );
	
}

void main(){
	r=radius;
	int numLayer=1<<level;
	
	dp01 =  gl_in[1].gl_Position - gl_in[0].gl_Position ;
	dp01= dp01 / float (numLayer);
	dp02 = gl_in[2].gl_Position - gl_in[0].gl_Position;
	dp02= dp02/float (numLayer);
	p0 = gl_in[0].gl_Position;
	
	
	
	
	t0= vertex[0]._texcoord;
	dt01=vertex[1]._texcoord -vertex[0]._texcoord;
	dt01= dt01/float (numLayer);
	dt02=vertex[2]._texcoord -vertex[0]._texcoord;
	dt02= dt02/float (numLayer);
	
	
	
	vec2 bot=vec2(0,numLayer-1);
	vec2 top=vec2(0,numLayer);
	
	
	for(int layer=0;layer<numLayer;++layer)
	{
		bot.x=0;
		top.x=0;
		
		for(int i=0;i<layer+1;++i)
		{
			//if(bot.y==0	)
			{
			produceV(bot.x,bot.y);
			produceV(top.x,top.y);
			}
			bot.x++;
			top.x++;
			
		}
		
		produceV(bot.x,bot.y);
		EndPrimitive();
		
		bot.y--;
		top.y--;
	}
	
	
}
