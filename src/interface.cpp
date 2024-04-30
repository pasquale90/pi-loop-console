#include "interface.h"

hardwareInterface* hardwareInterface::interface_instance_ptr = nullptr;

hardwareInterface::hardwareInterface(){
}

hardwareInterface& hardwareInterface::getInstance() {
  if (interface_instance_ptr == nullptr) {
    interface_instance_ptr = new hardwareInterface(); // Create the instance if it doesn't exist
  }
  return *interface_instance_ptr;  // Dereference the pointer to return the instance
}

//WORKS
// void hardwareInterface::listen(void (*notify_menu)(Control,bool)){
//     Control trigger;
//     bool isHold;
//     while(true){
//         ui.listen_user(event_occured,msg);
//         if (event_occured){            
//             parse_msg(trigger,isHold); // parse the message
//             // std::cout<<"Interface::listen::msg "<<msg<<" --> trigger="<<trigger<<", isHold="<<isHold<<std::endl;
//             if(trigger==PREV_SESSION || trigger == NEXT_SESSION || trigger == SAVE_SESSION)
//                 // _notify_menu(trigger,isHold);
//                 (*notify_menu)(trigger,isHold);
//             else
//                 _notify_session(trigger,isHold);
//         }
//     }   
// }

//ATTEMPT 2
void hardwareInterface::listen(void (Menu::*_notify_menu)(Control, bool), Menu& a){
    Control trigger;
    bool isHold;
    while(true){
        ui.listen_user(event_occured,msg);
        if (event_occured){            
            parse_msg(trigger,isHold); // parse the message
            // std::cout<<"Interface::listen::msg "<<msg<<" --> trigger="<<trigger<<", isHold="<<isHold<<std::endl;
            if(trigger==PREV_SESSION || trigger == NEXT_SESSION || trigger == SAVE_SESSION)
                // _notify_menu(trigger,isHold);
                // (*notify_menu)(trigger,isHold);
                (a.*_notify_menu)(trigger,isHold);
            else
                _notify_session(trigger,isHold);
        }
    }   
}

void hardwareInterface::parse_msg(Control& trigger, bool& isHold){
    trigger = Control(msg%100);
    isHold = msg/100;
    // std::cout<<"Interface::parse_msg::msg "<<msg<<" --> trigger="<<trigger<<", isHold="<<isHold<<std::endl;
}


// int hardwareInterface::_notify_menu(Control trigger,bool isHold){
//     std::cout<<"notify menu with message trigger,isHold : "<<trigger<<","<<isHold<<std::endl;
// }

int hardwareInterface::_notify_session(Control trigger,bool isHold){
    std::cout<<"notify session with message trigger,isHold : "<<trigger<<","<<isHold<<std::endl;
}