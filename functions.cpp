#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <stdio.h>
#include <filesystem>

#include "header.h"



Line::Line(const std::string &name, const std::string &password, const std::string &category,
           const std::string &websiteService, const std::string &login) : name(name), password(password),
                                                                          category(category),
                                                                          website_service(websiteService),
                                                                          login(login) {}
std::ostream& operator<<(std::ostream &o, Line const& line){
    std::string str = line.name + " " + line.password + " " + line.category + " "
            + line.website_service + " " + line.login + "\n";

    return o << str;
}


auto Line::toString() -> const std::string  {
    std::string str = name + " " + password + " " + category;
    if(!website_service.empty()  && !login.empty()){
        str += " " + website_service + " " + login;
    }

    str += "\n";

    return str;
}

auto validate_string(std::string& str) -> void{
    auto iter_exp = std::find(str.begin(), str.end(), '!');
    auto iter = std::find(str.begin(), str.end(), ' ');
    if(iter != str.end()) {
        std::cout << "Changing all spaces with \"_\"\n";
        std::replace(str.begin(), str.end(), ' ', '_');
    }

    if(iter_exp != str.end()){
        std::cout << "Changing all ! to #\n";
        std::replace(str.begin(), str.end(), '!', '#');
    }

}

auto load_files()-> std::vector<std::string>{
    auto files = std::vector<std::string>();
    auto dir = std::string("..\\..\\password_manager\\files");
    int iter = 1;
    for(auto const & file : std::filesystem::directory_iterator(dir)){
        auto f = file.path().string();
        auto extension = std::string(f.end()-4, f.end());
        if(/*extension == ".txt"*/ f != "..\\..\\password_manager\\CMakeLists.txt") {
            files.push_back(f);
            std::cout << iter++ <<". "<< file << "\n";
        }
    }
    std::cout << iter << ". Create new file\n";

    return files;
}

auto choose_file(std::vector<std::string> const& files) -> std::string{
    std::cout << "Choose one of the file (by index)\n";
    int option = 0;
    auto filePath = std::string();


    while(true) {
        std::cin >> option;
        std::cin.ignore(); //after this I can use getline
        if (option > files.size()) {
            std::cout << "Input name of the new file\n";
//            std::cin >> filePath;
            std::getline(std::cin, filePath);
            validate_string(filePath);
            filePath = "..\\..\\password_manager\\files\\" + filePath;
            break;
        } else if (option <= files.size()) {
            filePath = files[option-1];
            break;
        } else {
            std::cout << "Wrong input try again\n";
        }
    }

    return filePath;

}

auto set_all_lines(std::vector<Line> &all_lines, std::set<std::string> &categories, std::string const& filePath, std::string const& password)-> void{
    auto reader = std::fstream(filePath, std::ios::in);
    auto line = std::string();

    int index = 0;
    int num_of_timestamps = 0;
    int line_index = 0;
    while(std::getline(reader, line)){
        if((line_index%2 == 0 && num_of_timestamps < 3) || line.size() == 2){
            num_of_timestamps++;
        }else {
            decrypt_password(line, password);
            auto lineStream = std::stringstream(line);
            auto name = std::string();
            auto password = std::string();
            auto category = std::string();
            auto website_service = std::string();
            auto login = std::string();

            lineStream >> name;
            lineStream >> password;
            lineStream >> category;
            lineStream >> website_service;
            lineStream >> login;

            categories.insert(category);
            auto l = Line(name, password, category, website_service, login);

            all_lines.push_back(l);
        }
        line_index++;
    }
    reader.close();
    updateTimeStamp(filePath, password, all_lines);

}

auto encrypt_password(std::string& string_to_encrypt, std::string const &password ) -> std::string{

    int pass_index = 0;
    for(int i = 0; i < string_to_encrypt.size(); i++){

        int len = password[pass_index++]%125;
        if(pass_index >= password.size())
            pass_index=0;

        if(string_to_encrypt[i] == '\n')
            break;


        if(string_to_encrypt[i] == ' '){
            string_to_encrypt[i]++;
        }else {

            for (int j = 0; j < len; j++) {
//            if(password_to_encrypt[i]%32 + 1 > 26)
//            {
//                password_to_encrypt[i] -= 25;
//            }else
//                password_to_encrypt[i] += 1;
                if (string_to_encrypt[i] + 1 > 126)
                    string_to_encrypt[i] = 34;
                else
                    string_to_encrypt[i]++;
                if (string_to_encrypt[i] == '!')
                    string_to_encrypt[i]++;

            }
        }
    }

    return string_to_encrypt;
}

