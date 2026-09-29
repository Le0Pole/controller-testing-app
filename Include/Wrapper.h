#pragma once

#include <string>

namespace wrp {
				
				typedef int INT;
				typedef float FLOAT;
				typedef bool BOOL;
				typedef unsigned int UINT;
				typedef std::string STRING;
				
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
								STRING Log;

								STRING (*ErrCallbackFunc)();
								STRING (*WarnCallbackFunc)();
								STRING (*InfoCallbackFunc)();

								public:
								
								INT  SaveLogToFile();
								void ChangeLogLevel(LogType LogLevel);

								void ErrSetCallbackForMoreInfo(STRING (*CallbackFunction)());
								void WarnSetCallbackForMoreInfo(STRING (*CallbackFunction)());
								void InfoSetCallbackForMoreInfo(STRING (*CallbackFunction)());

								void Info(STRING Info);
								void Info(STRING Info, INT Line);
								void Warn(STRING Warning);
								void Warn(STRING Warning, INT Line);
								void Err (STRING Error);
								void Err (STRING Error, INT Line);

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
