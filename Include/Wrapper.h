#pragma once

#include <chrono>
#include <filesystem>
#include <string>
#include <iostream>
#include <stdint.h>


#include "GLAD/gl.h"

namespace wrp {
				
				typedef float wFLOAT;

				typedef int8_t wI8;
				typedef int16_t wI16;
				typedef int32_t wI32;
				typedef int64_t wI64;
				typedef uint8_t wU8;
				typedef uint16_t wU16;
				typedef uint32_t wU32;
				typedef uint64_t wU64;

				typedef bool wBOOL;
				typedef std::string wSTRING;

				struct Logging {

								enum LogType {
												LT_INFO = 3,
												LT_WARNING = 2,
												LT_ERROR = 1,
												LT_NONE = 0
								};
				
								Logging() { LogLevel = LogType::LT_INFO; };
								Logging(LogType LoggingLevel) : LogLevel(LoggingLevel) {};
								
								wBOOL RelativeLogTime = true;
								LogType LogLevel;

								private:
				
								std::chrono::time_point<std::chrono::high_resolution_clock> ProgStart = std::chrono::high_resolution_clock::now();
								
								wSTRING Log_History;

								wSTRING INFO_COLOR  = "\033[38;2;235;235;235m";
								wSTRING WARN_COLOR  = "\033[38;2;216;167;21m";
								wSTRING ERR_COLOR   = "\033[48;2;219;24;24;38;2;255;255;255m";
								wSTRING CLEAR_COLOR = "\033[0m";								

								private:

								wSTRING NowTimestamp() {
												if (RelativeLogTime) {
																wU64 NowMSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ProgStart).count() % 86400000;
								
																return std::to_string((NowMSeconds%3600000)/60000) + ":" + std::to_string((NowMSeconds%60000)/1000) + ":" + std::to_string(NowMSeconds%1000);
												} else {
																wU64 NowMSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() % 86400000;
								
																return std::to_string(NowMSeconds/3600000) + ":" + std::to_string((NowMSeconds%3600000)/60000) + ":" + std::to_string((NowMSeconds%60000)/1000) + ":" + std::to_string(NowMSeconds%1000);
												}
								}

								void Log(wSTRING Color, wSTRING Log) {
												wSTRING Log_Str = Color + "[" + NowTimestamp() + "] " + Log;

												std::cout << Log_Str << CLEAR_COLOR << "\n";
								} 
								void LogEx(wSTRING Color, wSTRING Log,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												wSTRING Prefix  = std::to_string(Line) + "] " + FileName + "::" + FuncName + "(): \""; 
												wSTRING Log_Str = Color + "[" + NowTimestamp() + " | " + Prefix + Log + "\"";
												
												std::cout << Log_Str << CLEAR_COLOR << "\n";
								}
								void LogAdd(wSTRING Color, wSTRING Log, wSTRING AdditionalInfo,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												wSTRING Prefix  = std::to_string(Line) + "] " + FileName + "::" + FuncName + "(): \""; 
												wSTRING Log_Str = Color + "[" + NowTimestamp() + " | " + Prefix + Log + "\"";

												std::cout << Log_Str << ", additional information regardring the log: \"" + AdditionalInfo + "\"" << CLEAR_COLOR << "\n";
								}

								public:

								
								wI32 SaveLogToFile() {
												return 0;
								};

								void Info(wSTRING Information) {
												if (LogLevel < LT_INFO) return; else Log(INFO_COLOR, Information);
								}
								void Info(wSTRING Information,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogEx(INFO_COLOR, Information, Line, FileName, FuncName);
								}
								void Info(wSTRING Information, wSTRING AdditionalInfo,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogAdd(INFO_COLOR, Information, AdditionalInfo, Line, FileName, FuncName);
								}
								void Warn(wSTRING Warning) {
												if (LogLevel < LT_WARNING) return; else Log(WARN_COLOR, Warning);
								}
								void Warn(wSTRING Warning,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												if (LogLevel < LT_WARNING) return; else LogEx(WARN_COLOR, Warning, Line, FileName, FuncName);
								}
								void Warn(wSTRING Warning, wSTRING AdditionalInfo,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												if (LogLevel < LT_WARNING) return; else LogAdd(WARN_COLOR, Warning, AdditionalInfo, Line, FileName, FuncName);
								}
								void Err(wSTRING Error) {
												if (LogLevel < LT_ERROR) return; else Log(ERR_COLOR, Error);
								}
								void Err(wSTRING Error,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												if (LogLevel < LT_ERROR) return; else LogEx(ERR_COLOR, Error, Line, FileName, FuncName);
								}
								void Err(wSTRING Error, wSTRING AdditionalInfo,wI32 Line, wSTRING FileName, wSTRING FuncName) {
												if (LogLevel < LT_ERROR) return; else LogAdd(ERR_COLOR, Error, AdditionalInfo, Line, FileName, FuncName);
								}

				};

				struct OpenGL_Context {
								
								wBOOL SwapBuffers();
								
								GladGLContext gl = {};

				};
				
				struct ProgramContext {
								wBOOL ShouldBeRunning = true;
								wBOOL IsAlive = true;
								
								wI32 ExitVal = 0;

								OpenGL_Context OpenGL;
								Logging Log;

				};

				struct PlatformContext {
								
								#ifdef __WIN32__
								
								wBOOL BindContext(wBOOL BindUnbind /*Binds to context if true, unbinds if false.*/ );

								#endif // __WIN32__

				};

				void MAIN(ProgramContext* Context);

				void _MAIN(ProgramContext* Context, PlatformContext* PlatformCtx);

}
