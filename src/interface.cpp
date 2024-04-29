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

void hardwareInterface::listen(){
    while(true){
        ui.listen_user(event_occured,msg);
        if (event_occured){
            std::cout<<"Interface::msg "<<msg<<std::endl;
            
            // parse the message
            // if(msg==CH1_RECDUB)
            //     ...
            
        }
    }
    
}