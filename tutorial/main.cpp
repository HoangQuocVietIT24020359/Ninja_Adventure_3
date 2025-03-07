#include "iostream"
#include"SDL2/SDL.h"
#include"Engine.h"
#include "Timer.h"
#include "menu.h"
using namespace std;
    
int main(int argc, char **argv)
{
    Engine::GetInstance()->Init();
    
    while(Engine::GetInstance()->IsRunning())
    {
        Engine::GetInstance()->Events();
        Engine::GetInstance()->Update();
        Engine::GetInstance()->Render();
        Timer::GetInstance()->Tick();
    }
    Engine::GetInstance()->Clean();
    return 0;
}
