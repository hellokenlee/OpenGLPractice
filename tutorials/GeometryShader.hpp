/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef GEOMETRY_SHADER_HPP
#define GEOMETRY_SHADER_HPP
namespace GeometyShader{
GLfloat vertices[]={
    // Positions         // Colors
     0.5f, -0.5f,// 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f,// 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f,// 0.0f,  0.0f, 1.0f, 0.0f,   // Top Left
     0.5f,  0.5f // 0.0f,  1.0f, 0.0f, 0.0f,   // Top Right
};
const GLchar* gShaderCode=
(const GLchar*)
"#version 330 core\n"
"layout (points) in;\n"
"layout (line_strip, max_vertices=2) out;\n"
"void main(){"
"	gl_Position=gl_in[0].gl_Position+vec4(-0.1,0.0,0.0,0.0);\n"
"	EmitVertex();\n"
"	gl_Position=gl_in[0].gl_Position+vec4(0.1,0.0,0.0,0.0);\n"
"	EmitVertex();\n"
"	EndPrimitive();\n"
"}\n";
void tutorial(){
    GLFWwindow *window=initWindow("GeometryShader",800,600);
    showEnviroment();

    GLchar infoLog[512];


    Shader shaderProgram("shaders/GeometryShader/Shader.vs","shaders/GeometryShader/Shader.frag");
//    GLuint geometryShader=glCreateShader(GL_GEOMETRY_SHADER);
//    glShaderSource(geometryShader,1,&gShaderCode,nullptr);
//    glCompileShader(geometryShader);
//    glGetShaderInfoLog(geometryShader,512,nullptr,infoLog);
//    //cout<<infoLog<<endl;
//
//    glAttachShader(shaderProgram.programID, geometryShader);
//    glLinkProgram(shaderProgram.programID);
//
//    glGetProgramInfoLog(shaderProgram.programID,512,nullptr,infoLog);
    //cout<<infoLog<<endl;

    GLuint VAO,VBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),(GLvoid*)0);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS,0,4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
}


};
#endif  //  GEOMETRY_SHADER_HPP
