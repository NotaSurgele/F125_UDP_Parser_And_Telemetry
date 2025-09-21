//
// Created by User on 21/09/2025.
//
#pragma once
#include <winsock2.h>
#include <windows.h>
#include <d3d11.h>
#include <f125parser.h>
#include <tchar.h>
#include <windowsx.h>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class F125MetricDisplayer {
public:
    explicit F125MetricDisplayer(F125parser& parser) :
        g_pd3dDevice(nullptr), g_pd3dDeviceContext(nullptr),
        g_pSwapChain(nullptr), g_mainRenderTargetView(nullptr),
        g_hWnd(nullptr), _parser(parser),
        _panelPos(0,0), _panelSize(0,0) {}

    int Run(HINSTANCE hInstance) {
        // Register window class
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProcSetup,
            0L, 0L, hInstance, NULL, NULL, NULL, NULL,
            _T("F125Overlay"), NULL };
        RegisterClassEx(&wc);

        // Create overlay window
        g_hWnd = CreateWindowEx(
            WS_EX_LAYERED | WS_EX_TOPMOST, // Don't set WS_EX_TRANSPARENT permanently
            wc.lpszClassName, _T("Overlay"),
            WS_POPUP, 0, 0, 1920, 1080,
            NULL, NULL, wc.hInstance, this
        );

        SetLayeredWindowAttributes(g_hWnd, RGB(0,0,0), 0, LWA_COLORKEY);
        ShowWindow(g_hWnd, SW_SHOWDEFAULT);
        UpdateWindow(g_hWnd);

        // Init Direct3D
        if (!CreateDeviceD3D()) {
            CleanupDeviceD3D();
            UnregisterClass(wc.lpszClassName, wc.hInstance);
            return 1;
        }
        CreateRenderTarget();

        // Setup ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        // Main loop
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while (msg.message != WM_QUIT) {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }
            RenderFrame();
        }

        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        DestroyWindow(g_hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);

        return 0;
    }

private:
    // DX state
    ID3D11Device* g_pd3dDevice;
    ID3D11DeviceContext* g_pd3dDeviceContext;
    IDXGISwapChain* g_pSwapChain;
    ID3D11RenderTargetView* g_mainRenderTargetView;
    HWND g_hWnd;

    // ImGui panel position/size for hit-testing
    ImVec2 _panelPos;
    ImVec2 _panelSize;

    F125parser& _parser;

    static LRESULT CALLBACK WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (msg == WM_NCCREATE) {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto* app = reinterpret_cast<F125MetricDisplayer*>(cs->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProcRedirect);
            return app->WndProc(hWnd, msg, wParam, lParam);
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    static LRESULT CALLBACK WndProcRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        auto* app = reinterpret_cast<F125MetricDisplayer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return app->WndProc(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg) {
            case WM_NCHITTEST: {
                POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                ScreenToClient(hWnd, &pt);

                // Only allow clicks inside ImGui panel
                if (pt.x >= _panelPos.x && pt.x <= _panelPos.x + _panelSize.x &&
                    pt.y >= _panelPos.y && pt.y <= _panelPos.y + _panelSize.y)
                    return HTCLIENT;

                // Everything else is transparent
                return HTTRANSPARENT;
            }
            case WM_SIZE:
                if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
                    CleanupRenderTarget();
                    g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam),
                        DXGI_FORMAT_UNKNOWN, 0);
                    CreateRenderTarget();
                }
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void RenderFrame();

    bool CreateDeviceD3D() {
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = g_hWnd;
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL levels[1] = { D3D_FEATURE_LEVEL_11_0 };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
            levels, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
            &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
            return false;
        return true;
    }

    void CleanupDeviceD3D() {
        CleanupRenderTarget();
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    }

    void CreateRenderTarget() {
        ID3D11Texture2D* pBackBuffer = NULL;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget() {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    }
private:
    void RenderDriversSelectables() const;
    void RenderCarVelocity(int &index) const;
};

/*// Entry point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    F125MetricDisplayer overlay;
    return overlay.Run(hInstance);
}*/