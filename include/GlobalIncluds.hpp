#pragma once
#ifndef GLOBAL_INCLUDE
#define GLOBAL_INCLUDE "GLOBAL_INCLUDE"

#ifdef _WIN32
    #include <direct.h>
    #define STR_TYPE std::wstring
#else
    #include "unistd.h"
    #define STR_TYPE std::string
#endif

#include <regex>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <experimental/filesystem>
#include <ctime>
#include <algorithm>
#include <fcntl.h>
#include <stdio.h>
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

struct FileDate{
    public:
    std::string tmp_file_name;
    std::string file_path;
    std::size_t pos_begin = 0;
    std::size_t pos_end = 0;
    FileDate(){};
    ~FileDate(){};
};

namespace var{
    #ifdef WIN32
        inline std::wstring ask_user = L"Введите путь к месту хранения настроек: ";
        inline std::wtring remove_file_failed = "Удаление файла не удалось, удалите его сами.";
        inline std::wstring file_patched = "Файл изменён.";
        inline std::wstring say_goodby = "Файл изменён. Наслаждайтесь!";
    #else
        inline std::string ask_user = "Введите путь к месту хранения настроек: ";
        inline std::string remove_file_failed = "Удаление файла не удалось, удалите его сами.";
        inline std::string file_patched = "Файл изменён.";
        inline std::string say_goodby = "Файл изменён. Наслаждайтесь!";
    #endif // WIN32
    inline std::vector<std::string> use_plagin = {
        "natizyskunk.sftp",
        "liximomo.sftp"
    };
    inline std::vector<std::string> search_begin = {
        "function d(e){return Object.assign({},h,e)}", // for natizyskunk.sftp
        "function p(e){return Object.assign({},h,e)}" // for liximomo.sftp
    };
}

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
    
    inline void getLineCin(std::wstring &in_str){
        std::getline(std::wcin, in_str);
    }
    inline void getLineCin(std::string &in_str){
        std::getline(std::cin, in_str);
    }

    inline void printString(const wchar_t* str_to_print){
        std::wcout << str_to_print <<std::endl;
    }
    inline void printString(std::wstring str_to_print){
        std::wcout << str_to_print <<std::endl;
    }
    inline void printString(std::string str_to_print){
        std::cout << str_to_print <<std::endl;
    }
    inline void printString(std::exception str_to_print){
        #ifdef WIN32
            std::wcout << str_to_print.what() <<std::endl;
        #else
            std::cout << str_to_print.what() <<std::endl;
        #endif // WIN32
        
    }
}


#endif // !GLOBAL_INCLUDE