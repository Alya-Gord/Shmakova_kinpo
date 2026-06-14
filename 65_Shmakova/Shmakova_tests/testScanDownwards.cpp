#include "pch.h"
#include "CppUnitTest.h"
#include "Functions.h"
#include <vector>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testScanDownwards)
    {
    public:
        // Вспомогательная функция для проверки всех полей структуры.
        void AssertSubmatrix(int exp_area, int exp_row, int exp_col, int exp_w, int exp_h, const SubmatrixResult& res) {
            Assert::AreEqual(exp_area, res.max_area);
            Assert::AreEqual(exp_row, res.min_row);
            Assert::AreEqual(exp_col, res.min_col);
            Assert::AreEqual(exp_w, res.max_width);
            Assert::AreEqual(exp_h, res.max_height);
        }

        // Проверка на остановку сканирования при несовпадении значения элемента в следующей строке.
        TEST_METHOD(ValueMismatchStopsLoop) {
            std::vector<std::vector<int>> matrix = {
                {5, 5},
                {3, 5}
            };
            std::vector<std::vector<int>> consecutive = {
                {2, 1},
                {1, 1}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 2, consecutive, res);
            AssertSubmatrix(2, 0, 0, 2, 1, res);
        }

        // Проверка, что сканирование успешно доходит до самой нижней границы матрицы.
        TEST_METHOD(ReachesMatrixBottom) {
            std::vector<std::vector<int>> matrix = {
                {5},
                {5}
            };
            std::vector<std::vector<int>> consecutive = {
                {1},
                {1}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 2, consecutive, res);
            AssertSubmatrix(2, 0, 0, 1, 2, res);
        }

        // Проверка, когда сканирование начинается из самой последней строки матрицы.
        TEST_METHOD(StartAtBottomRow) {
            std::vector<std::vector<int>> matrix = {
                {1},
                {5}
            };
            std::vector<std::vector<int>> consecutive = {
                {1},
                {1}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 1, 0, 2, consecutive, res);
            AssertSubmatrix(1, 1, 0, 1, 1, res);
        }

        // Проверка постепенного монотонного сужения ширины подматрицы с каждым шагом вниз.
        TEST_METHOD(MonotonicallyDecreasingWidth) {
            std::vector<std::vector<int>> matrix = {
                {5, 5, 5},
                {5, 5, 0},
                {5, 0, 0}
            };
            std::vector<std::vector<int>> consecutive = {
                {3, 2, 1},
                {2, 1, 0},
                {1, 0, 0}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 3, consecutive, res);
            AssertSubmatrix(4, 0, 0, 2, 2, res);
        }

        // Проверка ограничения ширины самой первой строкой.
        TEST_METHOD(WidthLimitedByStartRow) {
            std::vector<std::vector<int>> matrix = {
                {5, 0, 0},
                {5, 5, 5}
            };
            std::vector<std::vector<int>> consecutive = {
                {1, 0, 0},
                {3, 2, 1}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 2, consecutive, res);
            AssertSubmatrix(2, 0, 0, 1, 2, res);
        }

        // Проверка прерывания очередности в середине сканирования.
        TEST_METHOD(BreakInMiddle) {
            std::vector<std::vector<int>> matrix = {
                {5, 5, 5, 5},
                {5, 0, 0, 0},
                {5, 5, 5, 5}
            };
            std::vector<std::vector<int>> consecutive = {
                {4, 3, 2, 1},
                {1, 0, 0, 0},
                {4, 3, 2, 1}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 3, consecutive, res);
            AssertSubmatrix(4, 0, 0, 4, 1, res);
        }

        // Проверка, что максимальная площадь достигается в самом конце матрицы.
        TEST_METHOD(MaxAreaAtTheBottom) {
            std::vector<std::vector<int>> matrix = {
                {5, 5},
                {5, 5},
                {5, 5}
            };
            std::vector<std::vector<int>> consecutive = {
                {2, 1},
                {2, 1},
                {2, 1}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 3, consecutive, res);
            AssertSubmatrix(6, 0, 0, 2, 3, res);
        }

        // Проверка корректной работы при граничных значениях элементов. 
        TEST_METHOD(ExtremeValuesRange) {
            std::vector<std::vector<int>> matrix = {
                {1000, 1000},
                {-1000, -1000}
            };
            std::vector<std::vector<int>> consecutive = {
                {2, 1},
                {2, 1}
            };

            SubmatrixResult res_first;
            scanDownwards(matrix, 0, 0, 2, consecutive, res_first);
            AssertSubmatrix(2, 0, 0, 2, 1, res_first);

            SubmatrixResult res_second;
            scanDownwards(matrix, 1, 0, 2, consecutive, res_second);
            AssertSubmatrix(2, 1, 0, 2, 1, res_second);
        }

        // Проверка граничного размера матрицы 100х100
        TEST_METHOD(MaxMatrixDimensions) {
            int max_dim = 100;
            std::vector<std::vector<int>> matrix(max_dim, std::vector<int>(max_dim, 1));
            std::vector<std::vector<int>> consecutive(max_dim, std::vector<int>(max_dim));

            for (int i = 0; i < max_dim; i++) {
                for (int j = 0; j < max_dim; j++) {
                    consecutive[i][j] = max_dim - j;
                }
            }
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, max_dim, consecutive, res);
            AssertSubmatrix(10000, 0, 0, 100, 100, res);
        }

        // Проверка совпадения площадей при изменении геометрии.
        TEST_METHOD(SameAreaDifferentShapes) {
            std::vector<std::vector<int>> matrix = {
                {5, 5, 5, 5, 5, 5},
                {5, 5, 5, 0, 0, 0},
                {5, 5, 0, 0, 0, 0}
            };
            std::vector<std::vector<int>> consecutive = {
                {6, 5, 4, 3, 2, 1},
                {3, 2, 1, 0, 0, 0},
                {2, 1, 0, 0, 0, 0}
            };
            SubmatrixResult res;
            scanDownwards(matrix, 0, 0, 3, consecutive, res);
            AssertSubmatrix(6, 0, 0, 6, 1, res);
        }
    };
}