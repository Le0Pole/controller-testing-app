#include "Include/Wrapper.h"
#include "Include/LinearAlgebra/Vec.h"
#include <iterator>
#include <malloc.h>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>

using wrp::wSTRING;
using wrp::wBOOL;
using wrp::wFLOAT;
using wrp::wI8;
using wrp::wI16;
using wrp::wI32;
using wrp::wI64;
using wrp::wU8;
using wrp::wU16;
using wrp::wU32;
using wrp::wU64;

#define PI 3.14159 

wFLOAT min(wFLOAT arg1, wFLOAT arg2) {
				return (arg1 < arg2 ? arg1 : arg2);
}

wFLOAT max(wFLOAT arg1, wFLOAT arg2) {
				return (arg1 > arg2 ? arg1 : arg2);
}

struct gamepad {
				
				wFLOAT JLX = 0; // Left Joystick X
				wFLOAT JLY = 0; // Left Joystick Y
				wFLOAT JRX = 0; // Right Joystick X
				wFLOAT JRY = 0; // Right Joystick X
				
				wFLOAT LJDeadZone = 0; // Dead Zone for the left Joystick
				wFLOAT RJDeadZone = 0; // Dead Zone for the right Joystick

				wBOOL IsLJdown = false;
				wBOOL IsRJdown = false;

				/*
				 *    BL4       BR4
				 * BL3   BL2 BR3   BR2
				 *    BL1       BR1
				 */

				wBOOL IsBL1Down = false;
				wBOOL IsBL2Down = false;
				wBOOL IsBL3Down = false;
				wBOOL IsBL4Down = false;

				wBOOL IsBR1Down = false;
				wBOOL IsBR2Down = false;
				wBOOL IsBR3Down = false;
				wBOOL IsBR4Down = false;

				/*
				 *  BL2  BR2
				 *	BL1  BR1
				 */

				wFLOAT BR1 = 0;
				wFLOAT BR2 = 0;
				wFLOAT BL1 = 0;
				wFLOAT BL2 = 0;

				wBOOL IsSpecial1Down = false; // Share
				wBOOL IsSpecial2Down = false; // Options
				wBOOL IsSpecial3Down = false; // Main Menu
};

wBOOL PollInput(gamepad* Result, const wI32& gamepadNum = 0 ) {
				
				// Get input from somewhere
				// Update Gamepad Object
				
				if (false) {
								return false; // Return false if cannot get input
				}

				return true;
}

wU32 LoadShaderProgram(wrp::ProgramContext* Ctx) {
				GladGLContext& gl = Ctx->OpenGL.gl;
				wrp::Logging& Log = Ctx->Log;
				wrp::System_Functions& OS = Ctx->OS;

				wU32 shaderProgram = gl.CreateProgram();
				
				wSTRING vertexShaderSource;
				if (!OS.GetFileContent("../Shaders/vertex.glsl", vertexShaderSource)) {
								Log.Err("Failed to load vertex shader file.", __LINE__, __FILE_NAME__, __FUNCTION__);
								return 0;
				}
				const char* src = vertexShaderSource.c_str();

				wU32 vertexShader = gl.CreateShader(GL_VERTEX_SHADER);
				gl.ShaderSource(vertexShader, 1, &src, NULL);
				gl.CompileShader(vertexShader);

				wI32 suceess;
				gl.GetShaderiv(vertexShader, GL_COMPILE_STATUS, &suceess);
				if (!suceess) {
								wSTRING infoLog;
								infoLog.reserve(512);
								gl.GetShaderInfoLog(vertexShader, 512, NULL, infoLog.data());
								Log.Err("Failed to compile vertex shader.", infoLog, __LINE__, __FILE_NAME__, __FUNCTION__);
								return 0;
				}
				
				wSTRING fragmentShaderSource;
				if (!OS.GetFileContent("../Shaders/fragment.glsl", fragmentShaderSource)) {
								Log.Err("Failed to load fragment shader file.", __LINE__, __FILE_NAME__, __FUNCTION__);
								return 0;
				}
				src = fragmentShaderSource.c_str();
				wU32 fragmentShader = gl.CreateShader(GL_FRAGMENT_SHADER);
				gl.ShaderSource(fragmentShader, 1, &src, NULL);
				gl.CompileShader(fragmentShader);

				gl.GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &suceess);
				if (!suceess) {
								wSTRING infoLog;
								infoLog.reserve(512);
								gl.GetShaderInfoLog(fragmentShader, 512, NULL, infoLog.data());
								Log.Err("Failed to compile fragment shader.", infoLog, __LINE__, __FILE_NAME__, __FUNCTION__);
								return 0;
				}
								
				gl.AttachShader(shaderProgram, vertexShader);
				gl.AttachShader(shaderProgram, fragmentShader);
				gl.LinkProgram(shaderProgram);

								
				gl.GetProgramiv(shaderProgram, GL_LINK_STATUS, &suceess);
				if (!suceess) {
								wSTRING infoLog;
								infoLog.reserve(512);
								gl.GetProgramInfoLog(shaderProgram, 512, NULL, infoLog.data());
								Log.Err("Failed to link shader program.", infoLog, __LINE__, __FILE_NAME__, __FUNCTION__);
								return 0;
				}

				gl.DeleteShader(vertexShader);
				gl.DeleteShader(fragmentShader);
				
				return shaderProgram;
}

void wrp::MAIN(wrp::ProgramContext* Ctx) {
				
				#define gl Ctx->OpenGL.gl
				#define Render Ctx->OpenGL

				wU32 ShaderProgram = LoadShaderProgram(Ctx);
				if (ShaderProgram == 0) return;
				gl.UseProgram(ShaderProgram);
				
				Vec4 BackgColor = {0.2, 0.3, 0.3, 1.0};
				
				wFLOAT vertices[] = {
								-0.5, -0.5, 0.0,
								0.5,  -0.5, 0.0,
								-0.5, -0.5, 0.0,
								-0.5,  0.5, 0.0
				};

				wU32 indices[] = {
								0, 1, 3,
								1, 2, 3
				};

				wU32 VBO;
				gl.GenBuffers(1,&VBO);
				
				wU32 VAO;
				gl.GenVertexArrays(1, &VAO);
				gl.BindVertexArray(VAO);
				
				gl.BindBuffer(GL_ARRAY_BUFFER, VBO);
				gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(wFLOAT), nullptr);
				gl.EnableVertexAttribArray(0);				


				std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

				while (Ctx->ShouldBeRunning) {
								
								Ctx->PollSystem(Ctx);

								gl.ClearColor(BackgColor.x, BackgColor.y, BackgColor.z, BackgColor.w);
								gl.Clear(GL_COLOR_BUFFER_BIT);
								
								gl.DrawArrays(GL_TRIANGLES, 0, 3);

								Render.SwapBuffers();

								std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();

								if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() > 60) Ctx->ShouldBeRunning = false;
				}

				//gamepad Gamepad;


				/*while (true) {

								std::cout << "\033[34;1mL: " << Gamepad.JLX << ", " << Gamepad.JLY << "\033[0m\n";
								std::cout << "\033[31;1mR: " << Gamepad.JRX << ", " << Gamepad.JRY << "\033[0m\n";

				}*/
				

}
