#include <iostream>
#include <array>

#include "../include/receiver.h"
#include "imgui.h"
#include "f125parser.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl2.h"

using asio::ip::udp;

void parseParticipantData(std::array<unsigned char, 4000>& recv_buff) {
    try {
        auto ppd = reinterpret_cast<PacketParticipantsData *>(&recv_buff);
        std::cout << static_cast<unsigned int>(ppd->m_numActiveCars) << std::endl;

        std::cout << ppd->m_participants[1].m_name << std::endl;


    } catch (std::exception e) {
        throw e.what();
    }
}

void parseMotionData(std::array<unsigned char, 4000> & recv_buff ) {
    try {
        auto pmd = reinterpret_cast<PacketMotionData *>(&recv_buff);


        std::cout << "Packet header " << pmd->m_header.m_packetFormat << std::endl;

        std::cout << "player index : \"" << (int) pmd->m_header.m_playerCarIndex << "\"" << std::endl;
        
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
}

int listener(void) {
    try {
        asio::io_context io_context;

        // Create a UDP socket and bind it to any port (or a specific port)
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 20777));

        std::array<unsigned char, 4000> recv_buf;
        udp::endpoint sender_endpoint;

        std::cout << "Listening for UDP messages on port 12345...\n";

        while (true) {
            // Receive data
            socket.receive_from(asio::buffer(recv_buf), sender_endpoint);

            auto ph = reinterpret_cast<PacketHeader *>(&recv_buf);

            switch (ph->m_packetId) {
                case ePacketIdMotion:
                    //parseMotionData(recv_buf);
                    break;
                case ePacketIdSession:
                    // do stuff
                        break;
                case ePacketIdLapData:
                    // do stuff
                        break;
                case ePacketIdEvent:
                    // do stuff
                        break;
                case ePacketIdParticipants:
                        parseParticipantData(recv_buf);
                        break;
                case ePacketIdCarSetups:
                    // do stuff
                        break;
                case ePacketIdCarTelemetry:
                    //do stuff
                        break;
                case ePacketIdCarStatus:
                    // do stuff;
                        break;
                case ePacketIdFinalClassification:
                    // do stuff
                        break;
                case ePacketIdLobbyInfo:
                    // do stuff
                        break;
                case ePacketIdCarDamage:
                    // do stuff
                        break;
                case ePacketIdSessionHistory:
                    //do stuff
                        break;
                case ePacketIdTyreSets:
                    // do stuff
                    break;
                case ePacketIdMotionEx:
                    // do stuff
                        break;
                case ePacketIdTimeTrial:
                    // do stuff
                        break;
                case ePacketIdLapPositions:
                    // do stuff
                        break;
                default:
                    throw std::invalid_argument("oupsi should not happend hihihi");
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
/*
// Forward declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND CreateOpenGLWindow(HINSTANCE hInstance, int width, int height, const char* title);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    int screenWidth = 1280;
    int screenHeight = 720;

    HWND hwnd = CreateOpenGLWindow(hInstance, screenWidth, screenHeight, "F1 Overlay");

    HDC hdc = GetDC(hwnd);

    // --- OpenGL setup ---
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    HGLRC glContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, glContext);

    // --- ImGui setup ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL2_Init();
    ImGui::StyleColorsDark();

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    // --- Main loop ---
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Example overlay window
        ImGui::Begin("Overlay Example");
        ImGui::Text("Hello, OpenGL overlay!");
        ImGui::End();

        ImGui::Render();

        // Clear screen
        glViewport(0, 0, screenWidth, screenHeight);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render ImGui
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SwapBuffers(hdc);
    }

    // --- Cleanup ---
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(glContext);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);

    return 0;
}

// --- Create a simple OpenGL window ---
HWND CreateOpenGLWindow(HINSTANCE hInstance, int width, int height, const char* title) {
    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = "OpenGLWindowClass";

    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowExA(
        WS_EX_APPWINDOW, // normal window
        "OpenGLWindowClass",
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr
    );

    return hwnd;
}

// --- Win32 message handler ---
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {
        case WM_SIZE: return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}*/

int main(void) {
    asio::io_context context;

    Receiver r = Receiver(context, 20777);
    F125parser f1parser = F125parser();

    std::cout << "float: " << sizeof(uint8) << std::endl;

    r.start();
    std::cout << "Start listening on 20777" << std::endl;
    while (true) {
        auto p = r.poll();

        if (p != nullptr) {
            try {
                f1parser.parse(p);
            } catch (std::exception& e) {
                std::cerr << "Parsing error: " << e.what() << std::endl;
            }
        } 
    }
    return 0;
}