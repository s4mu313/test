#ifndef SUPPORT_H
#define SUPPORT_H

#include <string>
#include <map>
#include <fstream>

/**
 * @brief Left trim.
 * @param str
 */
inline void
ltrim_(std::string& str);

/**
 * @brief Right trim.
 * @param str
 */
inline void
rtrim_(std::string& str);

/**
 * @brief Left and right trim.
 * @param s
 */
inline void
trim_(std::string& s);

/**
 * @brief Parse a line of file.
 * @param line_
 * @param section_
 * @param map_
 * @return 0 if SUCCESS, RETURN_CODE otherwise.
 */
unsigned short
parse_line_(const std::string& line_,
            const std::string& section_,
            std::map<std::string, std::string>& map_);

/**
 * @brief Read each line, trim and prepare the string to pass it to parse_line_.
 * @param line_
 * @param section_
 * @param map_
 * @return 0 if SUCCESS, RETURN_CODE otherwise.
 */
unsigned short
process_file_(const std::string& file_,
              std::map<std::string, std::string>& map_);

/**
 * @brief Write std::map on file as ini structured
 * @param line_
 * @param section_
 * @param map_
 * @return 0 if SUCCESS, RETURN_CODE otherwise.
 */
unsigned short
write_on_file(const std::string& path,
              std::map<std::string, std::string>& map_);


#endif // SUPPORT_H
