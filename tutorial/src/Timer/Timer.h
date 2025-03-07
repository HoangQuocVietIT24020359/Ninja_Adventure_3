#ifndef TIMER_H
#define TIMER_H

const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 1.5f;


class Timer{
    public:
        void Tick();
        inline float GetDeltaTime(){return m_DeltaTime;}
        inline static Timer* GetInstance(){
            return s_instance = (s_instance != nullptr) ? s_instance : new Timer();
        }
        
    private:
        Timer();
        static Timer* s_instance;
        float m_DeltaTime;
        float m_LastTime;
};

#endif // TIMER_H

