#include "keyboard.h"

UI* UI::ui_instance_ptr = nullptr;

UI::UI(){
} 

UI& UI::getInstance() {
  
  if (ui_instance_ptr == nullptr) {
    ui_instance_ptr = new UI(); // Create the instance if it doesn't exist
    ui_instance_ptr->initialize();
  }
  return *ui_instance_ptr;  // Dereference the pointer to return the instance
}

void UI::initialize(){
    setupKeyCodes();

    const int fd = open(eventDevice, O_RDONLY | O_NONBLOCK);

    if (fd < 0) errx(EXIT_FAILURE, "ERROR: cannot open device %s [%s]", eventDevice, strerror(errno));
    
    err = libevdev_new_from_fd(fd, &dev);

    if (err < 0) errx(EXIT_FAILURE, "ERROR: cannot associate event device [%s]", strerror(-err));

    printf("Keyboard::Device %s is open and associated w/ libevent\n", eventDevice);

    // initialize time_pressed
    for (int i=1; i!=NUM_CONTROLS;++i)
        time_pressed[Control(i)] = 0;

}

static void setupKeyCodes(void)
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
}

static const char* printableEventType(int t)
{
    switch (t)
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

void UI::listen_user_keyboard(std::atomic<bool> &event_occured, std::atomic<int> &msg){

    err = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
    if (err == 0 && keycodes[ev.code]!="unknown"){
        
        // printf("KEY: Value=%s; Code=%s; Time=%i\n ; ",
        //     printableEventType(ev.value),
        //     keycodes[ev.code]);
        // EV_SYN - EV_KEY - EV_REL

//@TODO EV_KEY === event_occured = true;

        if ( int(ev.value) == EV_SYN ) 
        {
            Control key = Control(atoi(keycodes[ev.code]));
            int pressed_time = (ev.time.tv_sec%100 )- time_pressed[key];
            // std::cout<<ev.time.tv_sec <<"-"<< time_pressed[key]<<" = "<<pressed_time<<std::endl;
            time_pressed[key] = 0; // reset;
            
            if (pressed_time>=2) msg = 100 + key; // if holded return 1xx 
            else msg = key;                         // else return 0xx
            event_occured = true;

            return;
        }
        else if ( int(ev.value) == EV_KEY){
            // tv_sec instead of tv_usec
            time_pressed[Control(atoi(keycodes[ev.code]))] = ev.time.tv_sec%100; //store the last 3 digits            
        }
    }
    event_occured = false;
    msg = NULL;
    return;
}

void UI::listen_user(std::atomic<bool> &event_occured, std::atomic<int> &msg){
    listen_user_keyboard(event_occured, msg); 
}
