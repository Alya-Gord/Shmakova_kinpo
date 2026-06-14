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

void calculateRightLengths(const std::vector<std::vector<int>>&, int, int, std::vector<std::vector<int>>&) {}

void scanDownwards(const std::vector<std::vector<int>>&, int, int, int, const std::vector<std::vector<int>>&, SubmatrixResult&) {}

void updateMaxMatrix(int, int, int, int, int, SubmatrixResult&) {}

int main()
{
    std::cout << "Hello World!\n";
}