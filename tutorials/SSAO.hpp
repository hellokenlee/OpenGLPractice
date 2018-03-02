/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/
#ifndef SSAO_CPP
#define SSAO_CPP

#include <unistd.h>
#include <random>

// Common Headers
#include "../NeneEngine/OpenGL/Nene.h"

namespace SSAO{

const unsigned int SAMPLES_NUM = 64;

extern GLfloat cubeVertices[6*6*8];

GLfloat screenVertices[6*5]={
    // Positions         // TexCoords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

float lerp(float a, float b, float f){
    return a + f * (b - a);
}
glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.5, 0.5, 0.5);
// SSAO 实现
// Q1： 为什么会产生Banding？
//       因为样本数过少，导致样本不足以覆盖整个半球域。所以相邻的几个像素计算采样后得到的结果是一样的。
// Q2： 随机旋转向量ssaoNoise的意义？
//       在采样数量不变的情况下，为了解决Q1的问题，方法就是相邻像素采取不一样的采样。
//       在不增加uniform的情况下，我们可以对kernel绕(切线空间下的)z轴进行旋转，进而改变取样。
//       由因为kernel的采样是建立在切线空间坐标系下的，因此很不容易地想到把切线空间的切线基更换，就相当于把kernel旋转了
// Q3： 为什么会随机旋转之后会产生花纹？
//       因为随机旋转的texture太小，比如说4x4，通过repeat采样后，(0,0)的采样和(5,5)的采样是一样的。
//       这个时候如果(0,0)的分布和(5,5)的分布相同，那么就会出现花纹。
//       不妨使用一个屏幕大小的随机旋转向量。
// Q4： 为什么要在ViewSpace中叠加kernel坐标以及计算？
//       因为viewspace和worldspace的信息量是等价的，在viewspace中计算可以少传递一个view矩阵进ssao的shader。
// Q5： rangeCheck是干啥的？
//       我们只能和NDC上面的数据比对，因此即使一个片段不是在AO的判断半径中，我们也会把他当成遮挡。
//       想象一个悬浮在房间中心的小球，因为小球四周的片段，离小球很远的房间的墙壁,的深度值也会被算作遮挡
//       ，因为我们没有判断这个像素对应的坐标是否在采样半径内。
void tutorial(){
    // 环境初始化
    GLFWwindow *window = initWindow("SSAO", 800, 600);
    showEnviroment();
    glfwSwapInterval(0);
    CameraController::bindControl(window);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //
    Camera *pCamera = CameraController::getCamera();
    pCamera->moveto(glm::vec3(0.0f, 0.0f, 8.0f));
    //
    CoordinateAxes ca(pCamera);
    FPSCounter fc;
    // 准备G缓冲
    GLuint gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        GLuint gPosition, gNormal, gAlbedoSpec;// 片段位置， 法向量， 漫反射颜色信息
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// 防止边缘Sample重采样
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
        // 深度缓冲
        GLuint gDepth;
        glGenRenderbuffers(1, &gDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, gDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth);
        // MRT绑定多个输出纹理
        GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);
        // 检查是否完整
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            cout<<"Framebuffer not complete!"<<endl;
            exit(-1);
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 着色器们
    Shader objShader("Resources/Shaders/SSAO/object.vert", "Resources/Shaders/SSAO/object.frag");
    Shader screenShader("Resources/Shaders/SSAO/screen.vert", "Resources/Shaders/SSAO/screen.frag");
    Shader ssaoShader("Resources/Shaders/SSAO/ssao.vert", "Resources/Shaders/SSAO/ssao.frag");
    Shader blurShader("Resources/Shaders/SSAO/blur.vert", "Resources/Shaders/SSAO/blur.frag");
    // 屏幕
    Shape screen(screenVertices, 6, POSITIONS_TEXTURES, GL_TRIANGLES);
    // 房间物体
    Shape room(cubeVertices, 36, POSITIONS_NORMALS_TEXTURES, GL_TRIANGLES);
    room.setModelMat(glm::translate(room.getModelMat(), glm::vec3(0.0, 7.0f, 0.0f)));
    room.setModelMat(glm::scale(room.getModelMat(), glm::vec3(7.5f, 7.5f, 7.5f)));
    // 人物模型
    Model nanoMan((GLchar*)"Resources/Meshes/nanosuit/nanosuit.obj");
    glm::mat4 tmpMat;
    tmpMat = glm::translate(tmpMat, glm::vec3(0.0f, 0.0f, 5.0));
    tmpMat = glm::rotate(tmpMat, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    tmpMat = glm::scale(tmpMat, glm::vec3(0.5f));
    nanoMan.setModelMat(tmpMat);
    // 准备SSAO采样器
    // 初始化半球kernel
    uniform_real_distribution<float> randomFloats(0.0, 1.0);
    default_random_engine generator;
    vector<glm::vec3> ssaoKernel;
    for(unsigned int i = 0; i < SAMPLES_NUM; ++i){
        glm::vec3 sample(
            randomFloats(generator) * 2.0 -1.0,
            randomFloats(generator) * 2.0 -1.0,
            randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = (float)i / (float)SAMPLES_NUM;
        scale= lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
    // 随机旋转kernel
    vector<glm::vec3> ssaoNoise;
    for(unsigned int i = 0; i < 16; ++i){
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f
        );
        ssaoNoise.push_back(noise);
    }
    // 生成一个4x4的旋转纹理
    GLuint noiseTexture;
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // SSAO FBO
    unsigned int ssaoFBO;
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        unsigned int ssaoColorBuffer;
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            cout<<"Framebuffer not complete!"<<endl;
            exit(-1);
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 模糊FBO
    GLuint blurFBO, blurColorBuffer;
    glGenFramebuffers(1, &blurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
        glGenTextures(1, &blurColorBuffer);
        glBindTexture(GL_TEXTURE_2D, blurColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorBuffer, 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
                cout<<"Framebuffer not complete!"<<endl;
                exit(-1);
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //
    ControlPanel panel(window);
    char uniformName[128];
    Texture tex0("Resources/Textures/white.png", GL_BGRA, GL_RGBA);
    // 主循环
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        CameraController::update();
        // Deferred Shading : Geometry Pass
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        objShader.use();
        glUniform1f(glGetUniformLocation(objShader.programID, "inverseNormal"), -1.0f);
        tex0.use(0);
        tex0.use(1);
        glUniform1i(glGetUniformLocation(objShader.programID, "texture_diffuse1"), 0);
        glUniform1i(glGetUniformLocation(objShader.programID, "texture_specular1"), 1);
        glCullFace(GL_FRONT);
        room.draw(&objShader, pCamera);
        glUniform1f(glGetUniformLocation(objShader.programID, "inverseNormal"),  1.0f);
        glCullFace(GL_BACK);
        nanoMan.draw(&objShader, pCamera);
        // SSAO Pass
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        ssaoShader.use();
        glUniform1i(glGetUniformLocation(ssaoShader.programID, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(ssaoShader.programID, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(ssaoShader.programID, "texNoise"), 2);
        glUniformMatrix4fv(glGetUniformLocation(ssaoShader.programID, "projection"),
                           1, GL_FALSE, pCamera->getProjectionMatrixVal());
        for(unsigned int i = 0; i < SAMPLES_NUM; ++i){
            sprintf(uniformName, "samples[%d]", i);
            glUniform3f(glGetUniformLocation(ssaoShader.programID, uniformName), ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
        }
        screen.draw(&ssaoShader, pCamera);
        // SSAO Blur Pass
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        blurShader.use();
        screen.draw(&blurShader, pCamera);
        // Deferred Shading : Lighting Pass
        // 绑定纹理
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, blurColorBuffer);
        //glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
         // 把gBuffer中的纹理写入shader
        screenShader.use();
        glUniform1i(glGetUniformLocation(screenShader.programID, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(screenShader.programID, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(screenShader.programID, "gAlbedoSpec"), 2);
        glUniform1i(glGetUniformLocation(screenShader.programID, "ssao"), 3);
        // 需要把光源位置转换到视角空间
        glm::vec3 lightPosViewSpace = glm::vec3(pCamera->projection * glm::vec4(lightPos, 1.0f));
        glUniform3f(glGetUniformLocation(screenShader.programID, "light.position"), lightPosViewSpace.x, lightPosViewSpace.y, lightPosViewSpace.z);
        glUniform3f(glGetUniformLocation(screenShader.programID, "light.color"), lightColor.x, lightColor.y, lightColor.z);
        screen.draw(&screenShader, pCamera);
        //
        panel.draw();
        // Buffer swap
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLfloat cubeVertices[6*6*8] = {
    // back face
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
    // front face
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
    // left face
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
    // right face
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
    // bottom face
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
    // top face
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
     1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
};

};
#endif
