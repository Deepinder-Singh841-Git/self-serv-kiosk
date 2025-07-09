// Ordering.cpp
#include "Ordering.h"
#include "Menu.h"
#include "Utils.h"
#include <fstream>
#include <iomanip>
using namespace std;

namespace seneca {
    size_t Ordering::countRecords(const char* file) const {
        size_t count = 0;
        if (file) {
            ifstream infile(file);
            if (infile.is_open()) {
                char ch;
                while (infile.get(ch)) {
                    if (ch == '\n') count++;
                }
            }
        }
        return count;
    }

    Ordering::Ordering(const char* drinks, const char* foods) {
        // Initialize bill array
        for (size_t i = 0; i < MaximumNumberOfBillItems; i++) {
            m_bill[i] = nullptr;
        }

        if (drinks && foods) {
            size_t drinkCount = countRecords(drinks);
            size_t foodCount = countRecords(foods);

            if (drinkCount > 0 && foodCount > 0) {
                ifstream drinkFile(drinks);
                ifstream foodFile(foods);

                if (drinkFile.is_open() && foodFile.is_open()) {
                    m_drinks = new Drink[drinkCount];
                    m_foods = new Food[foodCount];

                    bool success = true;
                    size_t i;

                    // Read drinks
                    for (i = 0; i < drinkCount && drinkFile.good(); i++) {
                        if (!m_drinks[i].read(drinkFile)) {
                            success = false;
                            break;
                        }
                    }
                    m_drinkItems = i;

                    // Read foods
                    if (success) {
                        for (i = 0; i < foodCount && foodFile.good(); i++) {
                            if (!m_foods[i].read(foodFile)) {
                                success = false;
                                break;
                            }
                        }
                        m_foodItems = i;
                    }

                    if (!success) {
                        delete[] m_drinks;
                        delete[] m_foods;
                        m_drinks = nullptr;
                        m_foods = nullptr;
                        m_foodItems = 0;
                        m_drinkItems = 0;
                    }
                }
            }
        }
    }

    Ordering::~Ordering() {
        delete[] m_drinks;
        delete[] m_foods;
        for (size_t i = 0; i < m_billItems; i++) {
            delete m_bill[i];
        }
    }

    Ordering::operator bool() const {
        return m_drinks && m_foods;
    }

    size_t Ordering::noOfBillItems() const {
        return m_billItems;
    }

    bool Ordering::hasUnsavedBill() const {
        return m_billItems > 0;
    }

    void Ordering::billTitle(std::ostream& ostr) const {
        ostr << "Bill # " << setfill('0') << setw(3) << m_billNumber 
             << " =============================" << endl;
    }

    void Ordering::printTotals(std::ostream& ostr, double total) const {
        double tax = total * Tax;
        double totalWithTax = total + tax;

        ostr << "                     Total:         " << fixed << setprecision(2) << total << endl;
        ostr << "                     Tax:           " << fixed << setprecision(2) << tax << endl;
        ostr << "                     Total+Tax:     " << fixed << setprecision(2) << totalWithTax << endl;
        ostr << "========================================" << endl;
    }

    void Ordering::listFoods() const {
        cout << "List Of Available Meals" << endl;
        cout << "========================================" << endl;
        for (size_t i = 0; i < m_foodItems; i++) {
            m_foods[i].print() << endl;
        }
        cout << "========================================" << endl;
    }

    void Ordering::listDrinks() const {
        cout << "List Of Available Drinks" << endl;
        cout << "========================================" << endl;
        for (size_t i = 0; i < m_drinkItems; i++) {
            m_drinks[i].print() << endl;
        }
        cout << "========================================" << endl;
    }

    void Ordering::orderFood() {
        Menu foodMenu("Food Menu", "Back to Order", 2);
        for (size_t i = 0; i < m_foodItems; i++) {
            foodMenu << (const char*)m_foods[i];
        }

        size_t selection = foodMenu.select();
        if (selection && selection <= m_foodItems && m_billItems < MaximumNumberOfBillItems) {
            m_bill[m_billItems] = new Food(m_foods[selection - 1]);
            if (m_bill[m_billItems]->order()) {
                m_billItems++;
            } else {
                delete m_bill[m_billItems];
                m_bill[m_billItems] = nullptr;
            }
        }
    }

    void Ordering::orderDrink() {
        Menu drinkMenu("Drink Menu", "Back to Order", 2);
        for (size_t i = 0; i < m_drinkItems; i++) {
            drinkMenu << (const char*)m_drinks[i];
        }

        size_t selection = drinkMenu.select();
        if (selection && selection <= m_drinkItems && m_billItems < MaximumNumberOfBillItems) {
            m_bill[m_billItems] = new Drink(m_drinks[selection - 1]);
            if (m_bill[m_billItems]->order()) {
                m_billItems++;
            } else {
                delete m_bill[m_billItems];
                m_bill[m_billItems] = nullptr;
            }
        }
    }

    void Ordering::printBill(std::ostream& ostr) const {
        double total = 0;
        billTitle(ostr);
        for (size_t i = 0; i < m_billItems; i++) {
            m_bill[i]->print(ostr) << endl;
            total += m_bill[i]->price();
        }
        printTotals(ostr, total);
    }

    void Ordering::resetBill() {
        char filename[256];
        ut.makeBillFileName(filename, m_billNumber);
        ofstream file(filename);
        if (file.is_open()) {
            printBill(file);
            cout << "Saved bill number " << m_billNumber << endl;
        }
        cout << "Starting bill number " << m_billNumber + 1 << endl;

        for (size_t i = 0; i < m_billItems; i++) {
            delete m_bill[i];
            m_bill[i] = nullptr;
        }
        m_billItems = 0;
        m_billNumber++;
    }
}