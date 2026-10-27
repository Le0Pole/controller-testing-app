#include "../Include/Wrapper.h"

#ifdef __WIN32__

void wrp::_MAIN(wrp::ProgramContext* pc, wrp::PlatformContext* PlatformCtx) {
				
				if (!PlatformCtx->BindContext(true)) {
								pc->Log.Err("Failed to bind to context", __LINE__,__FILE_NAME__,__FUNCTION__);
								goto end;
				}

				wrp::MAIN(pc);
				
				end:

				pc->IsAlive = false;

}

#endif

#ifndef __WIN32__ 
				
void wrp::_MAIN(wrp::ProgramContext* pc) {
				wrp::MAIN(pc);
				pc->IsAlive = false;
}

#endif
