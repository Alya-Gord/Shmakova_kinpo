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
bool FileIOHandler::validateValue(const std::string& token, int& value, std::set<std::string>& float_errors,
    std::set<std::string>& invalid_char_errors, std::vector<int>& out_of_range_errors) {
    // Проверяем, является ли число дробным
    if (token.find('.') != std::string::npos || token.find(',') != std::string::npos) {
		float_errors.insert(token);            // Если обнаружена точка или запятая, считаем это ошибкой вещественного числа
        return false;
    }

	// Пытаемся конвертировать строку в целое число
    try {
        size_t len_num;
        value = std::stoi(token, &len_num);

		// Если конвертировалась не вся строка, считаем это ошибкой недопустимых символов
        if (len_num != token.length()) {
            invalid_char_errors.insert(token);
            return false;
        }

		// Проверяем, входит ли число в допустимый диапазон [-1000...1000].
        if (value < -1000 || value > 1000) {
            out_of_range_errors.push_back(value);    //Если нет, считаем это ошибкой выхода за диапазон
            return false;
        }
    }
	// Если при попытке конвертации возникло исключение, считаем это ошибкой недопустимых символов
    catch (...) {
        invalid_char_errors.insert(token);
        return false;
    }

    return true;
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