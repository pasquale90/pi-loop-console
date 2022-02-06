#include <iostream>
#include <unistd.h>

static bool ON=0;

bool toggle_(){
    return ON=!ON;
}

int main(){

    std::string input;
    
    bool val=0;
    while(1){
        std::cin>>input;
        if(input=="P"){
            toggle_();
        }
        std::cout<<"Current_value =="<<ON<<std::endl;

        usleep(500);
    }
    return 0;
}
