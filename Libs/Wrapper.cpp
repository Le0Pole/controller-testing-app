#include "../Include/Wrapper.h"

#ifdef __WIN32__

void wrp::_MAIN(wrp::ProgramContext* pc, wrp::PlatformContext* PlatformCtx) {
				
				PlatformCtx->BindContext(true);

				wrp::MAIN(pc);
				pc->IsAlive = false;

}

#endif

#ifndef __WIN32__ 
				
void wrp::_MAIN(wrp::ProgramContext* pc) {
				wrp::MAIN(pc);
				pc->IsAlive = false;
}

#endif
