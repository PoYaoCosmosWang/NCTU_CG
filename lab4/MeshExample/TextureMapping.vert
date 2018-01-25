#version 150 compatibility

out Vertex{
    vec3 vertexEyeSpace;
	vec3 normal;
   	vec2 _texcoord;
	vec4 v;
}veterx;

/*This is for the one whose GPU is as poor as mine.
If you can use gl_ProjectionMatrix in the geometry shader, you can use it instead.
Then you should delete the next two line.*/
out mat4 ProjectionMatrix;
out vec4 center;


uniform vec4 centroid1;

void main(){
    
	
	veterx._texcoord=gl_MultiTexCoord0.xy;
	veterx.vertexEyeSpace = vec3(gl_ModelViewMatrix * gl_Vertex);
    veterx.normal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position =  gl_ModelViewMatrix * gl_Vertex;
	veterx.v=gl_ModelViewMatrix * gl_Vertex;
	
	
	center= gl_ModelViewMatrix*centroid1;
	
	ProjectionMatrix=gl_ProjectionMatrix;
	
}
