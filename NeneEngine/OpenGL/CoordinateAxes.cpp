/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#include "CoordinateAxes.h"

//坐标轴顶点着色器GLSL
const char vShaderSource[] =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "void main(){\n"
    "	gl_Position = proj * view * vec4(position, 1.0);\n"
    "	vertexColor = color;\n"
    "}\n";
//坐标轴颜色着色器GLSL
const char fShaderSource[] =
    "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 color;\n"
    "void main(){\n"
    "	color = vec4(vertexColor, 1.0f);\n"
    "}\n";

//网格顶点着色器
const char vShaderSource2[] =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "void main(){\n"
    "	gl_Position = proj * view * vec4(position, 1.0);\n"
    "}\n";
//网格颜色着色器
const char fShaderSource2[] =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main(){\n"
    "	color = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
    "}\n";

//构造函数
CoordinateAxes::CoordinateAxes(Camera *cam) {
    //坐标轴初始长度
    len = 512.0;
    grid = true;
    axes = true;
    gap = 1;
    //摄像机赋值
    this->cam = cam;
    //初始化顶点数组
    GLfloat vertices[36];
    for(int i = 0; i < 36; ++i) {
        if(i == 6 || i == 19 || i == 32) {
            vertices[i] = len;
        } else if(i == 3 || i == 9 || i == 16 || i == 22 || i == 29 || i == 35) {
            vertices[i] = 1.0f;
        } else {
            vertices[i] = 0.0f;
        }
    }
    GLfloat vertices2[(int)((len / gap + 1) * 2 * 2 * 2 * 3)];
    int counter = 0;
    //平行z轴
    float x = 0;
    while(x < len) {
        vertices2[counter * 3 + 0] = x;
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = (x == 0 ? 0 : len);
        counter++;
        vertices2[counter * 3 + 0] = x;
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = -len;
        counter++;
        vertices2[counter * 3 + 0] = -x;
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = (x == 0 ? 0 : len);
        counter++;
        vertices2[counter * 3 + 0] = -x;
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = -len;
        counter++;
        x += gap;
    }
    //平行与x轴
    float z = 0;
    while(z < len) {
        vertices2[counter * 3 + 0] = (z == 0 ? 0 : len);
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = z;
        counter++;
        vertices2[counter * 3 + 0] = -len;
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = z;
        counter++;
        vertices2[counter * 3 + 0] = (z == 0 ? 0 : len);
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = -z;
        counter++;
        vertices2[counter * 3 + 0] = -len;
        vertices2[counter * 3 + 1] = 0;
        vertices2[counter * 3 + 2] = -z;
        counter++;
        z += gap;
    }
    gridVertexNums = counter;
    //建立Shader
    axeShader = Shader::createShaderFromSource(vShaderSource, fShaderSource);
    gridShader = Shader::createShaderFromSource(vShaderSource2, fShaderSource2);
    //生成坐标轴对象
    glGenBuffers(1, &axeVBO);
    glGenVertexArrays(1, &axeVAO);
    glBindVertexArray(axeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axeVBO);
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);           //pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //color
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glGenBuffers(1, &gridVBO);
    glGenVertexArrays(1, &gridVAO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * counter * sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);           //pos
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //获取uniform位置
    axeViewPos = glGetUniformLocation(axeShader->programID, "view");
    axeProjPos = glGetUniformLocation(axeShader->programID, "proj");
    gridViewPos = glGetUniformLocation(gridShader->programID, "view");
    gridProjPos = glGetUniformLocation(gridShader->programID, "proj");
}
//析构函数
CoordinateAxes::~CoordinateAxes() {
    cam = nullptr;
    glDeleteVertexArrays(1, &axeVAO);
    glDeleteBuffers(1, &axeVBO);
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
}
//绘制坐标轴
void CoordinateAxes::draw() {
    // 绘制网格
    if(grid) {
        gridShader->use();
        glUniformMatrix4fv(gridViewPos, 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(gridProjPos, 1, GL_FALSE, cam->getProjectionMatrixVal());
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridVertexNums);
    }
    if(axes) {
        // 绘制坐标轴
        axeShader->use();
        glUniformMatrix4fv(axeViewPos, 1, GL_FALSE, cam->getViewMatrixVal());
        glUniformMatrix4fv(axeProjPos, 1, GL_FALSE, cam->getProjectionMatrixVal());
        glBindVertexArray(axeVAO);
        glDrawArrays(GL_LINES, 0, 6);
    }
    // 恢复上下文
    //glBindVertexArray(0);
    //glUseProgram(0);
}
//更新坐标轴最大长度
void CoordinateAxes::extend() {
    len *= 2.0;
    //更新显存数据
    glBindVertexArray(axeVAO);
    GLfloat *axeVBOvertices;
    axeVBOvertices = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    axeVBOvertices[6] = len;
    axeVBOvertices[19] = len;
    axeVBOvertices[32] = len;
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(0);
}

void CoordinateAxes::showGrid(bool const flag) {
    grid = flag;
}


void CoordinateAxes::showAxes(bool const flag) {
    axes = flag;
}
