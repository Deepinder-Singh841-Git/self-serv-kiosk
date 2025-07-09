// Utils.h
#ifndef SENECA_UTILS_H
#define SENECA_UTILS_H
#include <iostream>

namespace seneca {
    class Utils {
    public:
        char* alocpy(const char* src) const;
        char* alocpy(char*& des, const char* src) const;
        char* strcpy(char* des, const char* src) const;
        char* strcpy(char* des, const char* src, int len) const;
        int strlen(const char* str) const;
        const char* ltrim(const char* str) const;
        int getInt();
        int getInt(int min, int max);
        char* makeBillFileName(char* filename, size_t billNo) const;
        char* read(char*& toRet, std::istream& istr, char delimiter = '\n') const;
        bool isspace(char ch) const;
        bool isspace(const char* cstring) const;
    };
    
    extern Utils ut;
}

#endif // !SENECA_UTILS_H
