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

// Метод чтения содержимого текстового файла и возвращения его в виде вектора строк
std::vector<std::string> FileIOHandler::readText(const std::string& filename) const {
	// Проверяем расширение входного файла на соответствие допустимому формату
    std::unordered_set<std::string> extensions = { ".txt" };
    if (!checkExtension(filename, extensions)) {
        throw Error{ INVALID_EXTENSION };            // Завершаем работу, если формат файла неверный
    }

	// Открываем файл для чтения
    std::ifstream input_file(filename);
    // Проверяем физическое существование файла и права доступа на чтение
    if (!input_file.is_open()) {
        throw Error{ FILE_NOT_FOUND };             // Завершаем работу, если файл не удалось открыть
    }

    std::vector<std::string> lines;
    std::string current_line;

    // Построчно читаем файл, фильтруя строки, которые не содержат полезных данных
    while (std::getline(input_file, current_line)) {
        bool has_chars = std::any_of(current_line.begin(), current_line.end(), [](unsigned char c) {
            return !std::isspace(c);
            });

        // Если строка содержит полезные данные, сохраняем её для дальнейшего парсинга
        if (has_chars) {
            lines.push_back(current_line);
        }
    }

	// Если после фильтрации не осталось ни одной строки, считаем это ошибкой отсутствия входных данных
    if (lines.empty()) {
        throw Error{ NO_INPUT_DATA };
    }

	// Возвращаем вектор строк
    return lines;
}

// Метод парсинга строки для получения размерностей матрицы
void FileIOHandler::parseDimensions(const std::string& line, int& cols, int& rows) const {

    std::stringstream string_stream(line);
    std::vector<std::string> dimension_tokens;
    std::string token;

	// Разбиваем строку на токены, используя пробел в качестве разделителя, и сохраняем их в вектор
    while (string_stream >> token) { dimension_tokens.push_back(token); }

	// Проверяем количество полученных токенов. Должно быть ровно 2 - для количества столбцов и строк. Иначе - выбрасываем соответствующую ошибку
    if (dimension_tokens.empty()) throw Error{ NO_INPUT_DATA };
    if (dimension_tokens.size() > 2) throw Error{ DIMENSION_TOO_MANY_NUMBERS };
    if (dimension_tokens.size() < 2) throw Error{ DIMENSION_MISMATCH };

	// Проверяем, что оба токена являются целыми числами. 
    try {
        // Если в токене обнаружена точка или запятая, считаем это ошибкой вещественного числа.
        if (dimension_tokens[0].find('.') != std::string::npos || dimension_tokens[0].find(',') != std::string::npos ||
            dimension_tokens[1].find('.') != std::string::npos || dimension_tokens[1].find(',') != std::string::npos) {
            throw Error{ DIMENSION_NOT_INTEGER };
        }

        size_t size_cols, size_rows;

        // Пытаемся конвертировать оба токена в целые числа.
        cols = std::stoi(dimension_tokens[0], &size_cols);
        rows = std::stoi(dimension_tokens[1], &size_rows);

        // Если конвертировалась не вся строка, считаем это ошибкой недопустимых символов
        if (size_cols != dimension_tokens[0].length() || size_rows != dimension_tokens[1].length()) {
            throw Error{ DIMENSION_NOT_INTEGER };
        }
    }
    catch (...) {
        throw Error{ DIMENSION_NOT_INTEGER };
    }

	// Проверяем, что оба числа больше нуля. Иначе - выбрасываем ошибку
    if (cols <= 0 || rows <= 0) { throw Error{ DIMENSION_NEGATIVE }; }
	// Проверяем, что оба числа не превышают 100. Иначе - выбрасываем ошибку
    if (cols > 100) { throw Error{ OUT_OF_RANGE, cols }; }
    if (rows > 100) { throw Error{ OUT_OF_RANGE, rows }; }
}

// Метод чтения размерностей и данных матрицы из списка строк
void FileIOHandler::readDimensionsAndMatrix(const std::vector<std::string>& lines, int& cols, int& rows, std::vector<std::vector<int>>& matrix) {
	// Проверяем, что вектор строк не пустой. Если он пустой, считаем это ошибкой отсутствия входных данных
    if (lines.empty()) { throw Error{ NO_INPUT_DATA }; }

	// Парсим первую строку для получения размерностей матрицы
    parseDimensions(lines[0], cols, rows);

    std::set<std::string> float_errors;      
    std::set<std::string> invalid_char_errors; 
    std::vector<int> out_of_range_errors;     
    int row_index = 0; 

	// Проходим по каждой строке, начиная со второй, и парсим их для получения данных матрицы
    for (size_t i = 1; i < lines.size(); ++i) {
        std::stringstream row_stream(lines[i]);
        std::vector<int> row_data; 
        std::string token;

		// Разбиваем строку на токены, используя пробел в качестве разделителя
        while (row_stream >> token) {
            int value;
            
			// Если токен прошел валидацию, добавляем его числовое значение в текущую строку матрицы
            if (validateValue(token, value, float_errors, invalid_char_errors, out_of_range_errors)) {
                row_data.push_back(value); 
            }
        }

		// Если строка не содержит полезных данных и при этом не было обнаружено ошибок, пропускаем её
        if (row_data.empty() && float_errors.empty() && invalid_char_errors.empty() && out_of_range_errors.empty()) { continue; }

		// Если в строке не было обнаружено ошибок, но при этом количество элементов не совпало с заявленным количеством столбцов, выбрасываем ошибку
        if (float_errors.empty() && invalid_char_errors.empty() && out_of_range_errors.empty()) {
            if (row_data.size() != (size_t)cols) {
                throw Error{ ROW_FORMAT_ERROR, 0, "", row_index + 1, (int)row_data.size(), cols };
            }
			// Если количество элементов в строке совпало с заявленным количеством столбцов, добавляем её в матрицу
            matrix.push_back(row_data);
            row_index++; 
        }
    }

	// Если после обработки всех строк были обнаружены ошибки недопустимых символов, выбрасываем ошибку с перечислением всех таких токенов
    if (!invalid_char_errors.empty()) {
        std::string combined = "";
        for (const auto& bad_token : invalid_char_errors) {
            if (!combined.empty()) combined += " ";
            combined += bad_token;
        }
        throw Error{ INVALID_CHARACTER, 0, combined }; 
    }

	// Если после обработки всех строк были обнаружены ошибки вещественных чисел, выбрасываем ошибку с перечислением всех таких токенов
    if (!float_errors.empty()) {
        std::string combined = "";
        for (const auto& bad_token : float_errors) {
            if (!combined.empty()) combined += " ";
            combined += bad_token;
        }
        throw Error{ FLOAT_NUMBER, 0, combined }; 
    }

	// Если после обработки всех строк были обнаружены ошибки выхода за диапазон, выбрасываем ошибку с перечислением всех таких чисел
    if (!out_of_range_errors.empty()) {
        throw Error{ OUT_OF_RANGE, out_of_range_errors[0] };
    }

	// Если после обработки всех строк не было обнаружено ошибок, но при этом количество строк или столбцов не совпало с заявленным, выбрасываем ошибку
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
	output_file.close();    // Закрываем файл после записи
}