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
#pragma once
#ifndef FILE_REWRITE
#define FILE_REWRITE "FILE_REWRITE"


#include "GlobalIncluds.hpp"


class FileRewrite{
private:
    /** @brief Информация о файле*/
    GenerelInformation info;
    /** @brief Строка с готовым js кодом*/
    std::string str_to_insert;
    /** @brief Варианты заготовок с js кодом для разных плагинов*/
    std::vector<std::string> function_variant;
    /** @brief Сообщение о неудачном удалении*/
    const my_char *remove_file_failed;
    /** @brief Сообщение о удачном изменении файла*/
    const my_char *file_patched;
    /** @brief Прощание с пользователем*/
    const my_char *say_goodby;
    /** @brief Спросить пользователя путь где хранить файл*/
    const my_char *ask_user_a_path;
public:
    FileRewrite(GenerelInformation& info);
    /** Получить от пользователя путь к новому месту хранения настроек*/
    void getSettingsPatch();
    /** Редактирование файла*/
    void rewrite();
   
    
};

#endif // !FILE_REWRITE