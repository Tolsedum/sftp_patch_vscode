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
    /** @brief Путь к настройкам sftp*/
    std::string path_settings;
    /** @brief Имя файла с настройками программы*/
    std::string name_file;
    /** @brief Список плагинов в которых изменить путь*/
    std::vector<std::string> use_plugin;
    /** @brief Список строк для определения позиции начала правки*/
    std::vector<std::vector<std::string>> search_begin;
    /** @brief Номер плагина из списка use_plugin. Соответствует итератору vector*/
    short selected_number;
    /** @brief Предложение пользователю о выборе плагина*/
    const my_char *ask_user_a_plagin;
    /** @brief Сообщение о том что надо ввести число*/
    const my_char *invalid_argument;
    /** @brief Сообщение о ограничении вводимых чисел*/
    const my_char *out_of_range;
    /** @brief Сообщение о том что файл не может быть открыт*/
    const my_char *file_is_not_open;
    /** @brief Сообщение о том что файл не может быть открыт*/
    const my_char *file_is_kent_be_open;
    /** @brief Сообщение о том что файл не найден*/
    const my_char *file_is_not_found;
    /** @brief Сообщение о том что Имя пользователя не найдено*/
    const my_char *user_name_is_not_find;
    /** @brief Путь к каталогу с файлом*/
    const char *profile_patch;
    /** @brief Путь к домашней папки*/
    std::string current_patch;
    /** @brief Файл с настройками*/
    std::ofstream out_settings_file;
    std::ifstream settings_file;


    
    void getIntCin();
public:
    GetFileTarget();
    ~GetFileTarget(){};
    /** Определить пользователя и перейти к нему в home дирикторию*/
    void jumpToDirectory();
    /** Найти нужный файл для правки и найти место правки*/
    std::vector<GeneralInformation> getFilePosition();
    // Для Windows задает режим преобразования файлов и выводит логотип. Для Linux выыодит логотип.
    void setMode();
    /** @brief Вернуть путь к настройкам*/
    std::string getPathSettings();
    /** @brief Получить сохранённый каталог из файла или если его там нет то запросить у пользователя */
    void setPath();
};
#endif // !GET_FILE_TARGET