auto decrypt_password(std::string& string_to_decrypt, std::string const& password) -> void{
    int pass_index = 0;
    for(int i = 0; i < string_to_decrypt.size(); i++){

        int len = password[pass_index++]%124;
        if(pass_index >= password.size())
            pass_index=0;

        if(string_to_decrypt[i] == '\n')
            break;

        if(string_to_decrypt[i] == '!'){
            string_to_decrypt[i] = ' ';
        }else {

            for (int j = 0; j < len; j++) {
//            if(password_to_encrypt[i]%32 + 1 > 26)
//            {
//                password_to_encrypt[i] -= 25;
//            }else
//                password_to_encrypt[i] += 1;
                if (string_to_decrypt[i] - 1 < 33)
                    string_to_decrypt[i] = 126;
                else
                    string_to_decrypt[i]--;
                if (string_to_decrypt[i] == '!')
                    string_to_decrypt[i] = 126;

            }
        }
    }

}

auto updateTimeStamp(std::string const& file_path, std::string const& password, std::vector<Line> const& vector) -> void{
    time_t now = time(0);
    char * date_time = ctime(&now);
    std::stringstream s(date_time);
    auto str = std::string();
    for(int i = 0; i < 4; i++){
        s >> str;
    }

    std::string time[3];
    int index = 0;
    std::stringstream ss (str);

    while(std::getline(ss, time[index++], ':')){
        //setting hours minutes and seconds to array
    }

    write_passwords_to_file(file_path, password, time , vector);
}

auto write_passwords_to_file(std::string const& file_path, std::string const& password, std::string* time_stamp, std::vector<Line> const& vector)->void {
    auto write = std::fstream(file_path, std::ios::out);

    int line_num = 0;
    int index_for_timestamp = 0;


    for(Line line : vector){
        if(line_num%2 == 0 && index_for_timestamp < 3){
            write << time_stamp[index_for_timestamp++] + "\n";
        }
        auto l = line.toString();
        encrypt_password(l, password);
        write << l;

        line_num+=2;
    }

    if(index_for_timestamp < 3)
    {
        for(int i = index_for_timestamp; i < 3; i++){
            write << time_stamp[i] + "\n";
        }
    }

    write.close();

}

auto check_category(std::set<std::string> &categories , std::string const& str)-> void {
    if(!categories.contains(str)){
        std::cout << "Element " << str << " added to the categories\n";
        categories.insert(str);
    }
}

auto add_category(std::set<std::string> & categories) -> void{
    auto str = std::string();
    std::cout << "Input new category\n";
//    std::cin >> str;
    std::getline(std::cin, str);
    validate_string(str);
    if(categories.contains(str)){
        std::cout << "Category already listed in the set\n";
    }else
    {
        categories.insert(str);
        std::cout << "Category successfully saved\n";
    }
}

auto add_password(std::set<std::string> & categories,std::string const& password, std::string const& file_path, std::vector<Line> &vec)-> void {
    std::cout << "Choose adding password\n"
                 "1. Automatically generate password\n"
                 "2. Type password\n";
    auto choose = std::string();
    auto option = std::string();
//    char c;
//    std::cin >> choose;
    std::getline(std::cin, choose);
    choose = choose[0];
    auto result = std::string();
    auto name = std::string();
    auto category = std::string();
    auto website_service = std::string();
    auto login = std::string();
    auto password_to_save = std::string();
    if(choose == "1" || choose == "2") {
        std::cout << "Input name:\n";
//    std::cin >> name;
        std::getline(std::cin, name);
        validate_string(name);
        std::cout << "Input the category by name you want (the category not listed will be automatically added)\n";
        auto str = std::string();
        int it = 1;
        for (auto data: categories) {
            std::cout << it++ << ". " << data << '\n';
        }
//    std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        check_category(categories, str);
        category = str;
        std::cout << "Do you want to input website and login (yes/no) ?\n";

        std::getline(std::cin, option);
        if (option == "yes") {
            std::cout << "Website/Service:\n";
//        std::cin >> website_service;
            std::getline(std::cin, website_service);
            validate_string(website_service);
            std::cout << "Login:\n";
//        std::cin >> login;
            std::getline(std::cin, login);
            validate_string(login);
        }
    }
    if (choose == "1") {
        password_to_save = generate_random_password();
    } else if (choose == "2"){
        std::cout << "Input password to save:\n";
//        std::cin >> password_to_save;
        std::getline(std::cin, password_to_save);
        validate_string(password_to_save);
    }else{
        std::cout <<"Wrong input going back to menu\n";
        return;
    }

    auto l = Line(name, password_to_save, category, website_service, login);
    vec.push_back(l);
    result += name + " " + password_to_save + " " + category;

    if (option == "yes") {
        result += " " + website_service + " " + login;
    }
    updateTimeStamp(file_path, password, vec);

    std::cout << "Password successfully added\n";
}

