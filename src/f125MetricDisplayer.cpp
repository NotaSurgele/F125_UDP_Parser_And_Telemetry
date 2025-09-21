//
// Created by User on 21/09/2025.
//

#include "f125MetricDisplayer.h"

void F125MetricDisplayer::RenderCarVelocity(int& index) const {
    auto packetMotionData = _parser.getPacket<PacketMotionData>();

    if (!packetMotionData) return;

    auto carsMotionData = reinterpret_cast<CarMotionData *>(packetMotionData->data.data());
    auto playerCarMotionData = carsMotionData[index];

    ImGui::Text("Velocity x %f", playerCarMotionData.m_worldVelocityX);
    ImGui::Text("Velocity y %f", playerCarMotionData.m_worldVelocityY);
    ImGui::Text("Velocity z %f", playerCarMotionData.m_worldVelocityZ);
}


void F125MetricDisplayer::RenderDriversSelectables() const {
    std::shared_ptr<Packet> ppd = _parser.getPacket<PacketParticipantsData>();

    if (ppd) {
        auto playerPos = ppd->head.m_playerCarIndex;
        auto buffer = ppd->data;
        int activeCars = *ppd->data.data();
        auto pd = reinterpret_cast<ParticipantData *>(buffer.data() + sizeof(uint8)); // after m_numActiveCars

        ImGui::Text("Num drivers: %d", activeCars);
        ImGui::Text("Player array position: %d", playerPos);

        for (int i = 0; i < activeCars; i++) {
            std::string driverLabel = "Driver:  " + std::string(pd[i].m_name);
            auto treeNodeFlag = i == playerPos ? ImGuiTreeNodeFlags_DefaultOpen : 0;

            if (ImGui::TreeNodeEx(driverLabel.c_str(), treeNodeFlag)) {
                RenderCarVelocity(i);
                ImGui::TreePop();
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

    RenderDriversSelectables();

    ImGui::End();

    ImGui::Render();
    const float clear_color[4] = {0,0,0,0};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(1, 0);
}
