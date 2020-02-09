#include <algorithm>

#include "../include/support.h"
#include "../include/types.h"

void
rtrim_(std::string &str)
{
    str.erase(std::find_if_not(str.rbegin(),
                               str.rend(),
                               [](unsigned char c) { return std::isspace(c); }).base(),
              str.end());
}

void
ltrim_(std::string &str)
{
    str.erase(str.begin(),
              std::find_if_not(str.begin(),
                               str.end(),
                               [](unsigned char c) { return std::isspace(c); }));
}

void
trim_(std::string &s)
{
    ltrim_(s);
    rtrim_(s);
}

unsigned short
parse_line_(const std::string &line_,
            const std::string &section_,
            std::map<std::string, std::string> &map_)
{
    if (line_.find_first_of('=') == std::string::npos || section_ == "")
        return RETURN_CODE::READ_ERROR;

    try {
        std::string key = line_.substr(0, line_.find_first_of('='));
        trim_(key);
        key = section_ + "." + key;
        std::string value = line_.substr(line_.find_first_of('=') + 1);
        trim_(value);
        map_.insert(std::pair<std::string, std::string>(key, value)); /// Maybe mem.?
    }  catch (std::exception&) {
        return RETURN_CODE::GENERIC_ERROR;
    }

    return RETURN_CODE::SUCCESS;

}

unsigned short
process_file_(const std::string& path,
              std::map<std::string, std::string> &map_)
{
    std::ifstream in_file(path);

    if (!in_file.is_open())
        return RETURN_CODE::FILE_NOT_LOADED;

    std::string line, section;

    while (std::getline(in_file, line)) {
        trim_(line);
        if (line != "" && line[0] != ';') {
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                section = line.substr(1, line.size() - 2);
                trim_(section);
            } else if (auto code = parse_line_(line, section, map_); code) {
                in_file.close();
                map_.clear();
                return RETURN_CODE(code);
            }
        }
    }
    in_file.close();
    return RETURN_CODE::SUCCESS;

}

unsigned short
write_on_file(const std::string& path,
              std::map<std::string, std::string> &map_)
{
    std::ofstream out_file(path, std::fstream::out | std::fstream::trunc);

    if (!out_file.is_open())
        return RETURN_CODE::FILE_NOT_LOADED;

    try {
        std::string section, previous_sec;
        for (auto it = map_.begin(); it != map_.end(); ++it) {

            section = it->first.substr(0, it->first.find('.'));

            if (previous_sec != section) {
                previous_sec = section;
                out_file << "[" << section << "]\n";
            }

            out_file << it->first.substr(it->first.find('.') + 1) << " = " << it->second << "\n";
        }
    }  catch (std::exception&) {
        out_file.close();
        return RETURN_CODE::GENERIC_ERROR;
    }

    out_file.close();

    return RETURN_CODE::SUCCESS;
}
