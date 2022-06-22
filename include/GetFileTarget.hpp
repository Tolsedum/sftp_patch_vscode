#pragma once
#ifndef GET_FILE_TARGET
#define GET_FILE_TARGET "GET_FILE_TARGET"

#include "GlobalIncluds.hpp"

/**
 * @file GetFileTarget.hpp
 * @author Tolsedum (tolsedum@gmail.com)
 * @brief To go to the worcing dirictory
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

class GetFileTarget{
private:
    std::string name_file_target;
    std::string tmp_file_name;
    std::string current_patch;
    std::vector<std::string> use_plagin;
    std::vector<std::string> search_begin;
    short selected_number;
    #ifdef WIN32
        const wchar_t *ask_user_a_plagin;
        const wchar_t *invalid_argument;
        const wchar_t *out_of_range;
        const wchar_t *file_is_not_open;
        const wchar_t *file_is_not_found;
    #else
        const char *ask_user_a_plagin;
        const char *invalid_argument;
        const char *out_of_range;
        const char *file_is_not_open;
        const char *file_is_not_found;
    #endif // WIN32
    
    const char *profile_patch;
    void getIntCin();
public:
    GetFileTarget();
    /** Определить пользователя и перейти к нему в home дирикторию*/
    void jumpToDirectory();
    /** Найти нужный файл для правки и найти место правки*/
    GenerelInformation getFilePosition();
    void setMode();
    
};

#endif // !GET_FILE_TARGET