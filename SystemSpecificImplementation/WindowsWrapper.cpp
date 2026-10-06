#ifdef __WIN64__
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>
#include <string>

#include "../Include/Wrapper.h"

HWND WindowHandle;
HDC WindowDC;
HGLRC WGLCtx;

wrp::wBOOL wrp::OpenGL_Context::SwapBuffers() {
				return ::SwapBuffers(WindowDC);
}

wrp::ProgramContext ProgContext;

wrp::wBOOL wrp::PlatformContext::BindContext(wrp::wBOOL BindUnbind) {
				return wglMakeCurrent(WindowDC, WGLCtx);
}

wrp::PlatformContext PlatformCtx;

// Retrive OpenGL DLL

HMODULE OpenGL32 = GetModuleHandleA("opengl32.dll");




LRESULT windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp) {
				LRESULT rez = 0;

				switch (msg) {
								case WM_CLOSE:
												ProgContext.ShouldBeRunning = false;
												ProgContext.ExitVal = wp;
												break;
								default:
												rez = DefWindowProc(wind, msg, wp, lp);
												break;
				}

				return rez;
}


void* SafeGetProcAddress(const char* name) {
				void* addr = (void*)wglGetProcAddress(name);
				if (addr != NULL) return addr;

				ProgContext.Log.Warn("Failed to locate procedure named \'" + std::string(name) +"\' in context." , __LINE__, __FILE_NAME__, __FUNCTION__);
				
				addr = (void*)GetProcAddress(OpenGL32, name);
				
				if (addr == NULL) ProgContext.Log.Err("Failed to locate procedure named \'" + std::string(name) +"\' in opengl32.dll." , __LINE__, __FILE_NAME__, __FUNCTION__);

				return addr;
}








