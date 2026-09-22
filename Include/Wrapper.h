#pragma once

namespace wrp {
				
				typedef int INT;
				typedef float FLOAT;
				typedef bool BOOL;
				typedef unsigned int UINT;
				
				struct OpenGL_Context {
																
				};

				struct ProgramContext {
								BOOL ShouldBeRunning = true;
								BOOL IsAlive = true;

								OpenGL_Context OpenGL;

				};

				void MAIN(ProgramContext* Context);

}
