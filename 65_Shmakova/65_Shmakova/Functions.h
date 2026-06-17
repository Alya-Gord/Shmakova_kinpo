#pragma once
#include "Structures.h"
#include <vector>


/// @brief Функция для поиска максимальной подматрицы, состоящей из одинаковых элементов
/// @param[in] matrix матрица, в которой происходит поиск
/// @param[in] rows количество строк в матрице
/// @param[in] cols количество столбцов в матрице
/// @return Структура SubmatrixResult с параметрами найденной максимальной подматрицы
SubmatrixResult findMaxSubmatrix(const std::vector<std::vector<int>>& matrix, int rows, int cols);


/// @brief Функция для вычисления количества подряд идущих одинаковых элементов в каждой строке, начиная с каждой позиции
/// @param[in] matrix матрица, в которой происходит поиск
/// @param[in] rows количество строк в матрице
/// @param[in] cols количество столбцов в матрице
/// @param[out] consecutive_equal_elements матрица, содержащая количество подряд идущих одинаковых элементов в каждой строке
void calculateRightLengths(const std::vector<std::vector<int>>& matrix, int rows, int cols, std::vector<std::vector<int>>& consecutive_equal_elements);


/// @brief Функция сканирования матрицы вниз для определения максимального прямоугольника, начинающегося в заданной позиции
/// @param[in] matrix матрица, в которой происходит поиск
/// @param[in] start_row индекс строки начальной позиции
/// @param[in] start_col индекс элемента начальной позиции в строке
/// @param[in] rows количество строк в матрице
/// @param[in] consecutive_equal_elements матрица, содержащая количество подряд идущих одинаковых элементов в каждой строке
/// @param[in,out] res структура с параметрами максимальной подматрицы
void scanDownwards(const std::vector<std::vector<int>>& matrix, int start_row, int start_col, int rows, const std::vector<std::vector<int>>& consecutive_equal_elements, SubmatrixResult& res);


/// @brief Функция обновления параметров максимальной найденной подматрицы
/// @param[in] area площадь текущей подматрицы
/// @param[in] row номер верхней строки текущей подматрицы
/// @param[in] col номер левой колонки текущей подматрицы
/// @param[in] width ширина текущей подматрицы
/// @param[in] height высота текущей подматрицы
/// @param[in,out] res структура с параметрами максимальной подматрицы
void updateMaxMatrix(int area, int row, int col, int width, int height, SubmatrixResult& res);
