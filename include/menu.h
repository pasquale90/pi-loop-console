#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

class Menu {
    public:
        static Menu& getInstance();

        // Add your public members and methods here
        // ...
        int eraseme;

    private:
        Menu();
        Menu(const Menu&) = delete;
        void operator=(const Menu&) = delete;

        static Menu* menu_instance_ptr;
};

#endif