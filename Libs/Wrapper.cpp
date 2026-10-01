#include "../Include/Wrapper.h"

void wrp::_MAIN(wrp::ProgramContext* pc) {
				wrp::MAIN(pc);
				pc->IsAlive = false;
}

