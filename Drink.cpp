// Drink.cpp
#include "Drink.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

namespace seneca {
    ostream& Drink::print(ostream& ostr) const {
        const char* name = (const char*)*this;
        if (name) {
            ostr << name;
            
            int nameLen = ut.strlen(name);
            for (int i = nameLen; i < 28; i++) {
                ostr << '.';
            }
            
            if (ordered()) {
                switch (m_size) {
                    case 'S': ostr << "SML"; break;
                    case 'M': ostr << "MID"; break;
                    case 'L': ostr << "LRG"; break;
                    case 'X': ostr << "XLR"; break;
                }
                ostr << "..";
            } else {
                ostr << ".....";
            }
            
            ostr << " " << fixed << setprecision(2) << setw(6) << price();
        }
        return ostr;
    }

    bool Drink::order() {
        cout << "         Drink Size Selection\n"
             << "          1- Small\n"
             << "          2- Medium\n"
             << "          3- Large\n"
             << "          4- Extra Large\n"
             << "          0- Back\n"
             << "         > ";
        
        int choice = ut.getInt(0, 4);
        
        switch (choice) {
            case 1: m_size = 'S'; break;
            case 2: m_size = 'M'; break;
            case 3: m_size = 'L'; break;
            case 4: m_size = 'X'; break;
            case 0: m_size = 0; break;
        }
        
        return m_size != 0;
    }

    bool Drink::ordered() const {
        return m_size != 0;
    }

    ifstream& Drink::read(ifstream& file) {
        char buffer[1024];
        double priceValue;

        file.getline(buffer, 1024, ',');
        if (file) {
            file >> priceValue;
            file.ignore(1000, '\n');
            if (file) {
                name(buffer);
                Billable::price(priceValue);
                m_size = 0;
            }
        }
        return file;
    }

    double Drink::price() const {
        double base = Billable::price();
        
        if (!ordered() || m_size == 'L') {
            return base;
        }
        
        switch (m_size) {
            case 'S': return base * 0.5;
            case 'M': return base * 0.75;
            case 'X': return base * 1.5;
            default: return base;
        }
    }
}