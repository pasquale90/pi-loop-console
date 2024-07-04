#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include "monitor.h"
#include "looper.h"
#include "mixer.h"
#include "config.h"
#ifdef REL
    #include "gpio/leds.h"
#endif

class Session{
    public:
        Session();
        
        void load(); // calls monitor.setup_audio() --> rename to setup()
        
        void save();
        void migrate(int);
        void evacuate(); // rename to stop_running???
        void notify_session(Control, bool);

        void save_jam();

        void set_name(const char*);
        const char* get_name();

    private:
        Monitor monitor;
        Looper looper;
        Mixer mixer;
#ifdef REL
        Leds leds;
#endif
        Config& cfg = Config::getInstance();
        
        std::atomic<bool> is_running,is_firsTime,re_initialize;

        void _update_buffers(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS]);
        
        std::string _parse_time_val(int);
        std::string _get_datetime();
        // void reset2defaults();
        
        void _initialize_leds();
};

#endif