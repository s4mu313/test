#include <fstream>
#include <filesystem>

#include "../include/ini_parser.h"
#include "../include/support.h"

unsigned short
Ini_parser::get_value(const std::string& key,
                      std::string& value)
{
    if (status_ != RETURN_CODE::SUCCESS)     /// Can access only when file is loaded
        return status_;

    if (map_.find(key) == map_.end())
        return RETURN_CODE::MISSING_KEY;

    value = map_[key];

    return RETURN_CODE::SUCCESS;
}

/*****************************************************************************/

unsigned short
Ini_parser::set_value(const std::string& key,
                      const std::string& value)
{
    if (status_ != RETURN_CODE::SUCCESS)               /// Can access only when file is loaded
        return status_;

    if (key.find('.') == std::string::npos)            /// Section missing is an error
        return status_ = RETURN_CODE::GENERIC_ERROR;

    map_[key] = value;

    return status_ = RETURN_CODE(write_on_file(path_, map_));

}

/*****************************************************************************/

unsigned short
Ini_parser::load_resource(const std::string& path)
{
    if (!std::filesystem::exists(path))                /// Check if file exists before open it because
        return status_ = RETURN_CODE::FILE_NOT_EXISTS; /// fstream::in suppose the file to exists

    path_ = path;

    return status_ = RETURN_CODE(process_file_(path_, map_));
}
