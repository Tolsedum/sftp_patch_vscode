#include "GetFileTarget.hpp"

GetFileTarget::GetFileTarget(){
    tmp_file_name = "tmp_file.tmp";
    
    use_plagin = {
        "natizyskunk.sftp",
        "liximomo.sftp",
        "doujinya.sftp-revived"
    };
    search_begin = {
        "function d(e){return Object.assign({},h,e)}", // for natizyskunk.sftp
        "function p(e){return Object.assign({},h,e)}", // for liximomo.sftp
        "function d(e){return Object.assign({},h,e)}"  // for doujinya.sftp-revived
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
    

}

void GetFileTarget::getIntCin(){
    fn::printString(ask_user_a_plagin);
    STR_TYPE tmp_str;
    bool iter = true;
    while (iter){
        try{
            fn::getLineCin(tmp_str);
            selected_number = std::stoi(tmp_str);
            if(selected_number == 1){
                selected_number = 0;
                iter = false;
            }else if(selected_number == 2){
                selected_number = 1;
                iter = false;
            }else if(selected_number == 3){
                selected_number = 2;
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

void GetFileTarget::jumpToDirectory(){
    #ifdef WIN32
        system("chcp 65001");
        std::string cmd = "dir %UserProfile% >> "+tmp_file_name;
        system(cmd.c_str());
        
        std::ifstream tmp_file_point(tmp_file_name);
        if(tmp_file_point.is_open()){
            std::string search_str = " Directory of ";
            for(std::string line; getline(tmp_file_point, line);){
                std::size_t pos = line.find(search_str);
                if(pos != std::string::npos){
                    current_patch = line.replace(0, search_str.size(), "");
                    current_patch = std::regex_replace(current_patch, std::regex("\\\\"), "\\\\");
                    break;
                }
            }
            tmp_file_point.close();
            if(remove(tmp_file_name.c_str()) != 0){
                throw L"Remove tmp_file file failed";
            }
        }else{
            throw L"Tmp file is not excist";
        }
        _wchdir(fn::stringToWstring(current_patch).c_str());
    #else
        chdir(getenv("HOME"));
    #endif // WIN32
}

GenerelInformation GetFileTarget::getFilePosition(){
    getIntCin();
    std::ifstream file_point;
    GenerelInformation info;
    info.tmp_file_name = tmp_file_name;

    bool file_finde = false;
    for (auto const& p : fs::directory_iterator(profile_patch)){
        std::size_t pos = p.path().string().find(use_plagin[selected_number]);// "natizyskunk.sftp"
        if(pos != std::string::npos){
            name_file_target = p.path().string() + name_file_target;
            file_point.open(name_file_target);
            info.file_path = name_file_target;
            file_finde = true;
            break; 
        }
    }
    if(!file_finde){
        throw file_is_not_found;
    }else if (!file_point.is_open()){
        throw file_is_not_open;
    }

    int pos_len = 0;
    for (std::string line; getline(file_point, line);){
        std::size_t pos = line.find(search_begin[selected_number]);
        if(pos != std::string::npos){
            pos_len += pos + search_begin[selected_number].size();// +1 это - Сместить на f от }
            info.pos_begin = pos_len;
            std::string name_searching_str = "function";
            std::size_t pos_end = line.find(name_searching_str, pos + search_begin[selected_number].size()+name_searching_str.size());
            if(pos_end != std::string::npos){
                info.pos_end = pos_len + (pos_end-(pos + search_begin[selected_number].size()));
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
    info.selected_number = selected_number;
    return info;
}



void GetFileTarget::setMode(){
    #ifdef WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stdin),  _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
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