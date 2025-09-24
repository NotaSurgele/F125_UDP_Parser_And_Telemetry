//
// Created by User on 21/09/2025.
//

#include "f125MetricDisplayer.h"

void F125MetricDisplayer::RenderCarVelocity(int& index) const {
    auto packetMotionData = _parser.getPacket<PacketMotionData>();

    if (!packetMotionData) return;

    auto carsMotionData = reinterpret_cast<CarMotionData *>(packetMotionData->data.data());
    auto playerCarMotionData = carsMotionData[index];

    ImGui::Text("Velocity:");

    ImGui::Indent(INDENT);
    ImGui::Text("x %f", playerCarMotionData.m_worldVelocityX);
    ImGui::SameLine();
    ImGui::Text("y %f", playerCarMotionData.m_worldVelocityY);
    ImGui::SameLine();
    ImGui::Text("z %f", playerCarMotionData.m_worldVelocityZ);
    ImGui::Unindent(INDENT);
}

void F125MetricDisplayer::RenderPedalPressure(const float &amount,  ImU32 color) const {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 size = ImVec2(30, 200);

    draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), IM_COL32(50, 50, 50, 255));

    float filledHeight = size.y * amount;

    draw_list->AddRectFilled(
        ImVec2(p.x, p.y + size.y - filledHeight),
        ImVec2(p.x + size.x, p.y + size.y),
        color
    );

    ImGui::Dummy(size);
}


void F125MetricDisplayer::RenderPedalsPressure(int &index) const {
    static bool displayPressure = false;
    auto& packet = _parser.getPacket<PacketCarTelemetryData>();

    if (ImGui::Selectable("Pedal pressure")) {
        displayPressure = true;
    }

    if (!displayPressure || !packet) return;

    ImGui::Begin("Pedals Pressure", &displayPressure);

    auto carsTelemetryDataArray = reinterpret_cast<CarTelemetryData *>(packet->data.data());
    auto playerCarTelemetryData = carsTelemetryDataArray[index];

    std::cout << "size: " << sizeof(CarTelemetryData) << std::endl;

    ImGui::Text("packet size %d", packet->data.size());
    ImGui::Text("Throttle %f", playerCarTelemetryData.m_throttle);
    ImGui::Text("Brake %f", playerCarTelemetryData.m_brake);
    ImGui::Text("Gear %d", playerCarTelemetryData.m_gear);
    ImGui::Text("SPEED %d", playerCarTelemetryData.m_speed);

    std::cout << index << std::endl;
    // Throttle
    RenderPedalPressure(playerCarTelemetryData.m_throttle, IM_COL32(0, 255, 0, 255));

    ImGui::SameLine();

    // Brake
    RenderPedalPressure(playerCarTelemetryData.m_brake, IM_COL32(255, 0, 0, 255));

    ImGui::End();
}


void F125MetricDisplayer::RenderDriversGUI() const {
    std::shared_ptr<Packet> ppd = _parser.getPacket<PacketParticipantsData>();

    if (ppd) {
        auto playerPos = ppd->head.m_playerCarIndex;
        auto buffer = ppd->data;
        int activeCars = *ppd->data.data();
        auto pd = reinterpret_cast<ParticipantData *>(buffer.data() + sizeof(uint8)); // after m_numActiveCars

        ImGui::Text("Num drivers: %d", activeCars);
        ImGui::Text("Player array position: %d", playerPos);

        static int selected = playerPos;
        static bool displayData = true;

        for (int i = 0; i < activeCars; i++) {
            std::string driverLabel = "Driver:  " + std::string(pd[i].m_name);
            //auto treeNodeFlag = i == playerPos ? ImGuiTreeNodeFlags_DefaultOpen : 0;

            /*if (ImGui::TreeNodeEx(driverLabel.c_str(), treeNodeFlag)) {
                RenderCarVelocity(i);
                ImGui::TreePop();
            }*/
            if (ImGui::Selectable(driverLabel.c_str())) {
                selected = i;
                displayData = true;
            }
        }

        // display car data's
        {
            if (displayData) {
                ImGui::Begin("Cars data", &displayData);
                RenderCarVelocity(selected);
                RenderPedalsPressure(selected);
                ImGui::End();
            }
        }

    } else {
        ImGui::Text("Waiting for packets...");
    }
}


void F125MetricDisplayer::RenderFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("F1 25 Overlay");

    _panelPos = ImGui::GetWindowPos();
    _panelSize = ImGui::GetWindowSize();

    RenderDriversGUI();

    ImGui::End();

    ImGui::Render();
    const float clear_color[4] = {0,0,0,0};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(1, 0);
}
