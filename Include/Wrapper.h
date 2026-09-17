#pragma once

namespace wrp {
				
				typedef int INT;
				typedef float FLOAT;
				typedef bool BOOL;
				typedef unsigned int UINT;

				struct ProgramContext {
								BOOL ShouldBeRunning = true;
				};

				INT MAIN(ProgramContext* Context);

}
