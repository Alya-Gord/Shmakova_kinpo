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

// Метод валидации отдельного токена из файла.
bool FileIOHandler::validateValue(const std::string& token, int& value, std::set<std::string>& error) { return false; }

// Метод чтения матрицы из файла с полной валидацией данных.
void FileIOHandler::readMatrix(const std::string& filename, int& cols, int& rows, std::vector<std::vector<int>>& matrix) {}

// Метод записи результата в выходной файл.
void FileIOHandler::writeResult(const std::string& filename, const std::vector<std::vector<int>>& matrix, const SubmatrixResult& res) {}