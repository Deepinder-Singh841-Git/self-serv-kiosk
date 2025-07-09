// Menu.h
#ifndef SENECA_MENU_H
#define SENECA_MENU_H
#include <iostream>
#include "constants.h"

namespace seneca {
    class Menu;

    class MenuItem {
        friend class Menu;
        char* m_content{nullptr};
        unsigned int m_numOfindendations{0};
        unsigned int m_indentationSize{0};
        int m_row{0};

        MenuItem(const char* content, unsigned int numOfindendations, unsigned int indentationSize, int row);
        ~MenuItem();
        
        MenuItem(const MenuItem& item) = delete;
        MenuItem& operator=(const MenuItem& item) = delete;
        
        operator bool() const;
        bool isValid() const;
        void setEmpty();
        std::ostream& display(std::ostream& ostr = std::cout) const;
    };

    class Menu {
        unsigned int m_numOfindendations{0};
        unsigned int m_indentationSize{3};
        MenuItem m_title;
        MenuItem m_exitOption;
        MenuItem m_selection;
        unsigned int m_numMenuItem{0};
        MenuItem* m_menuItems[MaximumNumberOfMenuItems];

    public:
        Menu& operator=(const Menu& other) = delete;
        Menu(const Menu& other) = delete;
        Menu(const char* title, const char* exit = "Exit", unsigned int indendation = 0, unsigned int size = 3);
        ~Menu();
        
        Menu& operator<<(const char* content);
        friend size_t operator<<(std::ostream& ostr, const Menu& m);
        size_t select() const;
    };
}

#endif // !SENECA_MENU_H