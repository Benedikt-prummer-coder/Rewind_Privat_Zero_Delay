#include "app.h"
#include "ui.h"
#include "auth.h"
#include "logger.h"
#include <Windows.h>

int App::Run()
{
    Logger::Init();

    if (!Auth::Login())
        return 0;

    UI ui;
    if (!ui.Init())
        return 0;

    ui.Loop();
    ui.Shutdown();
    return 0;
}
