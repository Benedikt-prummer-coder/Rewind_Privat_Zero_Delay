// performance.cpp
#include "performance.h"
#include <imgui.h>
#include <Windows.h>
#include <psapi.h>

void Performance::Draw()
{
    MEMORYSTATUSEX mem{ sizeof(mem) };
    GlobalMemoryStatusEx(&mem);

    ImGui::Text("RAM Usage: %.1f %%", mem.dwMemoryLoad);
    ImGui::Text("RAM Total: %.1f GB",
        mem.ullTotalPhys / 1024.0 / 1024 / 1024);
}
