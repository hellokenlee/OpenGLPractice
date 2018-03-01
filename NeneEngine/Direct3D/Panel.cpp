/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Panel.h"

Panel::Panel() {
	// 初始化imgui
	ImGui_ImplDX11_Init(Utils::getHWND(), Utils::getDevice(), Utils::getContext());
	// 配色
	ImGui::StyleColorsClassic();
	// 关闭保存ini文件
	ImGui::GetIO().IniFilename = nullptr;
}

Panel::~Panel() {
	ImGui_ImplDX11_Shutdown();
}

void Panel::draw() {
	// 获取当前的摄像机
	pCamera = CameraController::pCamera;
	Utils::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Utils::getContext()->VSSetShader(nullptr, nullptr, 0);
	Utils::getContext()->PSSetShader(nullptr, nullptr, 0);
	Utils::getContext()->GSSetShader(nullptr, nullptr, 0);
	//
	ImGui_ImplDX11_NewFrame();
	ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	{
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(240, 260));
		// 摄像机相关信息
		if (pCamera != nullptr) {
			ImGui::TextColored(ImVec4(0.411f, 1.0f, 0.956f, 1.0), "Camera:");
			ImGui::Text("Position: (%.2f, %.2f, %.2f)", pCamera->position.x, pCamera->position.y, pCamera->position.z);
			ImGui::Text("Facing: (%.2f, %.2f, %.2f)", pCamera->front.x, pCamera->front.y, pCamera->front.z);
			ImGui::Text("Pitch/Yaw: (%.2f, %.2f)", CameraController::pitch, CameraController::yaw);
		}
		// 场景信息
		ImGui::TextColored(ImVec4(0.411f, 1.0f, 0.956f, 1.0), "Scene:");
		ImGui::Text("%.2f ms/frame (%.0f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("%d point lights", POINT_LIGHT_NUM);
		ImGui::Text("%d directional lights", DIRECTIONAL_LIGHT_NUM);
		// 快捷键
		ImGui::TextColored(ImVec4(0.411f, 1.0f, 0.956f, 1.0), "Control:");
		ImGui::Text("DrawMode[M]: %s", (Utils::getPolygonMode() == MODE_POLYGON_CULL ? "Fill" : "Wireframe"));
		ImGui::Text("Bloom[B]: %s", (CameraController::isBloom ? "On" : "Off"));
		switch (CameraController::caStatus) {
			case 0:
				ImGui::Text("Coordinates[C]: %s", "None");
				break;
			case 1:
				ImGui::Text("Coordinates[C]: %s", "Axes");
				break;
			case 2:
				ImGui::Text("Coordinates[C]: %s", "Grids");
				break;
		}
		switch (CameraController::aboardStatus) {
			case 1:
				ImGui::Text("Pree [F] to get on.");
				break;
			case 2:
				ImGui::Text("Pree [F] to get off.");
				break;
		}
	}
	ImGui::End();
	ImGui::Render();
}
