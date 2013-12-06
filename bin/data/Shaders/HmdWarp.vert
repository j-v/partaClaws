uniform vec2 dimensions;
varying vec2 oTexCoord;
 
void main()
{
	oTexCoord = gl_MultiTexCoord0.xy / dimensions;
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}