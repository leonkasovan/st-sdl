#ifndef __InputHandler__
#define __InputHandler__

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif	

// keyboard events, True if Data Available.
bool input_update(SDL_Event *event);

void input_reset();
void input_resetSDLTextInput();
const char* input_getSequence();

#ifdef __cplusplus
}
#endif	

#endif
