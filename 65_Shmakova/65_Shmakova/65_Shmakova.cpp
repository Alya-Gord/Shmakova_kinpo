#include <iostream>
#include "Functions.h"
#include "Structures.h"
#include "FileIOHandler.h"

// Структура для хранения информации об ошибке и генерации сообщения
std::string Error::generate_message() const {
    {
        if (type == FILE_NOT_FOUND) return "Неверно указан файл с входными данными. Возможно, файл не существует.";
        if (type == OUTPUT_FILE_ERROR) return "Неверно указан файл для выходных данных. Проверьте права на запись.";
        if (type == DIMENSION_MISMATCH) return "Количество строк/столбцов не совпадает. Проверьте корректность входных данных.";
        if (type == INVALID_CHARACTER) return "Во входных данных найдены недопустимые элементы: «" + token + "».";
        if (type == FLOAT_NUMBER) return "В файле присутствуют не целые числа: " + token + ".";
        if (type == OUT_OF_RANGE) return "Элемент " + std::to_string(incorrect_value) + " не принадлежит диапазону [-1000...1000].";
        if (type == DIMENSION_NOT_INTEGER) return "Размерность матрицы должна задаваться целыми числами.";
        if (type == DIMENSION_NEGATIVE) return "Размерность матрицы должна задаваться числами больше нуля.";
        if (type == DIMENSION_TOO_MANY_NUMBERS) return "Размерность матрицы задаётся не двумя цифрами. Перенесите массив на новую строку.";
        if (type == MATRIX_MISSING) return "В файле матрица не обнаружена. Добавьте значения элементов.";
        if (type == NO_INPUT_DATA) return "В файле не обнаружено входных данных.";
        if (type == INVALID_EXTENSION) return "Недопустимое расширение файла.";
        if (type == INVALID_ARGUMENTS) return "Необходимо указать два аргумента командной строки: входной и выходной файлы.";
        if (type == ROW_FORMAT_ERROR) return "В строке номер " + std::to_string(row) + ": " + std::to_string(actual_count) + " элемент(а/ов), ожидалось " + std::to_string(expected_count) + ".";
        return "Неизвестная ошибка";
    }
};

// Функция поиска самой большой подматрицы из одинаковых элементов
SubmatrixResult findMaxSubmatrix(const std::vector<std::vector<int>>& matrix, int rows, int cols) {
    SubmatrixResult res;
    if (rows == 0 || cols == 0) return res;                                                          // Если матрица пустая, сразу завершаем работу функции

    std::vector<std::vector<int>> consecutive_equal_elements(rows, std::vector<int>(cols, 0));       // Создаем вспомогательную матрицу для хранения длин одинаковых элементов
    calculateRightLengths(matrix, rows, cols, consecutive_equal_elements);                           // Заполняем вспомогательную матрицу длинами подряд идущих одинаковых элементов

    for (int row_index = 0; row_index < rows; row_index++) {                                         // Перебираем все элементы
        for (int col_index = 0; col_index < cols; col_index++) {
            scanDownwards(matrix, row_index, col_index, rows, consecutive_equal_elements, res);      // От каждой ячейки пытаемся построить прямоугольник вниз и найти максимальный
        }
    }
    return res;
}

// Функция предварительного вычисления количества подряд идущих одинаковых элементов вправо для каждого элемента
void calculateRightLengths(const std::vector<std::vector<int>>& matrix, int rows, int cols, std::vector<std::vector<int>>& len) {
    for (int row_index = 0; row_index < rows; row_index++) {                                            // Проходимся по каждой строке матрицы
        len[row_index][cols - 1] = 1;                                                                   // Для самого правого элемента длина цепочки всегда равна 1
        for (int col_index = cols - 2; col_index >= 0; col_index--) {                                   // Идем справа налево, пропуская самый правый элемент

            if (matrix[row_index][col_index] == matrix[row_index][col_index + 1])                       // Если текущий элемент равен соседу справа
                len[row_index][col_index] = len[row_index][col_index + 1] + 1;                          // Длина цепочки для текущего равна длине цепочки соседа + 1
			else                                                                                        // Иначе цепочка прерывается
                len[row_index][col_index] = 1;

        }
    }
}

