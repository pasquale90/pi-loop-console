#include "menu.h"

// Initialize the static member to nullptr
Menu* Menu::menu_instance_ptr = nullptr;


Menu::Menu(){
} 

void Menu::change_session(){
    
    cfg.display();
}

Menu& Menu::getInstance() {
  
  if (menu_instance_ptr == nullptr) {
    menu_instance_ptr = new Menu(); // Create the instance if it doesn't exist
  }
  return *menu_instance_ptr;  // Dereference the pointer to return the instance
}


// Config& cfg = Config::getInstance();
// cfg.display();
// std::cout<<"############################################################\n\n"<<std::endl;
// cfg.open(1);
// cfg.display();

// std::cout<<"public access "<<cfg.audio_settings.bit_quantization<<std::endl;
// cfg.metronome.tempo = 100;
// cfg.save();
