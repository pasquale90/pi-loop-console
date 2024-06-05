#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include "monitor.h"
#include "looper.h"
#include "mixer.h"
#include <atomic>

class Session{
    public:
        Session();
        
        void load(); // calls monitor.setup_audio() --> rename to setup()

        void save();
        void migrate(int);
        void evacuate(); // rename to stop_running???
        void notify_session(Control, bool);

        void set_name(const char*);
        const char* get_name();

    private:
        std::atomic<bool> is_running,is_firsTime,re_initialize;
        Monitor monitor;
        Looper looper;
        Mixer mixer;

        Config& cfg = Config::getInstance();
        
        void _update_buffers(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS]);

        // void reset2defaults();
        
};

#endif