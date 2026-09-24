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
	
				// WGL loading function pointers
				
				typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int *attribList);
				PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
				
				typedef bool (WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const float *pfAttribFList, unsigned int nMaxFormats, int *piFormats, unsigned int *nNumFormats);
				PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;



				wrp::ProgramContext ProgContext;

				// Wierd windows method of getting an OpenGL context and loading function
				{
								WNDCLASS DummyWC = {sizeof(DummyWC)};
				
								DummyWC.lpfnWndProc = DefWindowProc;
								DummyWC.hInstance = GetModuleHandle(NULL);
								DummyWC.lpszClassName = "DummyWindowClass";

								ATOM DWCID = RegisterClass(&DummyWC);
								if (!DWCID) return 0;      // Add Error Logging Later.

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
								if (!DummyPFID) return 0;      // Add Error Logging Later.
								if (!SetPixelFormat(DummyDC, DummyPFID, &pfd)) return 0;      // Add Error Logging Later.
				
								// Getting the Rendering Context
								
								HGLRC DummyContext = wglCreateContext(DummyDC);
								if (!DummyDC) return 0;      // Add Error Logging Later.
								wglMakeCurrent(DummyDC, DummyContext);
	


								PROC RetPROC = wglGetProcAddress("wglCreateContextAttribsARB");
								if (RetPROC == NULL) return 0;      // Add Error Logging Later.
								wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) RetPROC;

								RetPROC = wglGetProcAddress("wglChoosePixelFormatARB");
								if (RetPROC == NULL) return 0;      // Add Error Logging Later.
								wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) RetPROC;



								wglMakeCurrent(NULL, NULL);
								wglDeleteContext(DummyContext);
								DeleteDC(DummyDC);
								DestroyWindow(DW);
				}
				
				// Registering a window class

				WNDCLASS WindowClass = {sizeof(WNDCLASS)};

				WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
				WindowClass.hInstance = GetModuleHandle(0);
				WindowClass.lpszClassName = "MainWindowClass";
				WindowClass.style = CS_HREDRAW | CS_VREDRAW;
				WindowClass.lpfnWndProc = windProc;

				ATOM WCID = RegisterClass(&WindowClass);
				
				if (!WCID) return 0;       // Add Error Logging Later.

				// Starting Window

				HWND WindowHandle = CreateWindowA(
								WindowClass.lpszClassName,												
								"Controller Testing Application", 
								WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								CW_USEDEFAULT, 
								0, 0, GetModuleHandle(0), 0);	
				
				if (WindowHandle == NULL) return 0;       // Add Error Logging Later.

				HDC WindowDC = GetDC(WindowHandle);
				if (WindowDC == NULL) return 0;      // Add Error Logging Later.

				// Define everything for OpenGL wrp struct

				int attribs[] = {
                0x2003, 0x2027, // WGL_ACCELERATION_ARB | WGL_FULL_ACCELERATION_ARB
                0x201b, 8, // WGL_ALPHA_BITS_ARB
                0x2022, 24, // WGL_DEPTH_BITS_ARB
                0x2001, 1, // WGL_DRAW_TO_WINDOW_ARB
								0x2015, 8, // WGL_RED_BITS_ARB
								0x2017, 8, // WGL_GREEN_BITS_ARB
								0x2019, 8, // WGL_BLUE_BITS_ARB
                0x2013, 0x202B, // WGL_PIXEL_TYPE_ARB | WGL_TYPE_RGBA_ARB
                0x2010,		1, // WGL_SUPPORT_OPENGL_ARB
								0x2014,	 32, // WGL_COLOR_BITS_ARB
                0, 0
        };
				
				int PFormat = 0;
				unsigned int NumFormat = 0;
				wglChoosePixelFormatARB(WindowDC, attribs, 0, 1, &PFormat, &NumFormat);

				HGLRC WGLC = wglCreateContextAttribsARB(WindowDC, 0, attribs);

				std::thread SysAgnosticCode(wrp::_MAIN, &ProgContext);
				SysAgnosticCode.detach();

				MSG msg = {};
				while (windowContext.running) {
								if (!ProgContext.ShouldBeRunning) {
												break;
								}

								while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE) > 0) {
												TranslateMessage(&msg);
												DispatchMessage(&msg);
								}
				}

				CloseWindow(WindowHandle);

				ProgContext.ShouldBeRunning = false;

				// Wait for the detached thread to end execution before exiting.
				// Will exit anyways if the detached thread does not close in 5 seconds.

				int c = 0;
				while (ProgContext.IsAlive) {
								if (c == 5) break;
								
								c++;

								std::chrono::duration<FLOAT> second = std::chrono::seconds(1);
								std::this_thread::sleep_for(second);
				}
				
				return windowContext.ExitVal; 
}

#endif 
