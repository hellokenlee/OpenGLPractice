#ifndef UTILS_HPP
#define UTILS_HPP

using namespace std;
//初始化窗口
GLFWwindow* initWindow(const char* window_name,int w,int h,void (*key_callback)(GLFWwindow*,int,int,int,int)=nullptr,void (*mouse_callback)(GLFWwindow*,double,double)=nullptr){
    //初始化GLFW。设置GLFW参数，GL版本3.3，只使用CORE_PROFILE，不能Resize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
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



#endif
