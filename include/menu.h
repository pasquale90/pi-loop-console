#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED


class Menu{
    public:
        
        static Menu& getInstance(){
            static Menu instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
            return instance;
        }

        Menu(Menu const&) = delete;
        void operator=(Menu const&) = delete;

        void load_session(); // load a new config / clear and reset session menu etc..
        
    private:

        Menu() {}                    // Constructor? (the {} brackets) are needed here.
        
};



#endif