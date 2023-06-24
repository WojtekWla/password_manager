#include <iostream>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include <cstdlib>
#include <conio.h>
#include <thread>
#include <chrono>
#include "header.h"


int main() {

    auto files = load_files();
    auto filePath = choose_file(files);
    auto all_lines = std::vector<Line>();
    auto categories = std::set<std::string>();
    auto password = std::string();

    std::cout << "Welcome to password manager\n"
                 "---------------------------\n"
                 "Insert password\n";

    std::getline(std::cin, password);
    set_all_lines(all_lines, categories, filePath, password);
    while(true)
    {
        auto to_choose = std::string();
        std::cout << "Choose what do you want to do: \n"
                     "1. Search passwords\n"
                     "2. Sort passwords\n"
                     "3. Add password\n"
                     "4. Edit password\n"
                     "5. Delete password\n"
                     "6. Add category\n"
                     "7. Delete category\n"
                     "8. Display all passwords\n"
                     "9. Exit program\n";
        std::cin >> to_choose;
        std::cin.ignore();

        char option = to_choose[0];

        switch(option) {
            case '1': {
                search_passwords(all_lines);
                break;
            }
            case '2':
                //Sort passwords
                sort_passwords(all_lines);
                break;
            case '3': {
                add_password(categories, password, filePath, all_lines);
                break;
            }
            case '4': {
                edit_password(all_lines, filePath, password);
                break;
            }
            case '5': {
                //delete password
                delete_password(all_lines, filePath, password);
                break;
            }
            case '6':
                //add category
                add_category(categories);
                break;
            case '7': {
                //delete category
                delete_category(categories, all_lines, filePath, password);
                break;
            }
            case '8': {
                auto line = std::string();
                display_all_passwords(all_lines);
                break;
            }
            case '9':{
                std::cout << "Closing application\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                goto exit_loop;
                break;
            }
            default:
                std::cout << "Wrong input\n";
        }

        std::cout << "Click enter to continue...\n";
        getch();
    }
    exit_loop: ;
    return 0;
}
