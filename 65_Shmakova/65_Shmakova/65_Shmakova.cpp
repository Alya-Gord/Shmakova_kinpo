#include <iostream>
#include "Functions.h"

// Структура для хранения информации об ошибке и генерации сообщения
std::string Error::generate_message() {
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

SubmatrixResult findMaxSubmatrix(const std::vector<std::vector<int>>& matrix, int rows, int cols) { return {}; }

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

void scanDownwards(const std::vector<std::vector<int>>&, int, int, int, const std::vector<std::vector<int>>&, SubmatrixResult&) {}

void updateMaxMatrix(int area, int row, int col, int width, int height, SubmatrixResult& res) {
    if (area > res.max_area) {               // Если площадь текущей подматрицы больше максимальной известной
        res.max_area = area;                 // Обновляем рекорд максимальной площади
        res.min_row = row;                   // Сохраняем номер верхней строки лучшей подматрицы
        res.min_col = col;                   // Сохраняем номер левого столбца лучшей подматрицы
        res.max_width = width;               // Сохраняем ширину найденной лучшей подматрицы
        res.max_height = height;             // Сохраняем высоту найденной лучшей подматрицы
    }
}

int main()
{
    std::cout << "Hello World!\n";
}