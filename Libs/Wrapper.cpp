#include "../Include/Wrapper.h"

void wrp::_MAIN(wrp::ProgramContext* pc) {
				wrp::MAIN(pc);
				pc->IsAlive = false;
}

wrp::INT wrp::Logging_Manager::SaveLogToFile() {

}

void wrp::Logging_Manager::ChangeLogLevel(LogType LogLevel) {

}

void wrp::Logging_Manager::ErrSetCallbackForMoreInfo(STRING (*CallbackFunction)()) {

}

void wrp::Logging_Manager::WarnSetCallbackForMoreInfo(STRING (*CallbackFunction)()) {

}

void wrp::Logging_Manager::InfoSetCallbackForMoreInfo(STRING (*CallbackFunction)()) {

}


void wrp::Logging_Manager::Info(STRING Info) {

}

void wrp::Logging_Manager::Info(STRING Info, INT Line) {

}

void wrp::Logging_Manager::Warn(STRING Warning) {

}

void wrp::Logging_Manager::Warn(STRING Warning, INT Line) {

}

void wrp::Logging_Manager::Err (STRING Error) {

}

void wrp::Logging_Manager::Err (STRING Error, INT Line) {

}
