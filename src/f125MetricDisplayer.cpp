//
// Created by User on 21/09/2025.
//

#include "f125MetricDisplayer.h"

void F125MetricDisplayer::RenderFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("F1 25 Overlay");

    _panelPos = ImGui::GetWindowPos();
    _panelSize = ImGui::GetWindowSize();

    std::shared_ptr<Packet> pd = _parser.getPacket<ParticipantData>();

    if (pd) {
        auto buffer = pd->data;
        int activeCars = *pd->data.data();
        auto ppd = reinterpret_cast<ParticipantData *>(buffer.data() + sizeof(uint8)); // after m_numActiveCars
        ImGui::Text("Num drivers: %d", activeCars);

        for (int i = 0; i < activeCars; i++) {
            ImGui::Text("Driver name %s ",  ppd[i].m_name);
        }
    } else {
        ImGui::Text("Waiting for packet...");
    }

    ImGui::End();

    ImGui::Render();
    const float clear_color[4] = {0,0,0,0};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    g_pSwapChain->Present(1, 0);
}
