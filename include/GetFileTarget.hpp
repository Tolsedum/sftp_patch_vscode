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
#pragma once
#ifndef GET_FILE_TARGET
#define GET_FILE_TARGET "GET_FILE_TARGET"

#include "GlobalIncluds.hpp"



class GetFileTarget{
private:
    /** @brief Имя файла который надо менять*/
    std::string name_file_target;
    /** @brief Имя временного файла для сохранения в него информации*/
    std::string tmp_file_name;
    /** @brief Список плагинов в которых изменить путь*/
    std::vector<std::string> use_plagin;
    /** @brief Список строк для определения позиции начала правки*/
    std::vector<std::string> search_begin;
    /** @brief Номер плагина из списка use_plagin. Соответствует итератору vecntor*/
    short selected_number;
    /** @brief Предложение пользователю о выборе плагина*/
    const my_char *ask_user_a_plagin;
    /** @brief Сообщение о том что надо ввести число*/
    const my_char *invalid_argument;
    /** @brief Сообщение о ограничении вводимых чисел*/
    const my_char *out_of_range;
    /** @brief Сообщение о том что файл не может быть открыт*/
    const my_char *file_is_not_open;
    /** @brief Сообщение о том что файл не найден*/
    const my_char *file_is_not_found;
    /** @brief Путь к каталогу с файлом*/
    const char *profile_patch;
    void getIntCin();
public:
    GetFileTarget();
    /** Определить пользователя и перейти к нему в home дирикторию*/
    void jumpToDirectory();
    /** Найти нужный файл для правки и найти место правки*/
    GenerelInformation getFilePosition();
    // Для Windows задает режим преобразования файлов и выводит логотип. Для Linux выыодит логотип.
    void setMode();
    
};
#endif // !GET_FILE_TARGET