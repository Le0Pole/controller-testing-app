#include "Include/Wrapper.h"
#include <string>
#include <thread>
#include <chrono>
#include <cmath>

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

void wrp::MAIN(wrp::ProgramContext* Ctx) {
				
				#define gl Ctx->OpenGL.gl
				#define Render Ctx->OpenGL

				Render.SwapBuffers();
								
				std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

				while (Ctx->ShouldBeRunning) {
								wFLOAT t = float(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() % 1500) / 1000;
								
								wFLOAT R = max(std::sin(2*t*PI),0.0f);
								wFLOAT G = std::sin(2*t*PI + PI);
								wFLOAT B = max(max(2*t*PI + PI/2, 0.0f), 2*t*PI + PI);

								gl.ClearColor(R, G, B, 1.0f);
								gl.Clear(GL_COLOR_BUFFER_BIT);
								
								Render.SwapBuffers();

								std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();

								if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() > 10) Ctx->ShouldBeRunning = false;
				}

				//gamepad Gamepad;


				/*while (true) {

								std::cout << "\033[34;1mL: " << Gamepad.JLX << ", " << Gamepad.JLY << "\033[0m\n";
								std::cout << "\033[31;1mR: " << Gamepad.JRX << ", " << Gamepad.JRY << "\033[0m\n";

				}*/
				

}
