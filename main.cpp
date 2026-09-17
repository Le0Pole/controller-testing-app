#include "Include/Wrapper.h"
#include <thread>
#include <chrono>

using wrp::INT;
using wrp::UINT;
using wrp::FLOAT;
using wrp::BOOL;

struct gamepad {
				
				FLOAT JLX = 0; // Left Joystick X
				FLOAT JLY = 0; // Left Joystick Y
				FLOAT JRX = 0; // Right Joystick X
				FLOAT JRY = 0; // Right Joystick X
				
				FLOAT LJDeadZone = 0; // Dead Zone for the left Joystick
				FLOAT RJDeadZone = 0; // Dead Zone for the right Joystick

				BOOL IsLJdown = false;
				BOOL IsRJdown = false;

				/*
				 *    BL4       BR4
				 * BL3   BL2 BR3   BR2
				 *    BL1       BR1
				 */

				BOOL IsBL1Down = false;
				BOOL IsBL2Down = false;
				BOOL IsBL3Down = false;
				BOOL IsBL4Down = false;

				BOOL IsBR1Down = false;
				BOOL IsBR2Down = false;
				BOOL IsBR3Down = false;
				BOOL IsBR4Down = false;

				/*
				 *  BL2  BR2
				 *	BL1  BR1
				 */

				FLOAT BR1 = 0;
				FLOAT BR2 = 0;
				FLOAT BL1 = 0;
				FLOAT BL2 = 0;

				BOOL IsSpecial1Down = false; // Share
				BOOL IsSpecial2Down = false; // Options
				BOOL IsSpecial3Down = false; // Main Menu
};

BOOL PollInput(gamepad* Result, const INT& gamepadNum = 0 ) {
				
				// Get input from somewhere
				// Update Gamepad Object
				
				if (false) {
								return false; // Return false if cannot get input
				}

				return true;
}

void wrp::MAIN(wrp::ProgramContext* Context) {
				
				INT c = 0;
				while (Context->ShouldBeRunning) {
								c++;

								std::chrono::duration<FLOAT> second = std::chrono::seconds(1);
								std::this_thread::sleep_for(second);

								if (c == 10) Context->ShouldBeRunning = false;
				}

				//gamepad Gamepad;


				/*while (true) {

								std::cout << "\033[34;1mL: " << Gamepad.JLX << ", " << Gamepad.JLY << "\033[0m\n";
								std::cout << "\033[31;1mR: " << Gamepad.JRX << ", " << Gamepad.JRY << "\033[0m\n";

				}*/
				
				Context->IsAlive = false;

}
