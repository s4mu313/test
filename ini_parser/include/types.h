#ifndef TYPES_H
#define TYPES_H

/**
 * @brief RETURN_CODE enum.
 */
enum RETURN_CODE
{
    SUCCESS = 0,
    READ_ERROR = 1,
    FILE_NOT_EXISTS = 2,
    MISSING_KEY = 3,
    FILE_NOT_LOADED = 4,
    GENERIC_ERROR = 255
};

#endif // TYPES_H
