#pragma once
#ifndef FILE_REWRITE
#define FILE_REWRITE "FILE_REWRITE"
/**
 * @file FileRewrite.hpp
 * @author Tolsedum (tolsedum@gmail.com)
 * @brief For to change file
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "GlobalIncluds.hpp"


class FileRewrite{
private:
    GenerelInformation info;
    std::string str_to_insert;
    std::vector<std::string> function_variant;
    #ifdef WIN32
        const wchar_t *remove_file_failed;
        const wchar_t *file_patched;
        const wchar_t *say_goodby;
        const wchar_t *ask_user_a_path;
    #else
        const char *remove_file_failed ;
        const char *file_patched;
        const char *say_goodby;
        const char *ask_user_a_path;
    #endif // WIN32
public:
    FileRewrite(GenerelInformation _fileData);
    /** Получить от пользователя путь к новому месту хранения настроек*/
    void getSettingsPatch();
    /** Редактирование файла*/
    void rewrite();
   
    
};

#endif // !FILE_REWRITE