#include "computer/keyboard.h"

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
static const char *eventDevice{STR(KEYBOARD_DEVICE)};
static const char *keycodes[64 * 1024] = { 0 }; // hack
//@TODO move eventDevice to configuring params.

// #define VERBOSE_KEYBOARD

Keyboard::Keyboard(){
    
    _setupKeyCodes();

    const int fd = open(eventDevice, O_RDONLY | O_NONBLOCK);

    if (fd < 0) errx(EXIT_FAILURE, "ERROR: cannot open device %s [%s]. To procced, set the keyboard device, re-configure and re-build the application again.", eventDevice, strerror(errno));
    
    err = libevdev_new_from_fd(fd, &dev);

    if (err < 0) errx(EXIT_FAILURE, "ERROR: cannot associate event device [%s]", strerror(-err));

    printf("Keyboard::Device %s is open and associated w/ libevent\n", eventDevice);

    // initialize time_pressed
    for (int i=1; i!=NUM_CONTROLS;++i)
        time_pressed[Control(i)] = 0;

}

void Keyboard::_setupKeyCodes()
{
    for (int i = 0; i < COUNTOF(keycodes); i++)
        keycodes[i] = "unknown";

    keycodes[KEY_Z] = "1";
    keycodes[KEY_X] = "2";
    keycodes[KEY_C] = "3";
    keycodes[KEY_V] = "4";
    keycodes[KEY_B] = "5";
    keycodes[KEY_N] = "6";
    keycodes[KEY_A] = "7";
    keycodes[KEY_S] = "8";
    keycodes[KEY_Q] = "9";
    keycodes[KEY_W] = "10";
    keycodes[KEY_R] = "11";
    keycodes[KEY_T] = "12";
    keycodes[KEY_1] = "13";
    keycodes[KEY_2] = "14";
    keycodes[KEY_3] = "15";
    keycodes[KEY_4] = "16";
    keycodes[KEY_5] = "17";
    keycodes[KEY_6] = "18";
    keycodes[KEY_P] = "19";
    keycodes[KEY_J] = "20";
    keycodes[KEY_L] = "21";
    keycodes[KEY_K] = "22";

    keycodes[KEY_F1] = "23";
    keycodes[KEY_F2] = "24";
    keycodes[KEY_F5] = "25";
    keycodes[KEY_F6] = "26";
    keycodes[KEY_F9] = "27";
    keycodes[KEY_F10] = "28";
    keycodes[KEY_ESC] = "29";
}

const char* Keyboard::_printableEventType(int type)
{
    switch (type)
    {
    STRCASE(EV_SYN);
    STRCASE(EV_KEY);
    STRCASE(EV_REL);
    STRCASE(EV_ABS);
    STRCASE(EV_MSC);
    STRCASE(EV_SW);
    STRCASE(EV_LED);
    STRCASE(EV_SND);
    STRCASE(EV_REP);
    STRCASE(EV_FF);
    STRCASE(EV_PWR);
    STRCASE(EV_FF_STATUS);
    default: return "-?-";
    }
}

void Keyboard::is_pressed(Trigger &trigger){
    // std::atomic<int> &msg, std::atomic<int> &tval


    err = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    // if (err == 0 && keycodes[ev.code]!="unknown"){
    if (err == 0 && strcmp( keycodes[ev.code], "unknown") != 0){
#if VERBOSE_KEYBOARD     
            printf("KEY: Value=%s; Code=%s; Time=%i\n ; ",
                _printableEventType(ev.value),
                    keycodes[ev.code]);
        // EV_SYN - EV_KEY - EV_REL
#endif
//@TODO EV_KEY === event_occured = true;

        if ( int(ev.value) == EV_SYN ) 
        {
            Control key = Control(atoi(keycodes[ev.code]));
            int pressed_time = (ev.time.tv_sec%100 )- time_pressed[key];
            // std::cout<<ev.time.tv_sec <<"-"<< time_pressed[key]<<" = "<<pressed_time<<std::endl;
            time_pressed[key] = 0; // reset;
            
            if (pressed_time >= 2)
                trigger.subval.store(1);
            else trigger.subval.store(0);
            trigger.control.store(key);

            return;
        }
        else if ( int(ev.value) == EV_KEY){
            // tv_sec instead of tv_usec
            time_pressed[Control(atoi(keycodes[ev.code]))] = ev.time.tv_sec%100; //store the last 3 digits            
        }
    }
    // msg = -1; //NULL 
	// tval = -1;
    return;
}