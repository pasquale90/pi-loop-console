#include "menu.h"

// Initialize the static member to nullptr
Menu* Menu::menu_instance_ptr = nullptr;


Menu::Menu(){
  eraseme=5;
} 

Menu& Menu::getInstance() {
  if (menu_instance_ptr == nullptr) {
    menu_instance_ptr = new Menu(); // Create the instance if it doesn't exist
  }
  return *menu_instance_ptr;  // Dereference the pointer to return the instance
}

// Add definitions for your public members and methods here
// ...