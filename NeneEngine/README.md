## Nene Engine
#### 我们的游戏引擎　(•̀ᴗ•́)و


**增加了Direct3D11的支持，正在整理代码**

- 其实就是用面向对象把OpenGL抽象了一层，方便自己使用而已
- 可以通过创建对象的方法快速(而且高效)地构建第一人称视角的三维场景(见Demo)
- 会在[另一个repo](https://github.com/hellokenlee/OpenGLPractice)用到
- 基于OpenGL 3.3，只使用核心模式(使用GLSL语言)，不接受过期的立即模式(GLbegin(xxx)...GLend())
- 窗口管理基于GLFW,数学库基于glm(OpenGL Mathematics),图片读取基于FreeImage
- (=ﾟωﾟ)ﾉ　看！飞碟！

_ _ _

***仅需要20行的代码就能构建一个简单的三维场景！***


 ![image](https://github.com/hellokenlee/OpenGLPractice/raw/master/textures/demo/demo.png)

对应Demo代码(此处省略了顶点数组)：

```cpp
int main(){
	// 窗口初始化
    GLFWwindow *window = initWindow("Demo",800,600);
    // 着色器程序初始化
    Shader cubeShader("shaders/shader.vs","shaders/cubefrag.frag");
    cubeShader.use();
    glUniform3f(glGetUniformLocation(cubeShader.programID,"objColor"),1.0f,0.5f,0.31f);
    // 物体对象初始化
    Object cube(cubeVertices,36,POSITIONS_TEXTURES,GL_TRIANGLES);
    cube.setCamera(&CameraController::camera);
    cube.setShader(&cubeShader);
    // 绑定控制
	CameraController::bindControl();
    // 显示坐标轴
    CoordinateAxes ca(&CameraController::camera);
    // 主循环
    while(!glfwWindowShouldClose(window)){
		// 轮询IO
        glfwPollEvents();
		// 更新摄像机
        CameraController::update();
        // 设置背景色
		glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		// 绘制坐标轴
        ca.draw();
		// 绘制方块
        cube.draw();
		// 双缓冲交换
        glfwSwapBuffers(window);
    }
    // 清理后续
    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}

```

