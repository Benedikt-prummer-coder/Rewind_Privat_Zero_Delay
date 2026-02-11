// tweaks.cpp
#include "tweaks.h"
#include "logger.h"
#include <imgui.h>
#include <Windows.h>

static bool applied = false;

void Tweaks::Draw()
{
    if (!applied) {
        if (ImGui::Button("Power Plan Höchstleistung")) {
            system("powercfg -setactive SCHEME_MIN");
            Logger::Log("Power Plan aktiviert");
            applied = true;
        }
    } else {
        ImGui::Text("Applied");
        if (ImGui::Button("Revert")) {
            system("powercfg -setactive SCHEME_BALANCED");
            Logger::Log("Power Plan revertiert");
            applied = false;
        }
    }
}
