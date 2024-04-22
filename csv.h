#ifndef CSV_H
#define CSV_H

#include <string>

void create_file(const std::string& file_name, const std::string& key);
void create_user(const std::string& file_name);
std::string get_password(const std::string& file_name, const std::string& target_website,const std::string& key);
void edit_password(const std::string& file_name, const std::string& target_website, const std::string& new_password);
void display_file(const std::string& file_name, const std::string& key);
void delete_password(const std::string& file_name, const std::string& target_website);
bool verify_password(const std::string& file_name, std::string& username, std::string& password);
std::string encrypt(const std::string& data, const std::string& key);
#endif 