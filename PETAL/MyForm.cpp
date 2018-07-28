#include "MyForm.h"
#include "ApplicationLauncher.h"

using namespace System;
using namespace System::Windows::Forms;

void launchOptiKey() {

}

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PETAL::MyForm form;
	Application::Run(%form);
}

