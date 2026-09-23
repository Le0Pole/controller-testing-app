#ifdef __WIN64__
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <thread>
#include "../Include/Wrapper.h"

#include <wingdi.h>

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
	
				wrp::ProgramContext pc;

				// Wierd windows method of getting an OpenGL context
				{
								WNDCLASS DummyWC = {sizeof(DummyWC)};
				
								DummyWC.lpfnWndProc = DefWindowProc;
								DummyWC.hInstance = GetModuleHandle(NULL);
								DummyWC.lpszClassName = "DummyWindowClass";

								ATOM DWCID = RegisterClass(&DummyWC);
								if (!DWCID) return 0;

								HWND DW = CreateWindowA(
																DummyWC.lpszClassName,
																"You are not supposed to see this",
																WS_DISABLED,
																0,
																0,
																0,
																0,
																NULL, NULL, GetModuleHandle(NULL), 0);
				
								// Getting a device context and setting the PFD

								PIXELFORMATDESCRIPTOR pfd = {
												sizeof(PIXELFORMATDESCRIPTOR),
												1,
												PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
												PFD_TYPE_RGBA,
												24,
												0, 0, 
												0, 0, 
												0, 0,
								        8, 0,
												0, 0, 0, 0, 0,
												32,
												8, 
												0,
												PFD_MAIN_PLANE,
												0, 0, 0, 0
								};

								HDC DummyDC = GetDC(DW);

								int DummyPFID = ChoosePixelFormat(DummyDC,&pfd);
								if (!DummyPFID) return 0;
								if (!SetPixelFormat(DummyDC, DummyPFID, &pfd)) return 0;
				
								// Getting the Rendering Context
								
								HGLRC DummyContext = wglCreateContext(DummyDC);
								if (!DummyDC) return 0;
								wglMakeCurrent(DummyDC, DummyContext);
								

								DestroyWindow(DW);
				}
				//
				
				WNDCLASS WindowClass = {sizeof(WNDCLASS)};

				WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
				WindowClass.hInstance = GetModuleHandle(0);
				WindowClass.lpszClassName = "MainWindowClass";
				WindowClass.style = CS_HREDRAW | CS_VREDRAW;
				WindowClass.lpfnWndProc = windProc;

				ATOM WCID = RegisterClass(&WindowClass);
				
				if (!WCID) return 0; // Exit if Windows failed to create a Window

				HWND WindowHandle = CreateWindowA(
								WindowClass.lpszClassName,												
								"Controller Testing Application", 
								WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								0, 0, GetModuleHandle(0), 0);	
				
				
				// Define everything for OpenGL wrp struct

				std::thread SysAgnosticCode(wrp::_MAIN, &pc);
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

				CloseWindow(WindowHandle);

				pc.ShouldBeRunning = false;

				// Wait for the detached thread to end execution before exiting.
				// Will exit anyways if the detached thread does not close in 5 seconds.

				int c = 0;
				while (pc.IsAlive) {
								if (c == 5) break;
								
								c++;

								std::chrono::duration<FLOAT> second = std::chrono::seconds(1);
								std::this_thread::sleep_for(second);
				}
				
				return windowContext.ExitVal; 
}

#endif 
