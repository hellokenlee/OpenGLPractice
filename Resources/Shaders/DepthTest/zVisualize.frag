#version 330 core
out vec4 color;

float near = 1.0; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    float depth = (LinearizeDepth(gl_FragCoord.z) -near)/ (far-near); // divide by far for demonstration
    color = vec4(vec3(depth), 1.0f);
}