// Функция поиска максимального прямоугольника вниз от стартового элемента
void scanDownwards(const std::vector<std::vector<int>>& matrix, int start_row, int start_col, int rows,
    const std::vector<std::vector<int>>& len, SubmatrixResult& res) {
    int current_min_width = len[start_row][start_col];                                         // Изначально ширина равна длине цепочки одинаковых элементов в текущей строке
    int target_value = matrix[start_row][start_col];                                           // Запоминаем значение элемента, из которого строим прямоугольник

    // Спускаемся вниз по столбцу, пока не выйдем за пределы матрицы и пока значения совпадают со стартовым
    for (int row = start_row; row < rows && matrix[row][start_col] == target_value; row++) {
        if (len[row][start_col] < current_min_width)                                           // Если цепочка одинаковых элементов в текущей строке короче
            current_min_width = len[row][start_col];                                           // Обновить минимальную ширину

        int height = row - start_row + 1;                                                      // Вычислить высоту прямоугольника на текущем шаге
        int area = current_min_width * height;                                                 // Вычислить площадь получившегося прямоугольника

        // Проверить, не является ли новый прямоугольник самым большим из найденных
        updateMaxMatrix(area, start_row, start_col, current_min_width, height, res);
    }
}

// Функция обновления параметров максимальной найденной подматрицы, если текущая площадь больше известной.
void updateMaxMatrix(int area, int row, int col, int width, int height, SubmatrixResult& res) {
    if (area > res.max_area) {               // Если площадь текущей подматрицы больше максимальной известной
        res.max_area = area;                 // Обновляем рекорд максимальной площади
        res.min_row = row;                   // Сохраняем номер верхней строки лучшей подматрицы
        res.min_col = col;                   // Сохраняем номер левого столбца лучшей подматрицы
        res.max_width = width;               // Сохраняем ширину найденной лучшей подматрицы
        res.max_height = height;             // Сохраняем высоту найденной лучшей подматрицы
    }
}

/*! \mainpage Search Largest Submatrix Elements
 *
 * \section intro_sec Program functionality
 *
 * This program is designed to search for and select in a two-dimensional array a rectangular region of maximum area, consisting of identical integer elements.
 *
 * \section incstall_sec Using the program
 *
 * 65_Shmakova.exe input.txt output.txt
 */

// Функция main
int main(int argc, char* argv[]) {
    FileIOHandler io_handler;
    int cols = 0, rows = 0;
    std::vector<std::vector<int>> matrix;

	// Пробуем выполнить основную логику программы
    try {
		if (argc != 3) { throw Error{ INVALID_ARGUMENTS }; }                 // Проверяем, что количество аргументов командной строки равно 2. Иначе - выбрасываем ошибку

		std::vector<std::string> lines = io_handler.readText(argv[1]);       // Читаем строки из входного файла

		io_handler.readDimensionsAndMatrix(lines, cols, rows, matrix);       // Парсим строки для получения размерностей и данных матрицы

		SubmatrixResult result = findMaxSubmatrix(matrix, rows, cols);       // Ищем параметры самой большой подматрицы из одинаковых элементов

		if (result.max_area == 0) { throw Error{ MATRIX_MISSING }; }         // Если площадь найденной подматрицы равна нулю, считаем это ошибкой отсутствия матрицы

		io_handler.writeResult(argv[2], matrix, result);                     // Записываем результат в выходной файл

		std::cout << "Расчет успешно завершен! Результат сохранен в файл: " << argv[2] << "\n";   // Выводим сообщение об успешном завершении программы
    }
	// Если в процессе выполнения программы была выброшена ошибка, перехватываем её и выводим соответствующее сообщение
    catch (const Error& e) {
        std::cout << e.generate_message() << "\n";
        return 1;
    }

    return 0;
}