#include "menu.h"

Menu::Menu(){
} 

void Menu::set_metro_display(std::function<void(int)> f){
    session.set_metronome_display(f);
}

void Menu::set_display_initializer(std::function<void(int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1])> f){
    session.set_disp_initializer(f);
}

void Menu::setup(){
    session.setup();
}

void Menu::load_session(){
  session.load();
}

void Menu::unload(){
  session.evacuate();
}

void Menu::notify_menu(Trigger trigger, Response &response){

  // NO HOLD OPERATIONS DEFINED YET...
    if(trigger.control.load()!=PREV_SESSION && trigger.control.load() != NEXT_SESSION && trigger.control.load() != SAVE_SESSION)
      session.notify_session(trigger,response);
    else
      _edit(trigger.control.load(),response);
}

void Menu::_edit(Control control, Response &response){
    //3 menu buttons defined : SAVE_SESSION - PREV_SESSION - NEXT_SESSION + QUIT
    if (control == SAVE_SESSION){
        _save_session();
        // response.msg = SAVE_SESSION;    //unsupported yet
    }
    else {_change_session(control,response);}
}

void Menu::_save_session(){
// 2 things here : >
//    a) save config
//    b) save session (tracks in channels , saved jams ..)
// session.save is responsible for both
    session.save();
}

void Menu::_change_session(Control control, Response &response){
// 2 things here:
//    a)change config 
//    b)reset/load new session
// Menu::_change_session is responsible for a.
// session::migrate(int next_session) is responsible for loading the session using the new_session_id


// change config
    int max_sessions = cfg.get_max_sessions();
    int current_session = cfg.get_curr_session();
    int next_session;

    if (control == NEXT_SESSION)
    {
      next_session = current_session+1;
      if (next_session > max_sessions) next_session = 1;
      response.msg.store(SESSION_CHANGE);
      response.value.store(0);
      response.holder.store(next_session);
    }
    else if (control == PREV_SESSION)
    {
      next_session = current_session-1;
      if (next_session==0) next_session = max_sessions;
      response.msg.store(SESSION_CHANGE);
      response.value.store(1);
      response.holder.store(next_session);
    } 

// reset/load new session
    session.migrate(next_session);
}
