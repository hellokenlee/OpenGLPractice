/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/

#include "ControlPanel.h"

using namespace std;

ControlPanel::ControlPanel(GLFWwindow *window){
    // 初始化Imgui
    ImGui_ImplGlfwGL3_Init(window, false);
    // 忽略由Imgui产生的INVALID_OPERATION错误
    glGetError();
}

ControlPanel::~ControlPanel() {
    ImGui_ImplGlfwGL3_Shutdown();
}

void ControlPanel::draw() {
    //
    static Camera* currentCamera = CameraController::getCamera();
    // 设置GUI
    ImGui_ImplGlfwGL3_NewFrame();
    {
        bool tmp = false;
        ImGui::Begin("Status", &tmp, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(10, 10));
        ImGui::SetWindowSize(ImVec2(240, 180));
        // 视角相关信息
        ImGui::TextColored(ImVec4(0.411f, 1.0f , 0.956f, 1.0) ,"Camera:");
        ImGui::Text("Pos: (%.2f, %.2f, %.2f)",
                    currentCamera->cameraPos.x, currentCamera->cameraPos.y, currentCamera->cameraPos.z);
        ImGui::Text("Facing: (%.2f, %.2f, %.2f)",
                    currentCamera->cameraFront.x, currentCamera->cameraFront.y, currentCamera->cameraFront.z);
        ImGui::Text("PY: (%.2f, %.2f)",
                    CameraController::pitch, CameraController::yaw);
        ImGui::Text("Lock[L]: %s", (CameraController::lock ? "True" : "False"));
        // 性能相关信息
        ImGui::TextColored(ImVec4(0.411f, 1.0f , 0.956f, 1.0) ,"Performance:");
        ImGui::Text("%.2f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //
        switch(CameraController::polygonModes[CameraController::modeIndex]) {
            case GL_LINE:
                ImGui::Text("Mode[M]: Lines");
                break;
            case GL_POINT:
                ImGui::Text("Mode[M]: Points");
                break;
            case GL_FILL:
                ImGui::Text("Mode[M]: Fill");
                break;
            default:
                ImGui::Text("Mode[M]: Other");
        }
        //
        ImGui::End();
    }
    // 以填充模式绘制GUI
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // 绘制GUI
    ImGui::Render();
    // 恢复绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, CameraController::polygonModes[CameraController::modeIndex]);
}