int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
				if (OpenGL32 == NULL) {
								DWORD Code = GetLastError();
								ProgContext.Log.Err("Failed to retrive opengl32.dll.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
								return Code;
				}

				// Wierd windows method of getting a version 3.3 OpenGL Context
				{
				
								WNDCLASS DummyWC = {sizeof(DummyWC)};
				
								DummyWC.lpfnWndProc = DefWindowProc;
								DummyWC.hInstance = GetModuleHandle(NULL);
								DummyWC.lpszClassName = "DummyWindowClass";

								if (!RegisterClass(&DummyWC)) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to register the dummy Window Class.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}

								HWND DW = CreateWindowA(
												DummyWC.lpszClassName,
												"You are not supposed to see this",
												WS_DISABLED,
												0,
												0,
												0,
												0,
												NULL, NULL, GetModuleHandle(NULL), 0);
								if (DW == NULL) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to create the dummy window.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}

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
								if (DummyDC == NULL) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to create Device Context.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}

								int DummyPFID = ChoosePixelFormat(DummyDC,&pfd);
								if (!DummyPFID) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to find PixelFormat for dummy context.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								if (!SetPixelFormat(DummyDC, DummyPFID, &pfd)) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to set the PixelFormat for dummy context.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								};
				
								// Getting the Rendering Context
								
								HGLRC DummyContext = wglCreateContext(DummyDC);
								if (!DummyContext) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to create the dummy OpenGL context.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								wglMakeCurrent(DummyDC, DummyContext);
								
								typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int *attribList);
								PROC RetPROC = wglGetProcAddress("wglCreateContextAttribsARB");
								if (RetPROC == NULL) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to locate wglCreateContextAttribsARB.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) RetPROC;
								
								
								typedef bool (WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const float *pfAttribFList, unsigned int nMaxFormats, int *piFormats, unsigned int *nNumFormats);
								RetPROC = wglGetProcAddress("wglChoosePixelFormatARB");
								if (RetPROC == NULL) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to locate wglChoosePixelFormatARB.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) RetPROC;

								wglMakeCurrent(NULL, NULL);
								
								// Registering a window class
				
								WNDCLASS WindowClass = {sizeof(WNDCLASS)};

								WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
								WindowClass.hInstance = GetModuleHandle(0);
								WindowClass.lpszClassName = "MainWindowClass";
								WindowClass.style = CS_HREDRAW | CS_VREDRAW;
								WindowClass.lpfnWndProc = windProc;
								
								if (!RegisterClass(&WindowClass)) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to register the dummy Window Class.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
				
								// Starting Window
				
								WindowHandle = CreateWindowA(
												WindowClass.lpszClassName,												
												"Controller Testing Application", 
												WS_OVERLAPPEDWINDOW, 
												CW_USEDEFAULT, 
												CW_USEDEFAULT, 
												CW_USEDEFAULT, 
												CW_USEDEFAULT, 
												0, 0, GetModuleHandle(0), 0);	
								
								if (WindowHandle == NULL) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to create a window.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}			

								WindowDC = GetDC(WindowHandle);
								if (WindowDC == NULL) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to get Device Context.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
				
								int attribs[] = {
				                0x2003, 0x2027, // WGL_ACCELERATION_ARB | WGL_FULL_ACCELERATION_ARB
				                0x201b, 8,      // WGL_ALPHA_BITS_ARB
												0x2022, 24,     // WGL_DEPTH_BITS_ARB
				                0x2001, 1,      // WGL_DRAW_TO_WINDOW_ARB
												0x2015, 8,      // WGL_RED_BITS_ARB
												0x2017, 8,      // WGL_GREEN_BITS_ARB
												0x2019, 8,      // WGL_BLUE_BITS_ARB
				                0x2013, 0x202B, // WGL_PIXEL_TYPE_ARB | WGL_TYPE_RGBA_ARB
				                0x2010,	1,      // WGL_SUPPORT_OPENGL_ARB
												0x2014,	32,     // WGL_COLOR_BITS_ARB
												0x2023, 1,      // WGL_DOUBLE_BUFFER_ARB
												0x2012, false,  // WGL_STEREO_ARB
												0x2024, 0,      // WGL_AUX_BUFFERS_ARB
				                0, 0
								};
												
								int PFormat = 0;
								unsigned int NumFormat = 0;
								if (!wglChoosePixelFormatARB(WindowDC, attribs, 0, 1, &PFormat, &NumFormat)) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to find the PixelFormat.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								
								PIXELFORMATDESCRIPTOR Cpfd = {sizeof(PIXELFORMATDESCRIPTOR)};
								
								if (DescribePixelFormat(WindowDC, PFormat, sizeof(Cpfd), &Cpfd) == 0) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to clone PixelFormat.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								if (!SetPixelFormat(WindowDC, PFormat, &Cpfd)) {
												DWORD Code = GetLastError();
												ProgContext.Log.Err("Failed to set PixelFormat.", "Sys err code: " + std::to_string(Code), __LINE__, __FILE_NAME__, __FUNCTION__);
												return Code;
								}
								
								int CtxAttribs[] = {
												0x9126, 0x00000001, // WGL_CONTEXT_PROFILE_MASK_ARB | WGL_CONTEXT_CORE_PROFILE_BIT_ARB				
												0x2091, 3, // WGL_CONTEXT_MAJOR_VERSION_ARB
												0x2092, 3, // WGL_CONTEXT_MINOR_VERSION_ARB
												0, 0
								};
				
								WGLCtx = wglCreateContextAttribsARB(WindowDC, 0, CtxAttribs);

								wglDeleteContext(DummyContext);
								DeleteDC(DummyDC);
								DestroyWindow(DW);
				}
				
				wglMakeCurrent(WindowDC, WGLCtx);
				
				if (gladLoadGLContext(&ProgContext.OpenGL.gl, (GLADloadfunc)SafeGetProcAddress) == 0) {
								ProgContext.Log.Err("Failed to Load OpenGL functions with GLAD.", __LINE__, __FILE_NAME__, __FUNCTION__);
								return 1;
				}

				wglMakeCurrent(NULL, NULL);

				std::thread SysAgnosticCode(wrp::_MAIN, &ProgContext, &PlatformCtx);
				SysAgnosticCode.detach();
				
				ShowWindow(WindowHandle, SW_SHOW);
				UpdateWindow(WindowHandle);

				MSG msg = {};
				while (ProgContext.IsAlive) {
								while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE) > 0) {
												TranslateMessage(&msg);
												DispatchMessage(&msg);
								}
				}

				wglDeleteContext(WGLCtx);
				DeleteDC(WindowDC);
				DestroyWindow(WindowHandle);
				
				return ProgContext.ExitVal; 
}

#endif 
