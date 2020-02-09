#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <map>

#include "types.h"

class Ini_parser {

public:

    /// ctor. Set status_ to file_not_loaded.
    Ini_parser()
        : status_(RETURN_CODE::FILE_NOT_LOADED)
    {}

    /**
     * @brief Open an ini file and load in memory.
     * @param std::string path
     * @return 0 if succeded otherwise a value in RETURN_CODE
     */
    unsigned short
    load_resource(const std::string& path);

    /**
     * @brief Get a value from the ini file previously loaded.
     * @param key
     * @param value
     * @return 0 if succeded otherwise a value in RETURN_CODE
     */
    unsigned short
    get_value(const std::string& key,
              std::string& value);

    /**
     * @brief Set a value into the ini file previously loaded.
     * @param key
     * @param value
     * @return 0 if succeded otherwise a value in RETURN_CODE
     */
    unsigned short
    set_value(const std::string& key,
              const std::string& value);

private:
    std::map<std::string, std::string> map_;
    std::string path_;
    RETURN_CODE status_;

};

#endif // INI_PARSER_H
