#include "menu.h"

// Initialize the static member to nullptr
Menu* Menu::menu_instance_ptr = nullptr;

Menu::Menu(){
} 

void Menu::load(){

  Session temp(cfg.get_session_name().c_str());
  session = temp;
  std::cout<<"Menu loading -->> "<<session.get_name()<<std::endl;


  interface.listen(&Menu::_notify_menu, *this, &Session::notify_session,session);

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

void Menu::_notify_menu(Control msg, bool isHold){
  // std::cout<<"~Menu~::_notify::Control,bool "<<msg<<","<<isHold<<std::endl;  
  // NO HOLD OPERATIONS DEFINED YET...
  edit(msg);
}

void Menu::edit(Control trigger){
    //3 menu buttons defined : SAVE_SESSION - PREV_SESSION - NEXT_SESSION
    if (trigger == SAVE_SESSION)
        save_session();
    else change_session(trigger);
}


void Menu::save_session(){
// 2 things here
// save config
//save session (tracks in channels , saved jams ..)
    cfg.save();
    std::cout<<"Menu::save_session"<<std::endl;

}

void Menu::change_session(Control trigger){
// 2 things here:
// change config 
// reset/load session

// change session
    int max_sessions = cfg.get_max_sessions();
    int current_session = cfg.get_curr_session();
    int next_session;

    if (trigger == NEXT_SESSION)
    {
      next_session = current_session+1;
      if (next_session > max_sessions) next_session = 1;
    }
    else if (trigger == PREV_SESSION)
    {
      next_session = current_session-1;
      if (next_session==0) next_session = max_sessions;
    } 

// SKILLPOINT
// handle session things --> implement session first
//     Session temp(cfg.get_session_name().c_str());
//     session = temp;
//     std::cout<<"Menu loading -->> "<<session.get_name()<<std::endl;

    cfg.open(next_session);
    cfg.display();
    std::cout<<"Menu::change_session : Session changed from "<<current_session<<" to "<<next_session<<std::endl;   

}
