#ifdef __WIN64__
#define NOMINMAX

#include <Windows.h>
#include <thread>
#include "../Include/Wrapper.h"

struct WindowContext {
				bool running = true;
				WPARAM ExitVal;
};

WindowContext windowContext;

LRESULT windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp) {
				LRESULT rez = 0;

				switch (msg) {
								case WM_CLOSE:
												windowContext.running = false;
												windowContext.ExitVal = wp;
												break;
								default:
												rez = DefWindowProc(wind, msg, wp, lp);
												break;
				}

				return rez;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
				

				// Run System Agnostic Main Function in a seperate thread
				
				// Keep Window alive and provide openGl rendering?
				
				WNDCLASS WindowClass = {sizeof(WNDCLASS)};

				WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
				WindowClass.hInstance = GetModuleHandle(0);
				WindowClass.lpszClassName = "MainWindowClass";
				WindowClass.style = CS_HREDRAW | CS_VREDRAW;
				WindowClass.lpfnWndProc = windProc;

				ATOM WCID = RegisterClass(&WindowClass);
				
				if (!WCID) {
								//std::cout << "\033[31;1m" << GetLastError() << "\033[0m";
								return 0;
				}

				HWND WindowHandle = CreateWindowA(
								WindowClass.lpszClassName,												
								"Controller Testing Application", 
								WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								0, 0, GetModuleHandle(0), 0);	
				
				wrp::ProgramContext pc;

				std::thread SysAgnosticCode(wrp::MAIN, &pc);
				SysAgnosticCode.detach();

				MSG msg = {};
				while (windowContext.running) {
								if (!pc.ShouldBeRunning) {
												break;
								}

								while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE) > 0) {
												TranslateMessage(&msg);
												DispatchMessage(&msg);
								}
				}
				
				return windowContext.ExitVal; 
}

#endif 
