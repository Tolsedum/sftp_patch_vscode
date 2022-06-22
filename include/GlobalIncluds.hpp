#pragma once
#ifndef GLOBAL_INCLUDE
#define GLOBAL_INCLUDE "GLOBAL_INCLUDE"

#ifdef _WIN32
    #include <direct.h>
    #include <fcntl.h>
    #define STR_TYPE std::wstring
    #define CHAR_TYPR wchar_t
#else
    #include "unistd.h"
    #define STR_TYPE std::string
    #define CHAR_TYPR char
#endif


// #include <ctime>
// #include <algorithm>
// #include <stdio.h>

#include <regex>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <experimental/filesystem>
#include <vector>

/**
 * @file GlobalIncluds.hpp
 * @author Tolsedum (tolsedum@gmail.com)
 * @brief Global include headers, var, structs and functins
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

namespace fs = std::experimental::filesystem;

struct GenerelInformation{
    public:
    std::string tmp_file_name;
    std::string file_path;
    std::size_t pos_begin = 0;
    std::size_t pos_end = 0;
    short selected_number;
    GenerelInformation(){};
    ~GenerelInformation(){};
};

namespace fn{
    #ifdef WIN32
        inline std::string wstringToString(std::wstring w_str_to_convert){
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> w_conv;
            std::string ret_value = w_conv.to_bytes(w_str_to_convert);
            return ret_value;
        }
        inline std::wstring stringToWstring(std::string str_to_convert){
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> w_conv;
            std::wstring ret_value = w_conv.from_bytes(str_to_convert);
            return ret_value;
        }
    #endif // WIN32
    
    // ----------------------- getLineCin -----------------------
    inline void getLineCin(std::wstring &in_str){
        std::wcin >> in_str;
    }
    inline void getLineCin(std::string &in_str){
        std::getline(std::cin, in_str);
    }
    // ----------------------- getLineCin end -------------------

    // ----------------------- printString -----------------------


    inline void printString(wchar_t str_to_print){
        std::wcout << str_to_print <<std::endl;
    }
    inline void printString(char str_to_print){
        std::cout << str_to_print <<std::endl;
    }
    inline void printString(wchar_t* str_to_print){
        std::wcout << str_to_print <<std::endl;
    }
    inline void printString(char* str_to_print){
        std::cout << str_to_print <<std::endl;
    }
    inline void printString(const wchar_t* str_to_print){
        std::wcout << str_to_print <<std::endl;
    }
    inline void printString(const char* str_to_print){
        std::cout << str_to_print <<std::endl;
    }
    inline void printString(std::wstring str_to_print){
        std::wcout << str_to_print <<std::endl;
    }
    inline void printString(std::string str_to_print){
        std::cout << str_to_print <<std::endl;
    }
    inline void printString(std::exception except){
        #ifdef WIN32
            std::wcout << except.what() <<std::endl;
        #else
            std::cout << except.what() <<std::endl;
        #endif // WIN32
    }
    // ----------------------- printString end---------------------

    inline void pause(){
        #ifdef WIN32
            system("PAUSE");
        #else
            std::cout<< "Введите символ и нажмите enter" <<std::endl;
            char d;
            std::cin >> d;
        #endif // WIN32
    }
}


#endif // !GLOBAL_INCLUDE