#ifndef UTILS_HPP
#define UTILS_HPP

using namespace std;

//向 stdout 输出OpenGL,GLEW,GLFW版本信息
void showEnviroment(){
    fprintf(stdout,"Working at:\n\t%s\n",glGetString(GL_RENDERER));
    fprintf(stdout,"Using Libraries:\n\tOpenGL %s\n",glGetString(GL_VERSION));
    fprintf(stdout,"\tGLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout,"\tGLFW %s\n",glfwGetVersionString());
}

//检查GLSL编译情况
void checkShaderCompile(GLuint shaderID){
    GLint isSuccess;
    glGetShaderiv(shaderID,GL_COMPILE_STATUS,&isSuccess);
    if(!isSuccess){
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID,512,nullptr,infoLog);
        cout<<"ERROR: Comlile Shader Error:"<<endl<<infoLog<<endl;
        exit(-1);
    }
}

//检查多Shader链接情况
void checkProgramLink(GLuint programID){
    GLint isSuccess;
    glGetProgramiv(programID,GL_LINK_STATUS,&isSuccess);
    if(!isSuccess){
        GLchar infoLog[512];
        glGetProgramInfoLog(programID,512,nullptr,infoLog);
        cout<<"ERROR: Link Program Error:"<<endl<<infoLog<<endl;
        exit(-1);
    }
}

//初始化窗口
GLFWwindow* initWindow(const char* window_name,int w,int h){
    //初始化GLFW。设置GLFW参数，GL版本3.3，只使用CORE_PROFILE，不能Resize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //创建窗口
    GLFWwindow *window = glfwCreateWindow(w,h,window_name,nullptr,nullptr);
    //检查是否成功
    if(window == nullptr){
        cout<<"ERROR: Fail to create GLFW window"<<endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    //初始化GLEW。 使用Experimental函数允许我们更好的使用core_profile
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        cout<<"ERROR: Fail to init GLEW"<<endl;
        exit(-1);
    }
    //获取当前窗口大小，设置GLVIewport
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    return window;
}


#endif