auto generate_random_password()-> std::string {
    int signs[] = { '@', '#', '$', '%', '^', '&', '*', '(', ')','-','+','=',
                   '~','`','[',']','\\','|',';',':','\'','\"',',', '<', '.',
                   '>', '/', '?'};
    srand(time(nullptr));
    auto result = std::string();
    int num_of_characters = 0;
    std::cout << "Input number of characters in password: \n" ;
    std::cin >> num_of_characters;

    auto s_char = std::string();
    bool include_s = false;
    std::cout << "Do you want to include special characters (yes/no)\n";
    std::cin >> s_char;

    if(s_char == "yes") {
        include_s = true;
    }
    else if(s_char != "yes" && s_char != "no")
        s_char = "no";

    auto option = std::string();
    std::cout << "Include upper and lower case (yes/no) ?\n";
    std::cin >> option;

    if(option == "no"){
        std::cout << "Include lower case or upper case (lower/upper) ?\n";
        std::cin >> option;

        if(option == "lower"){
            for(int i = 0; i < num_of_characters; i++){
                if(i%5 == 0 && include_s){
                    result += '0' + rand()%9;
                }else if(i%3 == 0 && include_s) {
                    result += signs[rand() % 27];
                }else {
                    int ran = 'a' + rand() % 26;
                    result += (char) ran;
                }
            }
        }else
        {
            for(int i = 0; i < num_of_characters; i++){
                if(i%5 == 0 && include_s){
                    result += '0' + rand()%9;
                }else if(i%3 == 0 && include_s) {
                    result += signs[rand() % 27];
                }else {
                    int ran = 'A' + rand() % 26;
                    result += (char) ran;
                }
            }
        }
    }else
    {
        for (int i = 0; i < num_of_characters; i++) {

            if(i%5 == 0 && include_s){
                result += '0' + rand()%9;
            }else if(i%3 == 0 && include_s) {
                result += signs[rand() % 27];
            }else {
                int random_1 = rand()%2;
                if (random_1 == 0) {
                    result += 'a' + (char) rand() % 26;
                } else
                    result += 'A' + (char) rand() % 26;
            }
        }
    }
    return result;
}

auto display_all_passwords(std::vector<Line> const& vec) -> void{

    if(vec.size() == 0)
    {
        std::cout << "The list is empty add passwords to the container\n";
    }else {
        for (Line line: vec) {
            std::cout << line;
        }
    }

}

auto approximate_password(std::string const& input, std::string & password)-> bool {

    auto part_str = std::string(password.begin(), password.begin()+input.size());
    return input == part_str;
}

auto search_passwords(std::vector<Line> const &vec)-> void {
    std::cout << "Choose searching option (number)\n"
                 "1. By name\n"
                 "2. By password\n"
                 "3. By category\n";
    auto option = std::string();
    auto str = std::string();
    std::cin >> option;
    std::cin.ignore();
    option = option[0];
    bool found = false;
    if(option == "1"){
        //by name
        std::cout << "Input name\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(Line line : vec){
            if(line.name == str){
                //fix displaying object
                std::cout << line;
                found = true;
            }
        }
    }else if(option == "3"){
        std::cout << "Input category\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(Line line : vec){
            if(line.category == str)
            {
                std::cout << line;
                found = true;
            }
        }

    }else if(option == "2"){
        std::cout << "Which search do you want ot use:\n"
                     "1. Exact search\n"
                     "2. Approximate search\n";
        std::cin >> option;
        std::cin.ignore();
        option = option[0];
//        std::cout << "Input password\n";
//        std::getline(std::cin, str);
//        validate_string(str);
        if(option == "1"){
            std::cout << "Input password\n";
            std::getline(std::cin, str);
            validate_string(str);
            for(Line line : vec) {
                if(line.password == str) {
                    std::cout << line;
                    found = true;
                }
            }
        }else if(option == "2")
        {
            std::cout << "Input approximate password\n";
            std::getline(std::cin, str);
            validate_string(str);
            for(Line line : vec){
                if(approximate_password(str, line.password)){
                    std::cout << line;
                    found = true;
                }
            }
        }else{
            std::cout << "Wrong input going back to menu\n";
            return;
        }
    }else{
        std::cout << "Wrong input going back to menu\n";
        return;
    }
    if(!found){
        std::cout << "Line not found :((\n";
    }
}

