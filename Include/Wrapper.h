#pragma once

#include <chrono>
#include <filesystem>
#include <string>
#include <iostream>
#include <stdint.h>

namespace wrp {
				
				typedef int INT;
				typedef float FLOAT;

				typedef int8_t I8;
				typedef int16_t I16;
				typedef int32_t I32;
				typedef int64_t I64;
				typedef uint8_t U8;
				typedef uint16_t U16;
				typedef uint32_t U32;
				typedef uint64_t U64;

				typedef bool BOOL;
				typedef unsigned int UINT;
				typedef std::string STRING;
				
				struct OpenGL_Context {
								
				};


				struct Logging {

								enum LogType {
												LT_NONE = 0,
												LT_INFO = 1,
												LT_WARNING = 2,
												LT_ERROR = 3
								};
				
								Logging() { LogLevel = LogType::LT_ERROR; };
								Logging(LogType LoggingLevel) : LogLevel(LoggingLevel) {};
								
								BOOL RelativeLogTime = true;
								LogType LogLevel;

								private:
				
								std::chrono::time_point<std::chrono::high_resolution_clock> ProgStart = std::chrono::high_resolution_clock::now();
								
								STRING Log_History;

								STRING INFO_COLOR  = "\033[38;2;235;235;235m[";
								STRING WARN_COLOR  = "\033[38;2;216;167;21m[";
								STRING ERR_COLOR   = "\033[48;2;219;24;24;38;2;255;255;255m[";
								STRING CLEAR_COLOR = "\033[0m";								

								private:

								STRING NowTimestamp() {
												if (RelativeLogTime) {
																U64 NowMSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ProgStart).count() % 86400000;
								
																return std::to_string((NowMSeconds%3600000)/60000) + ":" + std::to_string((NowMSeconds%60000)/1000) + ":" + std::to_string(NowMSeconds%1000);
												} else {
																U64 NowMSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() % 86400000;
								
																return std::to_string(NowMSeconds/3600000) + ":" + std::to_string((NowMSeconds%3600000)/60000) + ":" + std::to_string((NowMSeconds%60000)/1000) + ":" + std::to_string(NowMSeconds%1000);
												}
								}

								void Log(STRING Color, STRING Log) {
												STRING Log_Str = Color + "[" + NowTimestamp() + "] " + Log;

												std::cout << Log_Str << CLEAR_COLOR << "\n";
								} 
								void LogEx(STRING Color, STRING Log, INT Line, STRING FileName, STRING FuncName) {
												STRING Prefix  = std::to_string(Line) + "] " + FileName + "::" + FuncName + "(): \""; 
												STRING Log_Str = Color + "[" + NowTimestamp() + " | " + Prefix + Log + "\"";
												
												std::cout << Log_Str << CLEAR_COLOR << "\n";
								}
								void LogAdd(STRING Color, STRING Log, STRING AdditionalInfo, INT Line, STRING FileName, STRING FuncName) {
												STRING Prefix  = std::to_string(Line) + "] " + FileName + "::" + FuncName + "(): \""; 
												STRING Log_Str = Color + "[" + NowTimestamp() + " | " + Prefix + Log + "\"";

												std::cout << Log_Str << ", additional information regardring the log: \"" + AdditionalInfo + "\"" << CLEAR_COLOR << "\n";
								}

								public:

								
								INT  SaveLogToFile() {
												return 0;
								};

								void Info(STRING Information) {
												if (LogLevel < LT_INFO) return; else Log(INFO_COLOR, Information);
								}
								void Info(STRING Information, INT Line, STRING FileName, STRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogEx(INFO_COLOR, Information, Line, FileName, FuncName);
								}
								void Info(STRING Information, STRING AdditionalInfo, INT Line, STRING FileName, STRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogAdd(INFO_COLOR, Information, AdditionalInfo, Line, FileName, FuncName);
								}
								void Warn(STRING Warning) {
												if (LogLevel < LT_INFO) return; else Log(WARN_COLOR, Warning);
								}
								void Warn(STRING Warning, INT Line, STRING FileName, STRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogEx(WARN_COLOR, Warning, Line, FileName, FuncName);
								}
								void Warn(STRING Warning, STRING AdditionalInfo, INT Line, STRING FileName, STRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogAdd(WARN_COLOR, Warning, AdditionalInfo, Line, FileName, FuncName);
								}
								void Err(STRING Error) {
												if (LogLevel < LT_INFO) return; else Log(ERR_COLOR, Error);
								}
								void Err(STRING Error, INT Line, STRING FileName, STRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogEx(ERR_COLOR, Error, Line, FileName, FuncName);
								}
								void Err(STRING Error, STRING AdditionalInfo, INT Line, STRING FileName, STRING FuncName) {
												if (LogLevel < LT_INFO) return; else LogAdd(ERR_COLOR, Error, AdditionalInfo, Line, FileName, FuncName);
								}

				};

				struct ProgramContext {
								BOOL ShouldBeRunning = true;
								BOOL IsAlive = true;

								OpenGL_Context OpenGL;
								Logging Log;

				};
				void MAIN(ProgramContext* Context);

				void _MAIN(ProgramContext* Context);

}
