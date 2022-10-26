#include "GetFileTarget.hpp"

GetFileTarget::GetFileTarget(){
    tmp_file_name = "tmp_file.tmp";
    
    use_plugin = {
        "natizyskunk.sftp",
        "liximomo.sftp",
        "doujinya.sftp-revived"
    };
    search_begin = {
        {
            "function d(e){return Object.assign({},h,e)}", 
            "function d(e){return Object.assign(Object.assign({},h),e)}"
        }, // for natizyskunk.sftp
        {
            "function p(e){return Object.assign({},h,e)}",
            "function d(e){return Object.assign(Object.assign({},h),e)}"
        }, // for liximomo.sftp
        {
            "function d(e){return Object.assign({},h,e)}",
            "function d(e){return Object.assign(Object.assign({},h),e)}"
        }// for doujinya.sftp-revived
    };
    
    #ifdef WIN32
        file_is_not_found = L"Файл не найден";
        file_is_not_open = L"Файл не может быть открыт";
        out_of_range = L"Число не должно быть больше 3 и меньше 1";
        invalid_argument = L"Введите число: ";
        ask_user_a_plagin = L"\
Каким плагином вы пользуетесь: \n \
1) natizyskunk.sftp \n \
2) liximomo.sftp \n \
3) doujinya.sftp-revived \n\
Введите номер: ";
        name_file_target = "\\\\dist\\\\extension.js";
        profile_patch = ".vscode\\\\extensions";
    #else
        file_is_not_found = "Файл не найден";
        file_is_not_open = "Файл не может быть открыт";
        out_of_range = "Число не должно быть больше 3 и меньше 1";
        invalid_argument = "Введите число: ";
        ask_user_a_plagin = "\
Каким плагином вы пользуетесь: \n \
1) natizyskunk.sftp \n \
2) liximomo.sftp \n \
3) doujinya.sftp-revived \n \
Введите номер: ";
        name_file_target = "/dist/extension.js";
        profile_patch = ".vscode/extensions";
    #endif // WIN32

    std::string name_file = "settings_file.conf";
    if(!fs::is_regular_file(name_file)){
        const my_char *ask_user_a_path;
        std::string path_settings;
        #ifdef WIN32
            ask_user_a_path = L"Введите путь к месту хранения настроек: ";
        #else
            ask_user_a_path = "Введите путь к месту хранения настроек: ";
        #endif // WIN32
        
        my_stryng tmp_path_settings_file;

        fn::printString(ask_user_a_path);
        // Получение от пользователя пути хранения настроек
        fn::getLineCin(tmp_path_settings_file);
        
        // std::getline(std::cin, path_settings);

        fn::printString(' ');
        #ifdef WIN32
            path_settings = fn::wstringToString(tmp_path_settings_file);
            // path_settings = fn::replaceSlash(fn::wstringToString(tmp_path_settings_file));
            path_settings = std::regex_replace(path_settings, std::regex("\\\\"), "\\\\");
        #else
            path_settings = tmp_path_settings_file;
        #endif // WIN32

        std::ofstream settings_file(name_file);
        settings_file<< "PATCH="+path_settings;
        settings_file.close();
    }else{
        std::ifstream settings_file(name_file);
        std::string anchor = "PATCH=";
        for(std::string line; getline(settings_file, line);){
            std::size_t pos = line.find(anchor)+anchor.size();
            if(std::string::npos != pos){
                for(std::size_t i = pos; i < line.size(); i++){
                    if(line[i] != ' '){
                        path_settings.append(1, line[i]);
                    }
                }
            }
        }
        settings_file.close();
    }
    
    // fn::printString(path_settings);
}



