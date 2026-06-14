#pragma once

#include <string>

// Перечисление для типов ошибок, которые могут возникать при работе с файлами и данными
enum ErrorType {
    FILE_NOT_FOUND,
    OUTPUT_FILE_ERROR,
    DIMENSION_MISMATCH,
    INVALID_CHARACTER,
    FLOAT_NUMBER,
    OUT_OF_RANGE,
    DIMENSION_NOT_INTEGER,
    DIMENSION_NEGATIVE,
    DIMENSION_TOO_MANY_NUMBERS,
    MATRIX_MISSING,
    NO_INPUT_DATA,
    INVALID_EXTENSION,
    INVALID_ARGUMENTS,
    ROW_FORMAT_ERROR
};

// Структура для хранения информации об ошибке и генерации соответствующего сообщения
struct Error {
    ErrorType type;
    int incorrect_value;
    std::string token;
    int row;
    int actual_count;
    int expected_count;

    std::string generate_message();
};

// Структура для хранения результатов поиска подматрицы
struct SubmatrixResult {
    int max_area = 0;
    int min_row = 0;
    int min_col = 0;
    int max_width = 0;
    int max_height = 0;
};
