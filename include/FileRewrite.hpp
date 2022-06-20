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
    FileDate fileDate;
    std::string str_to_insert;
public:
    FileRewrite(FileDate _fileData);
    /** Получить от пользователя путь к новому месту хранения настроек*/
    void getSettingsPatch();
    /** Редактирование файла*/
    void rewrite();
};

#endif // !FILE_REWRITE