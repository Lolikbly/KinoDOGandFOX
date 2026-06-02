#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "KinoDOGandFOX.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew KinoDOGandFOX());
    return 0;
}