#include "Timer.h"
#include "SDL2/SDL.h"


Timer* Timer::s_instance = nullptr;


Timer::Timer() {
    m_LastTime = SDL_GetTicks(); 
    m_DeltaTime = 0.0f;
}


void Timer::Tick(){
    m_DeltaTime = (SDL_GetTicks() - m_LastTime)*(TARGET_FPS/1000.0f);
    
    if (m_DeltaTime >TARGET_DELTATIME){
        m_DeltaTime = TARGET_DELTATIME;
    }
    
    m_LastTime = SDL_GetTicks();
}
