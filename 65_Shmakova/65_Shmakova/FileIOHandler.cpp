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
        return false;                                                                                      // Возвращаем false
    }
    try {
        size_t len_num;
        value = std::stoi(token, &len_num);                                                               // Пытаемся конвертировать строку в число

        if (len_num != token.length()) {                                                                  // Если конвертировалась не вся строка
            error.insert(token);                                                                          // Добавляем токен с лишними символами в множество ошибок
            return false;
        }                                                                                                 // Возвращаем false

        if (value < -1000 || value > 1000) {                                                              // Если число выходит за рамки требуемого диапазона
            error.insert(token);                                                                          // Добавляем число в множество ошибок
            return false;
        }                                                                                                 // Возвращаем false
    }
    catch (...) {                                                                                         // Перехватываем исключения преобразования
        error.insert(token);                                                                              // Добавляем неконвертируемый токен в множество
        return false;                                                                                     // Возвращаем false
    }
    return true;                                                                                          // Если все проверки пройдены, возвращаем true
}

// Метод чтения матрицы из файла с полной валидацией данных.
void FileIOHandler::readMatrix(const std::string& filename, int& cols, int& rows, std::vector<std::vector<int>>& matrix) {}

// Метод записи результата в выходной файл.
void FileIOHandler::writeResult(const std::string& filename, const std::vector<std::vector<int>>& matrix, const SubmatrixResult& res) {}