void GetFileTarget::jumpToDirectory(){
    #ifdef WIN32
        // Изменение кодировки в cmd на UTF-8
        system("chcp 65001");
        std::string current_patch;
        // Команда на сохранение списка файла и каталогов во временный файл tmp_file_name
        std::string cmd = "dir %UserProfile% >> "+tmp_file_name;
        // Выполнение заданной команды в cmd Windous
        system(cmd.c_str());
        // Получить из временного файла путь к пользовательскому каталогу
        std::ifstream tmp_file_point(tmp_file_name);
        if(tmp_file_point.is_open()){
            std::string search_str = " Directory of ";
            for(std::string line; getline(tmp_file_point, line);){
                std::size_t pos = line.find(search_str);
                if(pos != std::string::npos){
                    // Убрать то что находится перед нужной информацией
                    current_patch = line.replace(0, search_str.size(), "");
                    // Приводим в соответствие сам путь к директории. Должен выглядеть например вот так (C:\\Users\\user_himself)
                    current_patch = std::regex_replace(current_patch, std::regex("\\\\"), "\\\\");
                    break;
                }
            }
            tmp_file_point.close();
            if(remove(tmp_file_name.c_str()) != 0){
                throw L"Удалить временный файл не удалось, удалите его сами.";
            }
        }else{
            throw L"Временный файл открыть не удалось";
        }
        // Переход в домашнюю директорию пользователя
        _wchdir(fn::stringToWstring(current_patch).c_str());
    #else
        // Переход в домашнюю директорию пользователя
        chdir(getenv("HOME"));
    #endif // WIN32
}

void GetFileTarget::getIntCin(){
    fn::printString(ask_user_a_plagin);
    my_stryng tmp_str;
    bool iter = true;
    while (iter){
        try{
            fn::getLineCin(tmp_str);
            selected_number = std::stoi(tmp_str);
            if(selected_number > 0 && selected_number < 4){
                selected_number--;
                iter = false;
            }else{
                fn::printString(out_of_range);
            }
        }catch(const std::invalid_argument &e){
            fn::printString(invalid_argument);
        }catch(const std::out_of_range &e){ 
            fn::printString(out_of_range);
        }
    }
}


std::vector<GeneralInformation> GetFileTarget::getFilePosition(){
    // Получить номер который соответствует выбранному плагину
    getIntCin();
    std::vector<GeneralInformation> v_info;
    bool file_find = false;
    // Поиск подходящей директории плагина
    for (auto const& p : fs::directory_iterator(profile_patch)){
        std::size_t pos = p.path().string().find(use_plugin[selected_number]);
        if(pos != std::string::npos){
            std::ifstream file_point;
            GeneralInformation info;
            info.tmp_file_name = tmp_file_name;
            name_file_target = p.path().string() + name_file_target;
            name_file_target = fn::replaceSlash(name_file_target);
            file_point.open(name_file_target);
            
            if (!file_point.is_open()){
                throw file_is_not_open;
            }else{
                info.file_path = name_file_target;
                int pos_len = 0;
                bool found = false;

                std::string search_method = "";
                // Определение позиции в файле для его правки
                for (std::string line; getline(file_point, line);){
                    for(auto var : search_begin[selected_number]){
                        std::size_t pos = line.find(var);
                        if(pos != std::string::npos){
                            search_method = var;
                            found=true;
                            break;
                        }
                    }
                    if(found) break;
                }
                file_point.seekg(0, file_point.beg);
                // Определение позиции в файле для его правки
                for (std::string line; getline(file_point, line);){
                    std::size_t pos = line.find(search_method);
                    if(pos != std::string::npos){
                        pos_len += pos + search_method.size();
                        info.pos_begin = pos_len;
                        std::string name_searching_str = "function";
                        std::size_t pos_end = line.find(name_searching_str, pos + search_method.size()+name_searching_str.size());
                        if(pos_end != std::string::npos){
                            info.pos_end = pos_len + (pos_end-(pos + search_method.size()));
                            found = true;
                            break;
                        }
                    }else{
                        // Так как в Linux новая строка обозначается \n а в Windows \r\n при работе со строками в системе Win надо увеличить размер строки на 1 символ
                        pos_len += line.size();
                        #ifdef WIN32
                            pos_len +=1;
                        #endif // WIN32
                    }
                }
                info.selected_number = selected_number;
                
                v_info.push_back(info);
                file_find = true;
                file_point.close();
            }
            
        }
    }

    if(!file_find){
        throw file_is_not_found;
    }

    
    
    
    return v_info;
}



