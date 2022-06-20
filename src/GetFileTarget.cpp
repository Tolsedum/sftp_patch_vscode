#include "GetFileTarget.hpp"



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
            throw "Tmp file is not excist";
        }
        _wchdir(fn::stringToWstring(current_patch).c_str());
    #else
        chdir("~/");
    #endif // WIN32
}

FileDate GetFileTarget::getFilePosition(){
    std::ifstream file_point;
    FileDate fileDate;
    fileDate.tmp_file_name = tmp_file_name;
    std::string dir_target = "\\\\dist\\\\extension.js";
    std::string profile_patch = ".vscode\\\\extensions";
    
    for (auto const& p : fs::directory_iterator(profile_patch)){
        std::size_t pos = p.path().string().find("natizyskunk.sftp");
        
        std::cout<< p.path().string()<< std::endl;

        if(pos != std::string::npos){
            dir_target = p.path().string() + dir_target;
            file_point.open(dir_target);
            fileDate.file_path = dir_target;
            break; 
        }
    }
    if (!file_point.is_open()){
            throw L"File not found";
    }
    std::string search_begin = "function d(e){return Object.assign({},h,e)}";
                            //    "function p(e){return Object.assign({},h,e)}"
    
    int pos_len = 0;
    for (std::string line; getline(file_point, line);){
        std::size_t pos = line.find(search_begin);
        if(pos != std::string::npos){
            pos_len += pos + search_begin.size()+1;// +1 это - Сместить на f от }
            fileDate.pos_begin = pos_len;
            std::string name_searching_str = "function";
            std::size_t pos_end = line.find(name_searching_str, pos + search_begin.size()+name_searching_str.size());
            if(pos_end != std::string::npos){
                fileDate.pos_end = pos_len + (pos_end-(pos + search_begin.size()));
                break;
            }
        }else{
            pos_len += line.size();
        }
    }
    file_point.close();
    return fileDate;
}

GetFileTarget::GetFileTarget(){
    tmp_file_name = "tmp_file.tmp";
    
}

void GetFileTarget::setMode(){
    #ifdef WIN32
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stdin),  _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
        auto const & sz_message
        {
            L" __________________________________________\n"
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
            " __________________________________________\n"
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