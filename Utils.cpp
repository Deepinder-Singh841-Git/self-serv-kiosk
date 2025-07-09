// Utils.cpp
#include "Utils.h"
#include <iostream>
using namespace std;

namespace seneca {
    Utils ut;

    char* Utils::alocpy(const char* src) const {
        char* des{};
        return alocpy(des, src);
    }

    char* Utils::alocpy(char*& des, const char* src) const {
        delete[] des;
        des = nullptr;
        if (src && *src) {
            des = new char[strlen(src) + 1];
            strcpy(des, src);
        }
        return des;
    }

    char* Utils::strcpy(char* des, const char* src) const {
        int i;
        for (i = 0; src[i]; i++) des[i] = src[i];
        des[i] = char(0);
        return des;
    }

    char* Utils::strcpy(char* des, const char* src, int len) const {
        int i;
        for (i = 0; i < len && src[i]; i++) des[i] = src[i];
        des[i] = 0;
        return des;
    }

    int Utils::strlen(const char* str) const {
        int len;
        for (len = 0; str && str[len]; len++);
        return len;
    }

    const char* Utils::ltrim(const char* str) const {
        while (str && *str && isspace(*str)) {
            str++;
        }
        return str;
    }

    int Utils::getInt() {
        int num;
        bool done = false;
        do {
            cin >> num;
            if (cin) {
                if (cin.peek() != '\n') {
                    cout << "Only an integer please:\n> ";
                }
                else {
                    done = true;
                }
            }
            else {
                if (cin.eof()) {
                    cin.clear();
                    cout << "You must enter a value:\n> ";
                }
                else {
                    cin.clear();
                    cout << "Invalid integer:\n> ";
                }
            }
            cin.ignore(100000, '\n');
        } while (!done);
        return num;
    }

    int Utils::getInt(int min, int max) {
        int num;
        bool done = false;
        do {
            num = getInt();
            if (num < min || num > max) {
                cout << "Invalid value: [" << min << " <= value <= " << max << "], try again:\n> ";
            }
            else {
                done = true;
            }
        } while (!done);
        return num;
    }

    char* Utils::makeBillFileName(char* filename, size_t billNo) const {
        char billName[32] = "bill_";
        size_t temp = billNo;
        int cnt = 5;
        int length;

        do {
            cnt++;
            temp /= 10;
        } while (temp > 0);
        length = cnt;

        while (billNo > 0) {
            billName[--cnt] = (billNo % 10) + '0';
            billNo /= 10;
        }
        if (billName[cnt - 1] == '\0') {
            billName[--cnt] = '0';
        }
        
        strcpy(&billName[length], ".txt");
        strcpy(filename, billName);
        return filename;
    }

    char* Utils::read(char*& toRet, std::istream& istr, char delimiter) const {
        bool done = false;
        char* cstr = new char[128];
        toRet = cstr;
        int size = 128;
        int nullindex = 1;
        
        do {
            istr.getline(cstr, 128, delimiter);
            if (istr.fail()) {
                char* temp = new char[size + 128];
                strcpy(temp, toRet);
                delete[] toRet;
                toRet = temp;
                cstr = &toRet[size - nullindex++];
                size += 128;
                istr.clear();
            }
            else {
                done = true;
            }
        } while (!done);
        
        char* temp = new char[strlen(toRet) + 1];
        strcpy(temp, toRet);
        delete[] toRet;
        toRet = temp;
        return toRet;
    }

    bool Utils::isspace(char ch) const {
        return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' || ch == '\r';
    }

    bool Utils::isspace(const char* cstring) const {
        if (!cstring) return false;
        while (*cstring && isspace(*cstring)) {
            cstring++;
        }
        return *cstring == 0;
    }
}
