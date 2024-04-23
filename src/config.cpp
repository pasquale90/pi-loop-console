#include "config.h"

// Initialize the static member to nullptr
Config* Config::cfg_instance_ptr = nullptr;


Config::Config(){
    eraseme=54;
} 

Config& Config::getInstance() {
  if (cfg_instance_ptr == nullptr) {
    cfg_instance_ptr = new Config(); // Create the instance if it doesn't exist
  }
  return *cfg_instance_ptr;  // Dereference the pointer to return the instance
}

// Add definitions for your public members and methods here
// ...