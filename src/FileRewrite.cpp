#include "FileRewrite.hpp"

FileRewrite::FileRewrite(GenerelInformation &_info){
    this->info = _info;
    #ifdef WIN32
        this->remove_file_failed = L"Удаление файла не удалось, удалите его сами.";
        this->file_patched = L"Файл изменён.";
        this->say_goodby = L"Файл изменён. Наслаждайтесь!";
        this->ask_user_a_path = L"Введите путь к месту хранения настроек: ";
        
    #else
        this->remove_file_failed = "Удаление файла не удалось, удалите его сами.";
        this->file_patched = "Файл изменён.";
        this->say_goodby = "Файл изменён. Наслаждайтесь!";
        this->ask_user_a_path = "Введите путь к месту хранения настроек: ";
        
    #endif // WIN32
    this->function_variant = {
        "function p(e){e='%s';return o.join(e,a.CONFIG_PATH)}",// for natizyskunk.sftp
        "function d(e){e='%s';return o.join(e,a.CONFIG_PATH)}", // for liximomo.sftp
        "function p(e){e='%s';return o.join(e,a.CONFIG_PATH)}" // for doujinya.sftp-revived
    };
}


void FileRewrite::getSettingsPatch(){
    my_stryng tmp_path_settings_file;

    fn::printString(this->ask_user_a_path);
    // Получение от пользователя пути хранения настроек
    fn::getLineCin(tmp_path_settings_file);
    fn::printString(' ');
    std::string path_settings;
    #ifdef WIN32
        path_settings = fn::wstringToString(tmp_path_settings_file);
        path_settings = std::regex_replace(path_settings, std::regex("\\\\"), "\\\\");
    #else
        path_settings = tmp_path_settings_file;
    #endif // WIN32
    
    this->str_to_insert = function_variant[this->info.selected_number];
    // Вставка пути в шаблон
    this->str_to_insert = std::regex_replace(this->str_to_insert, std::regex("%s"), path_settings);
}

void FileRewrite::rewrite(){
    if(this->info.pos_begin > 0){
        std::ifstream file_target(this->info.file_path, std::ios::in | std::ios::binary);
        std::ofstream tmp_file(this->info.tmp_file_name, std::ios::out | std::ios::binary | std::ios::trunc);
        bool is_opened = tmp_file.is_open() && file_target.is_open();
        
        if(is_opened){
            // Пмшем во временный файл данный из файла с кодом до позиции вставки
            char *big_buffer = new char[this->info.pos_begin];
            file_target.read(big_buffer, this->info.pos_begin);
            tmp_file.write(big_buffer, this->info.pos_begin);
            delete[] big_buffer;
            // Вставляем во временный файл новую запись
            tmp_file.write(this->str_to_insert.c_str(), this->str_to_insert.size());
            
            file_target.seekg(0, file_target.end);
            int size_buffer = file_target.tellg();
            // Определяем сколько надо пропустить
            size_buffer -= this->info.pos_begin+(this->info.pos_end-this->info.pos_begin);
            // Пропускаем старую запись
            file_target.seekg(this->info.pos_end);
            // Записываем всё что осталось
            char *buffer = new char[size_buffer];
            file_target.read(buffer, size_buffer);
            tmp_file.write(buffer, size_buffer);
            delete[] buffer;
        }
        file_target.close();
        tmp_file.close();
        if(is_opened){
            std::ifstream tmp_file_if(this->info.tmp_file_name, std::ios::in | std::ios::binary);
            std::ofstream file_target_in(this->info.file_path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
            // Переносим содержимое из временного файла в оригинальный
            file_target_in << tmp_file_if.rdbuf();
            tmp_file_if.close();
            file_target_in.close();
            // Удаляем временный файл
            if (remove(this->info.tmp_file_name.c_str()) != 0) {
                throw this->remove_file_failed;
            }else{
                fn::printString(this->file_patched);
            }
        }
        fn::printString(this->say_goodby);
    }
}