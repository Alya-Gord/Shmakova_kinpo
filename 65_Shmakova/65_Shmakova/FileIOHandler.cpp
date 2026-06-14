#include "FileIOHandler.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

// Метод проверки расширения файла на соответствие заданному множеству расширений
bool FileIOHandler::checkExtension(const std::filesystem::path& filename, const std::unordered_set<std::string>& extensions) const {
    std::string ext = filename.extension().string();                                                        // Извлечь расширение файла из переданного пути

    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });   // Привести полученное расширение к нижнему регистру

    return extensions.find(ext) != extensions.end();                                                        // Если расширение найдено в наборе допустимых - вернуть true, иначе false
}

// Метод валидации отдельного токена из файла
bool FileIOHandler::validateValue(const std::string& token, int& value, std::set<std::string>& error) {
    if (token.find('.') != std::string::npos || token.find(',') != std::string::npos) {                    // Проверяем, является ли число дробным
        error.insert(token);                                                                               // Добавляем ошибочный токен в множество ошибок
        return false;
    }

    try {
        size_t len_num;
        value = std::stoi(token, &len_num);                                                               // Пытаемся конвертировать строку в число

        if (len_num != token.length()) {                                                                  // Если конвертировалась не вся строка
            error.insert(token);                                                                          // Добавляем токен с лишними символами в множество ошибок
            return false;
        }

        if (value < -1000 || value > 1000) {                                                              // Если число выходит за рамки требуемого диапазона
            error.insert(token);                                                                          // Добавляем число в множество ошибок
            return false;
        }
    }
    catch (...) {                                                                                         // Перехватываем исключения преобразования
        error.insert(token);                                                                              // Добавляем неконвертируемый токен в множество
        return false;
    }
    return true;                                                                                          // Если все проверки пройдены, возвращаем true
}

// Метод чтения матрицы из файла с полной валидацией данных
void FileIOHandler::readMatrix(const std::string& filename, int& cols, int& rows, std::vector<std::vector<int>>& matrix) {
    // Проверяем расширение файла
    std::unordered_set<std::string> extensions = { ".txt" };
    if (!checkExtension(filename, extensions)) throw Error{ INVALID_EXTENSION };

    // Если файл не открылся, выбрасываем ошибку
    std::ifstream input_file(filename);
    if (!input_file.is_open()) throw Error{ FILE_NOT_FOUND };

    // Пытаемся считать первую строку. Если пусто - ошибка
    std::string current_line;
    if (!std::getline(input_file, current_line)) throw Error{ NO_INPUT_DATA };

    std::stringstream string_stream(current_line);
    std::vector<std::string> dimension_tokens;
    std::string token;

    while (string_stream >> token) dimension_tokens.push_back(token);                                 // Разбиваем первую строку на слова по пробелам
    if (dimension_tokens.empty()) throw Error{ NO_INPUT_DATA };                                       // Если слов нет - файл пуст
    if (dimension_tokens.size() > 2) throw Error{ DIMENSION_TOO_MANY_NUMBERS };                       // Если чисел больше 2, размерность задана неверно
    if (dimension_tokens.size() < 2) throw Error{ DIMENSION_MISMATCH };                               // Если число 1, размерность задана неверно

    try {
        // Проверка на то, что числа размерности не являются дробными
        if (dimension_tokens[0].find('.') != std::string::npos || dimension_tokens[1].find('.') != std::string::npos) throw Error{ DIMENSION_NOT_INTEGER };

        cols = std::stoi(dimension_tokens[0]);
        rows = std::stoi(dimension_tokens[1]);

        // Проверка на то, что числа размерности не являются нулевыми или отрицательными
        if (cols <= 0 || rows <= 0) throw Error{ DIMENSION_NEGATIVE };
    }
    catch (...) { throw Error{ DIMENSION_NOT_INTEGER }; }                                      // При любой ошибке парсинга выбрасываем ошибку нецелочисленной размерности

    int row_index = 0;
    std::set<std::string> file_errors;

    while (std::getline(input_file, current_line)) {                                           // Построчно считываем остаток файла
        if (current_line.empty()) continue;                                                    // Пропускаем пустые строки

        std::stringstream row_stream(current_line);
        std::vector<int> row_data;

        while (row_stream >> token) {                                                          // Читаем токены из строки до конца
            int value;
            if (validateValue(token, value, file_errors))                                      // Вызываем валидацию элемента. Если элемент невалидный, он будет добавлен в множество ошибок
                row_data.push_back(value);                                                     // Добавляем валидное число в строку
        }

        if (row_data.size() != (size_t)cols && file_errors.empty())                            // Если количество чисел в строке не совпало с заявленным
            throw Error{ ROW_FORMAT_ERROR, 0, "", row_index + 1, (int)row_data.size(), cols }; // Выбрасываем ошибку формата строки

        matrix.push_back(row_data);                                                            // Добавляем считанную строку в матрицу
        row_index++;                                                                           // Увеличиваем счетчик считанных строк
    }

    // Если за время парсинга матрицы накопились ошибочные элементы
    if (!file_errors.empty()) {
        std::string combined_errors_string = "";
        for (const auto& bad_token : file_errors) {                                            // Проходимся по всем собранным ошибочным токенам
            combined_errors_string += bad_token + " ";                                         // Приклеиваем каждый токен в строку через пробел
        }
        throw Error{ INVALID_CHARACTER, 0, combined_errors_string };                           // Выбрасываем единое исключение, демонстрирующее все найденные невалидные элементы
    }

    if (row_index == 0) throw Error{ MATRIX_MISSING };
    if (row_index != rows) throw Error{ DIMENSION_MISMATCH };
}

// Метод записи результата в выходной файл
void FileIOHandler::writeResult(const std::string& filename, const std::vector<std::vector<int>>& matrix, const SubmatrixResult& res) {
    std::unordered_set<std::string> extensions = { ".txt" };                           // Проверить расширение выходного файла
    if (!checkExtension(filename, extensions)) throw Error{ INVALID_EXTENSION };       // Если расширение не совпало - выбросить ошибку

    std::ofstream output_file(filename);                                               // Открываем файл для записи
    if (!output_file.is_open()) throw Error{ OUTPUT_FILE_ERROR };                      // Если файл не удалось открыть - выбросить ошибку

    // Проходимся по строкам и столбцам найденной лучшей подматрицы
    for (int row_index = res.min_row; row_index < res.min_row + res.max_height; row_index++) {
        for (int col_index = res.min_col; col_index < res.min_col + res.max_width; col_index++) {
            // Записываем число в файл, добавляя пробел после каждого числа, кроме последнего в строке
            output_file << matrix[row_index][col_index] << (col_index < res.min_col + res.max_width - 1 ? " " : "");
        }
        output_file << "\n";                                                           // Переходим на новую строку в файле после окончания строки матрицы
    }
}