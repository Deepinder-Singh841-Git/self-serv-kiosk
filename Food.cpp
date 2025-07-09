// Food.cpp
#include "Food.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

namespace seneca {
    Food::Food() : m_ordered(false), m_child(false), m_customize(nullptr) {}

    Food::~Food() {
        delete[] m_customize;
    }

    Food::Food(const Food& other) : Billable(other) {
        m_child = other.m_child;
        m_ordered = other.m_ordered;
        ut.alocpy(m_customize, other.m_customize);
    }

    Food& Food::operator=(const Food& other) {
        if (this != &other) {
            Billable::operator=(other);
            m_ordered = other.m_ordered;
            m_child = other.m_child;
            ut.alocpy(m_customize, other.m_customize);
        }
        return *this;
    }

    std::ostream& Food::print(std::ostream& ostr) const {
        const char* name = (const char*)*this;
        if (name) {
            ostr << name;
            
            int nameLen = ut.strlen(name);
            for (int i = nameLen; i < 28; i++) {
                ostr << '.';
            }
            
            if (ordered()) {
                ostr << (m_child ? "Child" : "Adult");
            } else {
                ostr << ".....";
            }
            
            ostr << " " << fixed << setprecision(2) << setw(6) << price();
            
            if (m_customize && &ostr == &cout) {
                ostr << " >> " << m_customize;
            }
        }
        return ostr;
    }

    bool Food::order() {
        cout << "         Food Size Selection\n"
             << "          1- Adult\n"
             << "          2- Child\n"
             << "          0- Back\n"
             << "         > ";
        
        int choice = ut.getInt(0, 2);
        
        if (choice != 0) {
            m_ordered = true;
            m_child = (choice == 2);
            
            cout << "Special instructions\n> ";
            char* temp = nullptr;
            ut.read(temp, cin);
            
            delete[] m_customize;
            m_customize = nullptr;
            
            if (temp && temp[0]) {
                ut.alocpy(m_customize, temp);
            }
            delete[] temp;
        } else {
            m_ordered = false;
            m_child = false;
            delete[] m_customize;
            m_customize = nullptr;
        }
        
        return m_ordered;
    }

    bool Food::ordered() const {
        return m_ordered;
    }

    double Food::price() const {
        double base = Billable::price();
        return (ordered() && m_child) ? base * 0.5 : base;
    }

    ifstream& Food::read(ifstream& file) {
        char buffer[1024];
        double priceValue;

        file.getline(buffer, 1024, ',');
        if (file) {
            file >> priceValue;
            file.ignore(1000, '\n');
            if (file) {
                name(buffer);
                Billable::price(priceValue);
                m_child = false;
                m_ordered = false;
                delete[] m_customize;
                m_customize = nullptr;
            }
        }
        return file;
    }
}