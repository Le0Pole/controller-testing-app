#ifdef __WIN64__
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>
#include <string>

#include "../Include/Wrapper.h"

// Prefer NVIDIA / AMD GPU

extern "C" 
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Retrive OpenGL DLL
HMODULE OpenGL32 = GetModuleHandleA("opengl32.dll");


//  ============= Wrapper Windows Implementation =============

HWND WindowHandle;
HDC WindowDC;
HGLRC WGLCtx;

struct WrapperVars {
				wrp::wI32 XWindow = 0;
				wrp::wI32 YWindow = 0;
				wrp::wI32 dXWindow = 0;
				wrp::wI32 dYWindow = 0;
};

WrapperVars Vars;



wrp::wBOOL wrp::PlatformContext::BindContext(wrp::wBOOL BindUnbind) {
				return wglMakeCurrent(WindowDC, WGLCtx);
}

wrp::PlatformContext PlatformCtx;



wrp::wBOOL wrp::OpenGL_Context::SwapBuffers() {
				wrp::wBOOL swapResult = ::SwapBuffers(WindowDC);
	
				return swapResult;
}

wrp::wBOOL wrp::System_Functions::GetFileContent(wrp::wSTRING FilePath, wrp::wSTRING& Content) {
				
				wrp::wSTRING ret = "";

				HANDLE File = CreateFileA(FilePath.data(), 
												GENERIC_READ, 
												0 /*No sharing*/, 
												NULL /*no security attributes*/, 
												OPEN_EXISTING, 
												FILE_ATTRIBUTE_NORMAL, 
												NULL /*No temporary handle... what even is that???*/);

				if (File == INVALID_HANDLE_VALUE) return 0;
				
				LARGE_INTEGER FileSize;
				if (!GetFileSizeEx(File, &FileSize)) return 0;
				
				Content.resize(FileSize.LowPart);

				DWORD BytesRead = 0;
				if (!ReadFile(File, LPVOID(Content.data()), FileSize.LowPart, &BytesRead, NULL /*Synchronous read*/)) return 0;

				CloseHandle(File);

				return 1;
				
};


void wrp::ProgramContext::PollSystem(wrp::ProgramContext* Ctx) {
				if (Vars.dXWindow > 0 && Vars.dYWindow > 0){
								Ctx->OpenGL.gl.Viewport(0, 0, Vars.dXWindow, Vars.dYWindow);
				}
};

wrp::ProgramContext ProgContext;


//  ==========================

LRESULT windProc(HWND wind, UINT msg, WPARAM wp, LPARAM lp) {
				LRESULT rez = 0;

				switch (msg) {
								case WM_CLOSE:
												ProgContext.ShouldBeRunning = false;
												ProgContext.ExitVal = wp;
												break;
								case WM_SIZE:
												Vars.dXWindow = wrp::wI32(wrp::wI16(LOWORD(lp)));
												Vars.dYWindow = wrp::wI32(wrp::wI16(HIWORD(lp)));

												break;
								case WM_MOVE:
												Vars.XWindow = wrp::wI32(wrp::wI16(LOWORD(lp)));
												Vars.YWindow = wrp::wI32(wrp::wI16(HIWORD(lp)));

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

				//ProgContext.Log.Warn("Failed to locate procedure named \'" + std::string(name) +"\' in context." , __LINE__, __FILE_NAME__, __FUNCTION__);
				
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
												32,
												0, 0, 
												0, 0, 
												0, 0,
								        0, 0,
												0, 0, 0, 0, 0,
												24,
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
												CW_USEDEFAULT, // X 
												CW_USEDEFAULT, // Y
												CW_USEDEFAULT, // Witdh
												CW_USEDEFAULT, // Height
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
												0x2001, GL_TRUE, // WGL_DRAW_TO_WINDOW_ARB
												0x2010, GL_TRUE, // WGL_SUPPORT_OPENGL_ARB
												0x2011, GL_TRUE, // WGL_DOUBLE_BUFFER_ARB
												0x2013, 0x202B , // WGL_PIXEL_TYPE_ARB | WGL_TYPE_RGBA_ARB
												0x2014, 32     , // WGL_COLOR_BITS_ARB
												0x2022, 24     , // WGL_DEPTH_BITS_ARB
												0x2023, 8      , // WGL_STENCIL_BITS_ARB
												0                // END
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
												0x2091, 3         , // WGL_CONTEXT_MAJOR_VERSION_ARB
												0x2092, 3         , // WGL_CONTEXT_MINOR_VERSION_ARB
												0                   // END
								};
				
								WGLCtx = wglCreateContextAttribsARB(WindowDC, 0, CtxAttribs);

								wglDeleteContext(DummyContext);
								DeleteDC(DummyDC);
								DestroyWindow(DW);

								wglMakeCurrent(NULL, NULL);
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
