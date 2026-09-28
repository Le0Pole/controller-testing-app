#pragma once

#include <string>

namespace wrp {
				
				typedef int INT;
				typedef float FLOAT;
				typedef bool BOOL;
				typedef unsigned int UINT;
				
				struct OpenGL_Context {
								
				};


				struct Logging_Manager {

								enum LogType {
												LT_INFO = 1,
												LT_WARNING = 2,
												LT_ERROR = 3
								};
				
								Logging_Manager() { LOG_TYPES = LogType::LT_INFO; };
								Logging_Manager(LogType LoggingLevel) : LOG_TYPES(LoggingLevel) {};

								private:
				
								LogType LOG_TYPES; 
				
								public:

								void Info(std::string Info);
								void Warn(std::string Warning);
								void Err (std::string Error);

				};

				struct ProgramContext {
								BOOL ShouldBeRunning = true;
								BOOL IsAlive = true;

								OpenGL_Context OpenGL;
								Logging_Manager Log;

				};
				void MAIN(ProgramContext* Context);

				void _MAIN(ProgramContext* Context);

}
