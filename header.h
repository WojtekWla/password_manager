#include <iostream>
#include <vector>
#include <string>
#pragma once

/**
 * Struct where we save all corresponding information to one password
 */
struct Line{
    std::string name;
    std::string password;
    std::string category;
    std::string website_service;
    std::string login;

    Line() = default;

    /**
    * Constructor for one line from a file
    * @param name
    * @param password
    * @param category
    * @param websiteService
    * @param login
    */
    Line(const std::string &name, const std::string &password, const std::string &category,
         const std::string &websiteService, const std::string &login);
    /**
     * Overriding operator << to display object in the console
     * @return ostream
     */
    friend auto operator<<(std::ostream&,Line const& ) -> std::ostream&;

    /**
    * function which prepare object to be written to the specific file
    * @return string of object's elements
    */
    auto toString() -> const std::string;
};
/**
 * Load all files from a directory into a vector
 * @return vector with paths to the files
 */
auto load_files()-> std::vector<std::string>;
/**
 * choose file from a vector or create new file
 * @param files vector with paths to the files
 * @return string with path to the file on which we will operate
 */
auto choose_file(std::vector<std::string> const& files) -> std::string;

/**
 * decrypting and adding all lines into a vector and updating timestamp
 * @param all_lines container for decrypted data
 * @param categories container for categories
 * @param filePath path to file on which we are operating
 * @param password to decrypt and encryot data
 */
auto set_all_lines(std::vector<Line> &all_lines, std::set<std::string> &categories, std::string const& filePath, std::string const& password)-> void;

/**
 * function checks the time and prepare it to be saved to the specific file
 * @param file_path path to the file where the lines are saved
 * @param password string with which we are encrypting the file
 * @param vector consists of all decrypted Lines
 */
auto updateTimeStamp(std::string const& file_path, std::string const& password, std::vector<Line> const& vector) -> void ;
/**
 * function which encrypts the string
 * @param string_to_encrypt line which we want to encrypt
 * @param password string with which we are encrypting the file
 * @return
 */
auto encrypt_password(std::string& password_to_encrypt, std::string const& password ) -> std::string;
/**
 * function which decrypts string
 * @param string_to_decrypt line which we want to decrypt
 * @param password string with which we are encrypting the file
 */
auto decrypt_password(std::string& password_to_encrypt, std::string const& password ) -> void;
/**
 * writing every line into the file and it also saves current timestamp
 * @param file_path path to the file where the lines are saved
 * @param password string with which we are encrypting the file
 * @param time_stamp 3 element array which consists of hours minutes and seconds
 * @param vector consists of all decrypted Lines
 */
auto write_passwords_to_file(std::string const& file_path, std::string const& password, std::string* time_stamp, std::vector<Line> const& vector)-> void;
/**
 * Function which add new category to the set if and only if this category is not already in the set
 * @param categories set of all categories
 */
auto validate_string(std::string const& str)-> void;

/**
 * adding new category if the category is not already listed in the file
 * @param categories set of all categories
 */
auto add_category(std::set<std::string> & categories) -> void;

/**
 * Checks whether string is listed in categories the function is used in add_password function to check whether
 * inputted password is in categories or not
 * @param categories set of all categories
 */
auto check_category(std::set<std::string> &, std::string const& str)-> void ;
/**
 * Adding new password which will be saved to vector and encrypted with password and sent to the file using previously defined functions
 * @param categories set of all categories
 * @param password to encrypt and decrypt data
 * @param file_path path to file on which we are operating
 * @param vec contains all decrypted data
 */
auto add_password(std::set<std::string> & categories, std::string const& password, std::string const& file_path, std::vector<Line> & vec) -> void;
/**
 * displaying all currently saved passwords from the file (is uses container which has all those passwords)
 * @param vec contains all decrypted data
 */
auto display_all_passwords(std::vector<Line> const& vec) -> void;

/**
* generating random password it is used by add_password function which provide option of generating random password
* @return returns new randomly generating string
*/
auto generate_random_password() -> std::string;
/**
 * function which is used in searching function it looks for similar password in vector
 * @param input string which was provided by user
 * @param password
 * @return
 */
auto approximate_password(std::string const& input, std::string & password)-> bool;
/**
 * function which find passwords using multiple searching options
 * @param vec
 */
auto search_passwords(std::vector<Line> const &vec) -> void ;
/**
 * sorting password; password can be sorted in 3 ways by name and password, category or website/service and login
 * @param vector contains all decrypted data
 */
auto sort_passwords(std::vector<Line> & vector) -> void;
/**
 * Function which edit password we can find this password by 4 options name, password website/service or login
 * @param vector contains all decrypted data
 * @param file_path path to file on which we are operating
 * @param password to encrypt and decrypt data
 */
auto edit_password(std::vector<Line> & vector, std::string const&, std::string const&) -> void;
/**
 * deleting password from file it can be done by multiple options; if program find more than one password which fits requirements it deletes it too
 * @param vector contains all decrypted data
 * @param file_path path to file on which we are operating
 * @param password to encrypt and decrypt data
 */
auto delete_password(std::vector<Line> & vector, std::string const&, std::string const&)-> void;
/**
 * deleting category from container and deleting all password which fit inputed category
 * @param categories set of all categories
 * @param vector contains all decrypted data
 * @param file_path path to file on which we are operating
 * @param password to encrypt and decrypt data
 */
auto delete_category(std::set<std::string> &categories, std::vector<Line>& vector, std::string const& file_path, std::string const& password)-> void;


