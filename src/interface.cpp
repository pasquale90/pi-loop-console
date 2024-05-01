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

void hardwareInterface::listen(void (Menu::*_notify_menu)(Control, bool), Menu& m,    void (Session::*_notify_session)(Control, bool), Session& s){
    Control trigger;
    bool isHold;
    while(true){
        ui.listen_user(event_occured,msg);
        if (event_occured){            
            _parse_msg(trigger,isHold); // parse the message
            // std::cout<<"Interface::listen::msg "<<msg<<" --> trigger="<<trigger<<", isHold="<<isHold<<std::endl;
            if(trigger==PREV_SESSION || trigger == NEXT_SESSION || trigger == SAVE_SESSION)
                // _notify_menu(trigger,isHold);
                // (*notify_menu)(trigger,isHold);
                (m.*_notify_menu)(trigger,isHold);
            else
                (s.*_notify_session)(trigger,isHold);
                // _notify_session(trigger,isHold);
        }
    }   
}

void hardwareInterface::_parse_msg(Control& trigger, bool& isHold){
    trigger = Control(msg%100);
    isHold = msg/100;
    // std::cout<<"Interface::parse_msg::msg "<<msg<<" --> trigger="<<trigger<<", isHold="<<isHold<<std::endl;
}