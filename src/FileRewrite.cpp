#include "FileRewrite.hpp"

FileRewrite::FileRewrite(FileDate _fileData){
    fileDate = _fileData;
}

void FileRewrite::getSettingsPatch(){
    STR_TYPE tmp_path_settings_file;
    fn::printString(var::ask_user);
    // std::wcout<< L"Введите путь к месту хранения настроек: ";
    fn::getLineCin(tmp_path_settings_file);
    fn::printString("");
    // std::wcout<<std::endl;

    std::string path_settings;
    #ifdef WIN32
        path_settings = fn::wstringToString(tmp_path_settings_file);
    #else
        path_settings = tmp_path_settings_file;
    #endif // WIN32
    
    path_settings = std::regex_replace(path_settings, std::regex("\\\\"), "\\\\");
    str_to_insert = "function p(e){e='"+path_settings+"';return o.join(e,a.CONFIG_PATH)}";
}

void FileRewrite::rewrite(){
    if(fileDate.pos_begin > 0){
        std::ifstream file_target(fileDate.file_path, std::ios::in | std::ios::binary);
        std::ofstream tmp_file(fileDate.tmp_file_name, std::ios::out | std::ios::binary | std::ios::trunc);
        bool is_opened = tmp_file.is_open() && file_target.is_open();
        
        if(is_opened){
            char *big_buffer = new char[fileDate.pos_begin];
            file_target.read(big_buffer, fileDate.pos_begin);
            tmp_file.write(big_buffer, fileDate.pos_begin);
            delete[] big_buffer;

            tmp_file.write(str_to_insert.c_str(), str_to_insert.size());
            file_target.seekg(0, file_target.end);
            int size_buffer = file_target.tellg();
            size_buffer -= fileDate.pos_begin+(fileDate.pos_end-fileDate.pos_begin);
            file_target.seekg(fileDate.pos_end);

            char *buffer = new char[size_buffer];
            file_target.read(buffer, size_buffer);
            tmp_file.write(buffer, size_buffer);
            delete[] buffer;
            
        }
        file_target.close();
        tmp_file.close();
        if(is_opened){
            std::ifstream tmp_file_if(fileDate.tmp_file_name, std::ios::in | std::ios::binary);
            std::ofstream file_target_in(fileDate.file_path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
            file_target_in << tmp_file_if.rdbuf();
            tmp_file_if.close();
            file_target_in.close();

            if (remove(fileDate.tmp_file_name.c_str()) != 0) {
                throw var::remove_file_failed;
            }else{
                fn::printString(var::file_patched);
            }
        }
        fn::printString(var::say_goodby);
    }

}