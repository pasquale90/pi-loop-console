#include "menu.h"

// Initialize the static member to nullptr
Menu* Menu::menu_instance_ptr = nullptr;

Menu::Menu(){
} 

void Menu::load_session(){
  session.load();
}
// void Menu::load(){
  // Session temp(cfg.currSession_name.c_str());
  // session = temp;
  // std::cout<<"Menu loading -->> "<<session.get_name()<<std::endl;
  // interface.listen(&Menu::_notify_menu, *this, &Session::notify_session,session);
// }

Menu& Menu::getInstance() {
  
  if (menu_instance_ptr == nullptr) {
    menu_instance_ptr = new Menu(); // Create the instance if it doesn't exist
  }
  return *menu_instance_ptr;  // Dereference the pointer to return the instance
}

void Menu::unload(){
  session.evacuate();
  delete menu_instance_ptr;
  std::cout<<"Unload menu"<<std::endl;
}

void Menu::notify_menu(Control msg, bool isHold){
  // NO HOLD OPERATIONS DEFINED YET...
    if(msg!=PREV_SESSION && msg != NEXT_SESSION && msg != SAVE_SESSION)
      session.notify_session(msg,isHold);
    else
      _edit(msg);
}

void Menu::_edit(Control trigger){
    //3 menu buttons defined : SAVE_SESSION - PREV_SESSION - NEXT_SESSION + QUIT
    if (trigger == SAVE_SESSION)
        _save_session();
    // else if (trigger == SHUTDOWN_PILOOP)
        // terminate 
    else _change_session(trigger);
}

void Menu::_save_session(){
// 2 things here : 
//    a) save config
//    b) save session (tracks in channels , saved jams ..)
// session.save is responsible for both
    session.save();
}

void Menu::_change_session(Control trigger){
// 2 things here:
//    a)change config 
//    b)reset/load new session
// Menu::_change_session is responsible for a.
// session::migrate(int next_session) is responsible for loading the session using the new_session_id


// change config
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

// reset/load new session
    session.migrate(next_session);
}
