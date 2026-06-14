#include "pch.h"
#include "CppUnitTest.h"
#include "Functions.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testFindMaxSubmatrix)
    {
    public:
        void AssertSubmatrix(int exp_area, int exp_row, int exp_col, int exp_w, int exp_h, const SubmatrixResult& res) {
            Assert::AreEqual(exp_area, res.max_area);
            Assert::AreEqual(exp_row, res.min_row);
            Assert::AreEqual(exp_col, res.min_col);
            Assert::AreEqual(exp_w, res.max_width);
            Assert::AreEqual(exp_h, res.max_height);
        }

        // Подматрица является единичной
        TEST_METHOD(SingleElement) {
            std::vector<std::vector<int>> matrix = { {1} };
            SubmatrixResult res = findMaxSubmatrix(matrix, 1, 1);
            AssertSubmatrix(1, 0, 0, 1, 1, res);
        }

        // Подматрица задана вектор-строкой
        TEST_METHOD(VectorRow) {
            std::vector<std::vector<int>> matrix = {
                {2, 4, 6},
                {8, 8, 8},
                {3, 5, 7}
            };
            SubmatrixResult res = findMaxSubmatrix(matrix, 3, 3);
            AssertSubmatrix(3, 1, 0, 3, 1, res);
        }

        // Подматрица задана вектор-столбцом
        TEST_METHOD(VectorColumn) {
            std::vector<std::vector<int>> matrix = {
                {1, 9, 4},
                {2, 9, 5},
                {3, 9, 6}
            };
            SubmatrixResult res = findMaxSubmatrix(matrix, 3, 3);
            AssertSubmatrix(3, 0, 1, 1, 3, res);
        }

        // Наибольшая подматрица совпадает с матрицей 4*4
        TEST_METHOD(FullMatrixMatch) {
            std::vector<std::vector<int>> matrix = {
                {1, 1, 1, 1},
                {1, 1, 1, 1},
                {1, 1, 1, 1},
                {1, 1, 1, 1}
            };
            SubmatrixResult res = findMaxSubmatrix(matrix, 4, 4);
            AssertSubmatrix(16, 0, 0, 4, 4, res);
        }

        // Две подматрицы одинаковой размерности не пересекаются, выводим ближайшую
        TEST_METHOD(SelectionPrioritySubmatrix) {
            std::vector<std::vector<int>> matrix = {
                {0, 1, 5, 5},
                {1, 0, 5, 5},
                {2, 2, 1, 0},
                {2, 2, 0, 1}
            };
            SubmatrixResult res = findMaxSubmatrix(matrix, 4, 4);
            AssertSubmatrix(4, 0, 2, 2, 2, res);
        }

        // Подматрица находится в центре
        TEST_METHOD(SubmatrixBreakInMiddle) {
            std::vector<std::vector<int>> matrix = {
                {1, 1, 1},
                {1, 2, 1},
                {1, 1, 1}
            };
            SubmatrixResult res = findMaxSubmatrix(matrix, 3, 3);
            AssertSubmatrix(3, 0, 0, 3, 1, res);
        }

        // Отрицательные числа
        TEST_METHOD(NegativeNumbers) {
            std::vector<std::vector<int>> matrix = {
                {-5, -5, -5},
                {-5, -5, 5},
                {-5, -5, -5}
            };
            SubmatrixResult res = findMaxSubmatrix(matrix, 3, 3);
            AssertSubmatrix(6, 0, 0, 2, 3, res);
        }
    };
}