// Menu.cpp
#include "Menu.h"
#include "Utils.h"
#include <iostream>
using namespace std;

namespace seneca {
    MenuItem::MenuItem(const char* content, unsigned int numOfindendations, unsigned int indentationSize, int row)
        : m_content(nullptr), m_numOfindendations(numOfindendations), m_indentationSize(indentationSize), m_row(row) {
        
        if (!content || ut.isspace(content) || numOfindendations > 4 || indentationSize > 4 || 
            (row > static_cast<int>(MaximumNumberOfMenuItems) && row != -1)) {
            setEmpty();
            return;
        }
        
        const char* trimmed = ut.ltrim(content);
        if (ut.strlen(trimmed) == 0) {
            setEmpty();
            return;
        }
        
        ut.alocpy(m_content, trimmed);
    }

    MenuItem::~MenuItem() {
        delete[] m_content;
    }

    MenuItem::operator bool() const {
        return isValid();
    }

    bool MenuItem::isValid() const {
        return m_content && !ut.isspace(m_content) && 
               m_numOfindendations <= 4 && m_indentationSize <= 4 &&
               (m_row <= static_cast<int>(MaximumNumberOfMenuItems) || m_row == -1);
    }

    void MenuItem::setEmpty() {
        m_content = nullptr;
        m_numOfindendations = 0;
        m_indentationSize = 0;
        m_row = 0;
    }

    ostream& MenuItem::display(ostream& ostr) const {
        if (!isValid()) {
            return ostr << "??????????";
        }

        // Print indentation
        for (unsigned i = 0; i < m_numOfindendations; ++i) {
            for (unsigned j = 0; j < m_indentationSize; ++j) {
                ostr << " ";
            }
        }

        // Print row number if >= 0
        if (m_row >= 0) {
            if (m_row > 9) {
                ostr << m_row << "- ";
            } else {
                ostr << " " << m_row << "- ";
            }
        }

        ostr << m_content;
        return ostr;
    }

    Menu::Menu(const char* title, const char* exit, unsigned int indentation, unsigned int size)
        : m_numOfindendations(indentation), m_indentationSize(size),
          m_title(title, indentation, size, -1),
          m_exitOption(exit, indentation, size, 0),
          m_selection("> ", indentation, size, -1),
          m_numMenuItem(0) {
        
        for (size_t i = 0; i < MaximumNumberOfMenuItems; i++) {
            m_menuItems[i] = nullptr;
        }
    }

    Menu& Menu::operator<<(const char* content) {
        if (m_numMenuItem < MaximumNumberOfMenuItems) {
            m_menuItems[m_numMenuItem] = new MenuItem(content, m_numOfindendations, m_indentationSize, m_numMenuItem + 1);
            m_numMenuItem++;
        }
        return *this;
    }

    Menu::~Menu() {
        for (size_t i = 0; i < m_numMenuItem; i++) {
            delete m_menuItems[i];
        }
    }

    size_t Menu::select() const {
        if (m_title) {
            m_title.display() << endl;
        }

        for (size_t i = 0; i < m_numMenuItem; i++) {
            if (m_menuItems[i]) {
                m_menuItems[i]->display() << endl;
            }
        }

        m_exitOption.display() << endl;
        m_selection.display();
        
        return ut.getInt(0, m_numMenuItem);
    }

    size_t operator<<(ostream& ostr, const Menu& m) {
        if (&ostr == &cout) {
            return m.select();
        }
        return 0;
    }
}