void GetFileTarget::setMode(){
    #ifdef WIN32
        //  Задает режим преобразования файлов
        if(_setmode(_fileno(stdout), _O_U16TEXT) < 0)throw L"Не удалось установить режим преобразования файла!";
        auto const & sz_message
        {
            L" __________________________________________ \n"
            L"|                                          |\n"
            L"|   ╭━━━━┳━━━┳╮╱╱╭━━━┳━━━┳━━━┳╮╱╭┳━╮╭━╮    |\n"
            L"|   ┃╭╮╭╮┃╭━╮┃┃╱╱┃╭━╮┃╭━━┻╮╭╮┃┃╱┃┃┃╰╯┃┃    |\n"
            L"|   ╰╯┃┃╰┫┃╱┃┃┃╱╱┃╰━━┫╰━━╮┃┃┃┃┃╱┃┃╭╮╭╮┃    |\n"
            L"|   ╱╱┃┃╱┃┃╱┃┃┃╱╭╋━━╮┃╭━━╯┃┃┃┃┃╱┃┃┃┃┃┃┃    |\n"
            L"|   ╱╱┃┃╱┃╰━╯┃╰━╯┃╰━╯┃╰━━┳╯╰╯┃╰━╯┃┃┃┃┃┃    |\n"
            L"|   ╱╱╰╯╱╰━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻╯╰╯╰╯    |\n"
            L"|__________________________________________|\n"
            L"|                                          |\n"
            L"| Permission is hereby granted, free of    |\n"
            L"| charge, to any person obtaining a copy of|\n"
            L"| of this software and accompanying files, |\n"
            L"| to use them without restriction,         |\n"
            L"| including, without limitation, the       |\n"
            L"| rights to use, copy, modify, merge,      |\n"
            L"| publish, distribute, sublicense and/or   |\n"
            L"| sell copies of the software. The authors |\n"
            L"| or copyright holders shall not be liable |\n"
            L"| for any claims, damages or other         |\n"
            L"| liability, whether in contract, tort or  |\n"
            L"| otherwise, arising out of or in          |\n"
            L"| connection with the software or your use |\n"
            L"| or other dealings with the software.     |\n"
            L"|__________________________________________|\n"
            L"|   website: ***************               |\n"
            L"|   email: tolsedum@gmail.com              |\n"
            L"|   @version 1.1                           |\n"
            L"|__________________________________________|\n"
        };
        std::wcout << sz_message << std::endl;
    #else
        auto const &sz_message{
            " __________________________________________ \n"
            "|                                          |\n"
            "|   ╭━━━━┳━━━┳╮╱╱╭━━━┳━━━┳━━━┳╮╱╭┳━╮╭━╮    |\n"
            "|   ┃╭╮╭╮┃╭━╮┃┃╱╱┃╭━╮┃╭━━┻╮╭╮┃┃╱┃┃┃╰╯┃┃    |\n"
            "|   ╰╯┃┃╰┫┃╱┃┃┃╱╱┃╰━━┫╰━━╮┃┃┃┃┃╱┃┃╭╮╭╮┃    |\n"
            "|   ╱╱┃┃╱┃┃╱┃┃┃╱╭╋━━╮┃╭━━╯┃┃┃┃┃╱┃┃┃┃┃┃┃    |\n"
            "|   ╱╱┃┃╱┃╰━╯┃╰━╯┃╰━╯┃╰━━┳╯╰╯┃╰━╯┃┃┃┃┃┃    |\n"
            "|   ╱╱╰╯╱╰━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻╯╰╯╰╯    |\n"
            "|__________________________________________|\n"
            "|                                          |\n"
            "| Permission is hereby granted, free of    |\n"
            "| charge, to any person obtaining a copy of|\n"
            "| of this software and accompanying files, |\n"
            "| to use them without restriction,         |\n"
            "| including, without limitation, the       |\n"
            "| rights to use, copy, modify, merge,      |\n"
            "| publish, distribute, sublicense and/or   |\n"
            "| sell copies of the software. The authors |\n"
            "| or copyright holders shall not be liable |\n"
            "| for any claims, damages or other         |\n"
            "| liability, whether in contract, tort or  |\n"
            "| otherwise, arising out of or in          |\n"
            "| connection with the software or your use |\n"
            "| or other dealings with the software.     |\n"
            "|__________________________________________|\n"
            "|   website: ***************               |\n"
            "|   email: tolsedum@gmail.com              |\n"
            "|   @version 1.1                           |\n"
            "|__________________________________________|\n"
        };
        std::cout << sz_message << std::endl;
    #endif // WIN32
}

std::string GetFileTarget::getPathSettings(){
    return path_settings;
}