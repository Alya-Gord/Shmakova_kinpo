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

// Метод парсинга первой строки с размерностями матрицы и валидации полученных данных
void FileIOHandler::parseDimensions(const std::string& line, int& cols, int& rows) {
    std::stringstream string_stream(line);
    std::vector<std::string> dimension_tokens;
    std::string token;

    // Извлекаем все слова из строки, чтобы проконтролировать точное количество аргументов
    while (string_stream >> token) {
        dimension_tokens.push_back(token);
    }

    // Валидируем полученные значения и выбрасываем исключение в случае обнаружения ошибок
    if (dimension_tokens.empty()) throw Error{ NO_INPUT_DATA };
    if (dimension_tokens.size() > 2) throw Error{ DIMENSION_TOO_MANY_NUMBERS };
    if (dimension_tokens.size() < 2) throw Error{ DIMENSION_MISMATCH };

    // Проверяем, не являются ли размерности дробными числами
    try {
        if (dimension_tokens[0].find('.') != std::string::npos ||
            dimension_tokens[1].find('.') != std::string::npos) {
            throw Error{ DIMENSION_NOT_INTEGER };  // В случае обнаружения точки в любом из токенов, выбрасываем исключение.
        }

        // Конвертируем строковые токены в целочисленные значения
        cols = std::stoi(dimension_tokens[0]);
        rows = std::stoi(dimension_tokens[1]);
    }
    catch (...) {
        throw Error{ DIMENSION_NOT_INTEGER }; // Если конвертация не удалась, выбрасываем исключение.
    }

    // Проверяем, что полученные размерности положительные числа
    if (cols <= 0 || rows <= 0) throw Error{ DIMENSION_NEGATIVE };
}

// Метод чтения строк матрицы из файла с валидацией данных
void FileIOHandler::readMatrixRows(std::ifstream& input_file, int cols, int rows, std::vector<std::vector<int>>& matrix) {
    std::string current_line;
    int row_index = 0;
    std::set<std::string> file_errors;

    // Читаем файл построчно, начиная со второй строки, и валидируем каждую строку и каждый элемент в ней
    while (std::getline(input_file, current_line)) {
        std::stringstream row_stream(current_line);
        std::vector<int> row_data;
        std::string token;

        // Поэлементный разбор и валидация текущей строки
        while (row_stream >> token) {
            int value;
            // Если элемент прошел валидацию, добавляем его в текущую строку данных
            if (validateValue(token, value, file_errors)) { row_data.push_back(value); }
        }

        if (row_data.empty() && file_errors.empty()) continue;       // Игнорируем пустые строки, если в них не было найдено ошибочных символов

        // Если в строке нет ошибок, но количество элементов не совпало с заявленным количеством столбцов, выбрасываем исключение
        if (row_data.size() != static_cast<size_t>(cols) && file_errors.empty()) {
            throw Error{ ROW_FORMAT_ERROR, 0, "", row_index + 1, static_cast<int>(row_data.size()), cols };
        }

        matrix.push_back(row_data);    // Добавляем валидную строку данных в матрицу
        row_index++;
    }

    // Если за время парсинга матрицы накопились ошибочные элементы, формируем строку с перечислением всех таких элементов и выбрасываем исключение
    if (!file_errors.empty()) {
        std::string combined_errors_string = "";
        for (const auto& bad_token : file_errors) {
            if (!combined_errors_string.empty()) combined_errors_string += " ";
            combined_errors_string += bad_token;
        }
        throw Error{ INVALID_CHARACTER, 0, combined_errors_string };
    }

    // Финальная сверка фактического количества прочитанных строк с заявленной размерностью
    if (row_index == 0) throw Error{ MATRIX_MISSING };
    if (row_index != rows) throw Error{ DIMENSION_MISMATCH };
}

// Метод чтения матрицы из файла с полной валидацией данных
void FileIOHandler::readMatrix(const std::string& filename, int& cols, int& rows, std::vector<std::vector<int>>& matrix) {
    std::unordered_set<std::string> extensions = { ".txt" };

    // Проверяем расширение файла. Если расширение не совпало - выбрасываем исключение
    if (!checkExtension(filename, extensions)) throw Error{ INVALID_EXTENSION };

    // Открываем файл для чтения. Если файл не открылся, выбрасываем исключение
    std::ifstream input_file(filename);
    if (!input_file.is_open()) throw Error{ FILE_NOT_FOUND };

    // Пытаемся считать первую строку. Если пусто - выбрасываем исключение
    std::string first_line;
    if (!std::getline(input_file, first_line)) throw Error{ NO_INPUT_DATA };

    // Парсим размерности матрицы и валидируем полученные данные. Если обнаружены ошибки, выбрасываем исключение
    parseDimensions(first_line, cols, rows);

    // Читаем строки матрицы из файла, валидируя каждую строку и каждый элемент. Если обнаружены ошибки, выбрасываем исключение
    readMatrixRows(input_file, cols, rows, matrix);
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