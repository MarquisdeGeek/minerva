#include <SDL/SDL.h>

int main() {
   if (SDL_Init(SDL_INIT_JOYSTICK) < 0){
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
   }

   SDL_JoystickEventState(SDL_ENABLE);
   SDL_Joystick *pJoystick = SDL_JoystickOpen(0);

   SDL_Event event;
   char joycmd[256];
   while(SDL_PollEvent(&event)) {  
      switch(event.type) { 
         case SDL_JOYBUTTONDOWN:
            sprintf(joycmd, "minx joy %d %d %d\n", 
                event.jbutton.which, event.jbutton.button, event.jbutton.state);
printf(joycmd);
            break;
      }
   }
   SDL_JoystickClose(pJoystick);

   return 0;
}


