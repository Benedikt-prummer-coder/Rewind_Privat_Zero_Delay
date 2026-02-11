#include "ui.h"
#include "performance.h"
#include "tweaks.h"
#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#pragma comment(lib,"d3d11.lib")

static ID3D11Device* gDevice{};
static ID3D11DeviceContext* gContext{};
static IDXGISwapChain* gSwap{};
static ID3D11RenderTargetView* gRTV{};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, w, l))
        return true;
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, w, l);
}

bool UI::Init()
{
    WNDCLASSEX wc{ sizeof(wc), CS_CLASSDC, WndProc, 0,0,
        GetModuleHandle(nullptr), nullptr,nullptr,nullptr,nullptr,
        L"RewindPrivat", nullptr };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"Rewind Privat Zero Delay",
        WS_OVERLAPPEDWINDOW, 100,100,1280,800,
        nullptr,nullptr,wc.hInstance,nullptr);

    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &sd, &gSwap, &gDevice, nullptr, &gContext);

    ID3D11Texture2D* back{};
    gSwap->GetBuffer(0, IID_PPV_ARGS(&back));
    gDevice->CreateRenderTargetView(back, nullptr, &gRTV);
    back->Release();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(gDevice, gContext);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    return true;
}

void UI::Loop()
{
    MSG msg{};
    int tab = 0;

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0,0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Rewind Privat Zero Delay");

        ImGui::BeginChild("Sidebar", ImVec2(180,0), true);
        const char* tabs[] = {
            "Dashboard","Gaming Tweaks","Network Tweaks",
            "System Tweaks","Debloat","Performance","Logs","Settings"
        };
        for (int i=0;i<8;i++)
            if (ImGui::Selectable(tabs[i], tab==i))
                tab=i;
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Main");
        if (tab == 5)
            Performance::Draw();
        if (tab == 1)
            Tweaks::Draw();
        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        float clr[4]{0.06f,0.06f,0.08f,1};
        gContext->OMSetRenderTargets(1,&gRTV,nullptr);
        gContext->ClearRenderTargetView(gRTV,clr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        gSwap->Present(1,0);
    }
}

void UI::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (gRTV) gRTV->Release();
    if (gSwap) gSwap->Release();
    if (gContext) gContext->Release();
    if (gDevice) gDevice->Release();
}
