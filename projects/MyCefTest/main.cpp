#include "client_app.h"
#include "cef_app_browser.h"
#include "cef_app_render.h"
#include "cef_app_other.h"
#include "include/cef_command_line.h"
int main(int argc, char *argv[])
{
    CefMainArgs main_args(argc, argv);
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);
    // Create a ClientApp of the correct type.
    CefRefPtr<ClientApp> app;
    ClientApp::ProcessType process_type = ClientApp::GetProcessType(command_line);
    if (process_type == ClientApp::RendererProcess
            ||process_type == ClientApp::ZygoteProcess)
        // On Linux the zygote process is used to spawn other process types. Since
        // we don't know what type of process it will be give it the renderer
        // client.
        app = new CefAppRender();
    else if (process_type == ClientApp::OtherProcess)
        app = new CefAppOther();

    // Execute the secondary process.
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0)
    {
        return exit_code;
    }
    return 0;
}