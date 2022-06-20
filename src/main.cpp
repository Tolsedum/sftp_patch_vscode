#include <iostream>
#include "GetFileTarget.hpp"
#include "FileRewrite.hpp"

/**
 * @file main.cpp
 * @author Tolsedum (tolsedum@gmail.com)
 * @brief Entry point
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */ 

int main(int argc, char *argv[]) {
    try{
        GetFileTarget getFileTarget;
        // Определить пользователя и перейти к нему в home дирикторию
        getFileTarget.jumpToDirectory();
        getFileTarget.setMode();
        // Найти нужный файл для правки и найти место правки
        FileDate fileDate = getFileTarget.getFilePosition();
        FileRewrite fileRewrite(fileDate);
        // Получить от пользователя путь к новому месту хранения настроек
        fileRewrite.getSettingsPatch();
        // Внести изменения в файле
        fileRewrite.rewrite();
      
    }catch(const wchar_t* error){
        // std::wcout << error << std::endl;
        fn::printString(error);
    }catch (std::wstring error) {
        // std::wcout << error << std::endl << std::endl;
        fn::printString(error);
    }catch (const std::exception & e) {
        // std::wcout << e.what() << std::endl;
        fn::printString(e);
    }
    system("PAUSE");
	return 0;
} 