/**
 * @file main.cpp
 * @author Tolsedum (tolsedum@gmail.com)
 * @brief Entry point
 * @version 1.0
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */ 
#include "GetFileTarget.hpp"
#include "FileRewrite.hpp"



int main() {
    try{
        GetFileTarget getFileTarget;
        // Определить пользователя и перейти к нему в home дирикторию
        getFileTarget.jumpToDirectory();
        // Для Windows задает режим преобразования файлов и выводит логотип. Для Linux выыодит логотип.
        getFileTarget.setMode();
        
        // Найти нужный файл для правки и найти место правки
        GenerelInformation fileDate = getFileTarget.getFilePosition();
        FileRewrite fileRewrite(fileDate);

        // Получить от пользователя путь к новому месту хранения настроек и узнать какой плагин интересует пользователя
        fileRewrite.getSettingsPatch();

        // Внести изменения в файле
        fileRewrite.rewrite();
      
    }catch(const my_char* error){
        fn::printString(error);
    }catch (my_stryng error) {
        fn::printString(error);
    }catch(const fs::filesystem_error& e){
        fn::printString(e.what());
    }catch(std::exception & e) {
        fn::printString(e.what());
    }
    fn::pause();
	return 0;
}