auto sort_passwords(std::vector<Line> & vector)-> void {

    std::cout << "Choose sorting:\n"
                 "1. By name and password\n"
                 "2. By password and name\n"
                 "3. By category and password\n"
                 "4. By category and name\n";

    auto num = std::string();
    auto option = std::string();
    std::cin >> num;
//    bool sorted = true;
    if(num == "1")
    {
        std::cout << "Sort asc/desc ?\n";
        std::cin >> option;
        std::ranges::sort(vector, [option](Line const& line1, Line const& line2)-> bool {
            if(option == "asc") {
                if (line1.name == line2.name) {
                    return line1.password < line2.password;
                }else
                    return line1.name < line2.name;
            }else{
                if (line1.name == line2.name) {
                    return line1.password > line2.password;
                } else
                    return line1.name > line2.name;
            }
        });
    }else if(num == "2")
    {
        std::cout << "Sort asc/desc ?\n";
        std::cin >> option;
        std::ranges::sort(vector, [option](Line const& line1, Line const& line2)-> bool {
            if(option == "asc") {
                if (line1.password == line2.password) {
                    return line1.name < line2.name;
                } else
                    return line1.password < line2.password;
            }else{
                if (line1.password == line2.password) {
                    return line1.name > line2.name;
                } else
                    return line1.password > line2.password;
            }
        });
    }else if(num == "3") {
        std::cout << "Sort asc/desc ?\n";
        std::cin >> option;
        std::ranges::sort(vector, [option](Line const &line1, Line const &line2) -> bool {
            if (option == "asc") {
                if (line1.category == line2.category) {
                    return line1.name < line2.name;
                } else
                    return line1.category < line2.category;
            } else {
                if (line1.category == line2.category) {
                    return line1.name > line2.name;
                } else
                    return line1.category > line2.category;
            }
        });
    }else if(num == "4"){
        std::cout << "Sort asc/desc ?\n";
        std::cin >> option;
        std::ranges::sort(vector, [option](Line const &line1, Line const &line2) -> bool {
            if (option == "asc") {
                if (line1.category == line2.category) {
                    return line1.password < line2.password;
                } else
                    return line1.category < line2.category;
            } else {
                if (line1.category == line2.category) {
                    return line1.password > line2.password;
                } else
                    return line1.category > line2.category;
            }
        });
    }else {
        std::cout << "Wrong input going back to menu\n";
        return;
//        sorted = false;
    }
//    if(sorted)
        std::cout << "\nPasswords successfully sorted\n";
}

auto edit_password(std::vector<Line> & vector, std::string const& file_path, std::string const& password) -> void{
    std::cout << "Password editing, choose how to search password\n"
                 "1. By name\n"
                 "2. By password\n"
                 "3. By website/service\n"
                 "4. By login\n";
    auto option = std::string();
    auto str = std::string();
    auto changed = false;
    Line* line_to_change;
    std::cin >> option;
    std::cin.ignore();
    option = option[0];

    if(option == "1"){
        //searching by name
        std::cout << "Input name:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(Line &line : vector){
            if(line.name == str){
                line_to_change = &line;
                changed = true;
                break;
            }
        }
    }else if(option == "2"){
        //searching by password
        std::cout << "Input password:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(Line &line : vector){
            if(line.password == str){
                line_to_change = &line;
                changed = true;
                break;
            }
        }
    }
    else if(option == "3"){
        //searching by website/service
        std::cout << "Input website/service:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(Line &line : vector){
            if(line.website_service == str){
                line_to_change = &line;
                changed = true;
                break;
            }
        }
    }else if(option == "4"){
        //searching by login
        std::cout << "Input login:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(Line &line : vector){
            if(line.login == str){
                line_to_change = &line;
                changed = true;
                break;
            }
        }
    }else
    {
        std::cout << "Wrong input going back to menu\n";
        return;
    }
    if(!changed)
    {
        std::cout << "Couldn't find the object\n";
        return;
    }

    std::cout << "Choose option of creating password:\n"
                 "1. Input your new password\n"
                 "2. Generate new random password\n";
    std::cin >> option;
    std::cin.ignore();
    option = option[0];

    if(option == "1"){
        std::cout << "Input new password:\n";
        std::getline(std::cin, str);
        validate_string(str);
    }else if(option == "2")
    {
        str = generate_random_password();
    }else{
        std::cout << "Wrong input going back to menu\n";
        return;
    }


