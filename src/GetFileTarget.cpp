#include "GetFileTarget.hpp"

GetFileTarget::GetFileTarget(){
    this->tmp_file_name = "tmp_file.tmp";
    
    this->use_plagin = {
        "natizyskunk.sftp",
        "liximomo.sftp",
        "doujinya.sftp-revived"
    };
    this->search_begin = {
        "function d(e){return Object.assign({},h,e)}", // for natizyskunk.sftp
        "function p(e){return Object.assign({},h,e)}", // for liximomo.sftp
        "function d(e){return Object.assign({},h,e)}"  // for doujinya.sftp-revived
    };
    
    #ifdef WIN32
        this->file_is_not_found = L"Файл не найден";
        this->file_is_not_open = L"Файл не может быть открыт";
        this->out_of_range = L"Число не должно быть больше 3 и меньше 1";
        this->invalid_argument = L"Введите число: ";
        this->ask_user_a_plagin = L"\
Каким плагином вы пользуетесь: \n \
1) natizyskunk.sftp \n \
2) liximomo.sftp \n \
3) doujinya.sftp-revived \n\
Введите номер: ";
        this->name_file_target = "\\\\dist\\\\extension.js";
        this->profile_patch = ".vscode\\\\extensions";
    #else
        this->file_is_not_found = "Файл не найден";
        this->file_is_not_open = "Файл не может быть открыт";
        this->out_of_range = "Число не должно быть больше 3 и меньше 1";
        this->invalid_argument = "Введите число: ";
        this->ask_user_a_plagin = "\
Каким плагином вы пользуетесь: \n \
1) natizyskunk.sftp \n \
2) liximomo.sftp \n \
3) doujinya.sftp-revived \n \
Введите номер: ";
        this->name_file_target = "/dist/extension.js";
        this->profile_patch = ".vscode/extensions";
    #endif // WIN32
}

void GetFileTarget::jumpToDirectory(){
    #ifdef WIN32
        // Изменение кодировки в cmd на UTF-8
        system("chcp 65001");
        std::string current_patch;
        // Команда на сохранение списка файла и каталогов во временный файл tmp_file_name
        std::string cmd = "dir %UserProfile% >> "+this->tmp_file_name;
        // Выполнение заданной команды в cmd Windous
        system(cmd.c_str());
        // Получить из временного файла путь к пользовательскому каталогу
        std::ifstream tmp_file_point(this->tmp_file_name);
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
            if(remove(this->tmp_file_name.c_str()) != 0){
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
    fn::printString(this->ask_user_a_plagin);
    my_stryng tmp_str;
    bool iter = true;
    while (iter){
        try{
            fn::getLineCin(tmp_str);
            this->selected_number = std::stoi(tmp_str);
            if(this->selected_number == 1){
                this->selected_number = 0;
                iter = false;
            }else if(this->selected_number == 2){
                this->selected_number = 1;
                iter = false;
            }else if(this->selected_number == 3){
                this->selected_number = 2;
                iter = false;
            }else{
                fn::printString(this->out_of_range);
            }
        }catch(const std::invalid_argument &e){
            fn::printString(this->invalid_argument);
        }catch(const std::out_of_range &e){ 
            fn::printString(this->out_of_range);
        }
    }
}


GenerelInformation GetFileTarget::getFilePosition(){
    // Получить номер который соответствует выбранному плагину
    this->getIntCin();
    std::ifstream file_point;
    GenerelInformation info;
    info.tmp_file_name = tmp_file_name;

    bool file_finde = false;
    // Поиск подходящей директории плагина
    for (auto const& p : fs::directory_iterator(this->profile_patch)){
        std::size_t pos = p.path().string().find(this->use_plagin[this->selected_number]);
        if(pos != std::string::npos){
            this->name_file_target = p.path().string() + this->name_file_target;
            file_point.open(this->name_file_target);
            info.file_path = this->name_file_target;
            file_finde = true;
            break; 
        }
    }
    if(!file_finde){
        throw this->file_is_not_found;
    }else if (!file_point.is_open()){
        throw this->file_is_not_open;
    }

    int pos_len = 0;
    // Определение позиции в файле для его правки
    for (std::string line; getline(file_point, line);){
        std::size_t pos = line.find(this->search_begin[this->selected_number]);
        if(pos != std::string::npos){
            pos_len += pos + this->search_begin[this->selected_number].size();
            info.pos_begin = pos_len;
            std::string name_searching_str = "function";
            std::size_t pos_end = line.find(name_searching_str, pos + this->search_begin[this->selected_number].size()+name_searching_str.size());
            if(pos_end != std::string::npos){
                info.pos_end = pos_len + (pos_end-(pos + this->search_begin[this->selected_number].size()));
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
    file_point.close();
    info.selected_number = this->selected_number;
    return info;
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
            "|__________________________________________|\n"
        };
        std::cout << sz_message << std::endl;
    #endif // WIN32
}