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
    std::string tmp_file_name;
    std::string current_patch;
    
public:
    GetFileTarget();
    /** Определить пользователя и перейти к нему в home дирикторию*/
    void jumpToDirectory();
    /** Найти нужный файл для правки и найти место правки*/
    FileDate getFilePosition();
    void setMode();
};

#endif // !GET_FILE_TARGET