//    std::cin >> str;

    line_to_change->password = str;
//       std::cout << str << " " << line_to_change->password << "\n";
//        std::cout << *line_to_change << '\n';

    std::cout << "Changes positively saved\n";

    auto writer = std::fstream(file_path, std::ios::out);

    updateTimeStamp(file_path, password, vector);
}

auto delete_password(std::vector<Line> & vector, std::string const& file_path, std::string const& password)-> void {


    std::cout << "Password deleting, choose how to search password\n"
                 "1. By name\n"
                 "2. By password\n"
                 "3. By website/service\n"
                 "4. By login\n";
    auto option = std::string();
    auto str = std::string();
    auto erased = false;
    std::cin >> option;
    std::cin.ignore();

    option = option[0];
    if(option == "1"){
        //searching by name
        std::cout << "Input name:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(int i = 0; i < vector.size();i++){
            if(vector[i].name == str){
                erased = true;
                vector.erase(vector.begin()+i);
            }
        }
    }else if(option == "2"){
        //searching by password
        std::cout << "Input password:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(int i = 0; i < vector.size();i++){
            if(vector[i].password == str){
                erased = true;
                vector.erase(vector.begin()+i);
            }
        }
    }
    else if(option == "3"){
        //searching by website/service
        std::cout << "Input website/service:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(int i = 0; i < vector.size();i++){
            if(vector[i].website_service == str){
                erased = true;
                vector.erase(vector.begin()+i);
            }
        }
    }else if(option == "4"){
        //searching by login
        std::cout << "Input login:\n";
//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);
        for(int i = 0; i < vector.size();i++){
            if(vector[i].login == str){
                erased = true;
                vector.erase(vector.begin()+i);
            }
        }
    }else {
        std::cout << "Wrong input coming back to menu\n";
        return;
    }
    if(erased) {
        std::cout << "Element successfully deleted\n";
        auto writer = std::fstream (file_path, std::ios::out);
        updateTimeStamp(file_path, password, vector);

    }
    else
        std::cout << "Couldn't find element to erased\n";
}

auto delete_category(std::set<std::string> &categories, std::vector<Line> & vector, std::string const& file_path, std::string const& password)-> void{
    auto option = std::string();
    std::cout << "Warning!! All lines with this category will be also deleted."
                 "\nDo you want to continue (yes/no)?\n";
    std::cin >> option;
    std::cin.ignore();
    if(option == "yes") {
        fmt::print("{}", categories);
        std::cout << "\nInput category to delete\n";
        int counter = 0;
        auto str = std::string();
        bool deleted = false;

//        std::cin >> str;
        std::getline(std::cin, str);
        validate_string(str);

        auto iter = std::ranges::find(categories.begin(), categories.end(), str);
        if (iter != categories.end()) {
            categories.erase(iter);
            deleted = true;
        }

        if (deleted) {
            for (int i = 0; i < vector.size(); i++) {
                if (vector[i].category == str) {
                    counter++;
                    if(i == vector.size()-1)
                        vector.erase(vector.end());
                    else
                        vector.erase(vector.begin() + i);
                }
            }

            updateTimeStamp(file_path, password, vector);
            std::cout << "Successfully deleted category\n";
            if(counter != 1)
                std::cout << "Program has deleted " << counter << " lines!\n";
            else
                std::cout << "Program has deleted " << counter << " line!\n";
        }
        else
            std::cout << "Couldn't find element to delete\n";
    }else
        std::cout << "Deleting category stopped coming back to main menu.\n";
}