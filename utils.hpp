#ifndef UTILS_HPP
#define UTILS_HPP

using namespace std;

//FPS计数器
class FPSCounter{
public:
    FPSCounter(){
        init();
    }
    void init(){
        lastTime=currentTime=glfwGetTime();
        frameCounter=0;
    }
    //Call in main loop.
    void update(){
        deltaTime=glfwGetTime()-currentTime;
        currentTime=glfwGetTime();
        ++frameCounter;
        if(currentTime-lastTime>=1.0){
            printf("%f ms/frame\n",1000.0/double(frameCounter));
            frameCounter=0;
            lastTime+=1.0;
        }
    }
public:
    double lastTime,currentTime,deltaTime;//上次输出FPS的时间，当前时间，最近2帧的间隔时间
    int frameCounter;
};


//初始化窗口
GLFWwindow* initWindow(const char* window_name,int w,int h,void (*key_callback)(GLFWwindow*,int,int,int,int)=nullptr,void (*mouse_callback)(GLFWwindow*,double,double)=nullptr){
    //初始化GLFW。设置GLFW参数，GL版本3.3，只使用CORE_PROFILE，不能Resize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //创建窗口
    GLFWwindow *window = glfwCreateWindow(w,h,window_name,nullptr,nullptr);
    //获取显示器大小
    const GLFWvidmode *screen;
    screen=glfwGetVideoMode(glfwGetPrimaryMonitor());
    //设置窗口在中间
    glfwSetWindowPos(window,(screen->width-w)/2,(screen->height-h)/2);
    //检查是否成功
    if(window == nullptr){
        cout<<"ERROR: Fail to create GLFW window"<<endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    //绑定按键回调函数
    glfwSetKeyCallback(window,key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //初始化GLEW。 使用Experimental函数允许我们更好的使用core_profile
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        cout<<"ERROR: Fail to init GLEW"<<endl;
        exit(-1);
    }
    glGetError();//忽略由glew引起的INVALID_ENUM错误
    //获取当前窗口大小，设置GLVIewport
    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);
    return window;
}



//输出OpenGL,GLEW,GLFW版本信息
void showEnviroment(){
    fprintf(stdout,"Working at:\n\t%s\n",glGetString(GL_RENDERER));
    fprintf(stdout,"Using Libraries:\n\tOpenGL %s\n",glGetString(GL_VERSION));
    fprintf(stdout,"\tGLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout,"\tGLFW %s\n",glfwGetVersionString());
}

//获取GL状态机错误
void checkError(){
    cout<<"ERROR Check: "<<gluErrorString(glGetError())<<endl;
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



#endif
