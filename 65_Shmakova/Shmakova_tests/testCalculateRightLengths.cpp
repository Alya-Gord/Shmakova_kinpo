#include "pch.h"
#include "CppUnitTest.h"
#include "Functions.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testCalculateRightLengths)
    {
    public:
        // Проверяем, корректность накопления длины.
        TEST_METHOD(AllIdenticalElements) {
            std::vector<std::vector<int>> matrix = { {7, 7, 7, 7} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(4, 0));

            calculateRightLengths(matrix, 1, 4, consecutive);

            Assert::AreEqual(4, consecutive[0][0]);
            Assert::AreEqual(3, consecutive[0][1]);
            Assert::AreEqual(2, consecutive[0][2]);
            Assert::AreEqual(1, consecutive[0][3]);
        }

        // Проверяем отсутствие ложных срабатываний, когда повторений нет вообще.
        TEST_METHOD(AllDifferentElements) {
            std::vector<std::vector<int>> matrix = { {1, 2, 3, 4} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(4, 0));

            calculateRightLengths(matrix, 1, 4, consecutive);

            for (int i = 0; i < 4; i++) {
                Assert::AreEqual(1, consecutive[0][i]);
            }
        }

        // Проверяем, что цепочка в конце строки корректно считается, а одиночные элементы не повлияют.
        TEST_METHOD(ChainAtTheEnd) {
            std::vector<std::vector<int>> matrix = { {3, 1, 2, 3, 3, 3} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(6, 0));

            calculateRightLengths(matrix, 1, 6, consecutive);

            Assert::AreEqual(1, consecutive[0][0]);
            Assert::AreEqual(1, consecutive[0][1]);
            Assert::AreEqual(1, consecutive[0][2]);
            Assert::AreEqual(3, consecutive[0][3]);
            Assert::AreEqual(2, consecutive[0][4]);
            Assert::AreEqual(1, consecutive[0][5]);
        }

        // Проверяем, что цепочка в начале строки корректно считается, а одиночные элементы не повлияют.
        TEST_METHOD(ChainAtTheStart) {
            std::vector<std::vector<int>> matrix = { {5, 5, 5, 9, 8} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(5, 0));

            calculateRightLengths(matrix, 1, 5, consecutive);

            Assert::AreEqual(3, consecutive[0][0]);
            Assert::AreEqual(2, consecutive[0][1]);
            Assert::AreEqual(1, consecutive[0][2]);
            Assert::AreEqual(1, consecutive[0][3]);
            Assert::AreEqual(1, consecutive[0][4]);
        }

        // Проверяем, что цепочка в середине строки корректно считается, а одиночные элементы не повлияют.
        TEST_METHOD(ChainInTheMiddle) {
            std::vector<std::vector<int>> matrix = { {4, 2, 5, 5, 5, 9, 8} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(7, 0));

            calculateRightLengths(matrix, 1, 7, consecutive);

            Assert::AreEqual(1, consecutive[0][0]);
            Assert::AreEqual(1, consecutive[0][1]);
            Assert::AreEqual(3, consecutive[0][2]);
            Assert::AreEqual(2, consecutive[0][3]);
            Assert::AreEqual(1, consecutive[0][4]);
            Assert::AreEqual(1, consecutive[0][5]);
            Assert::AreEqual(1, consecutive[0][6]);
        }

        // Проверяем, что чередующиеся элементы корректно считаются.
        TEST_METHOD(AlternatingElements) {
            std::vector<std::vector<int>> matrix = { {1, 0, 1, 0, 1} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(5, 0));

            calculateRightLengths(matrix, 1, 5, consecutive);

            for (int i = 0; i < 5; i++) {
                Assert::AreEqual(1, consecutive[0][i]);
            }
        }

        // Проверяем, что функция корректно обрабатывает единичную матрицу и не выходит за границы массива.
        TEST_METHOD(SingleElementMatrix) {
            std::vector<std::vector<int>> matrix = { {4} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(1, 0));

            calculateRightLengths(matrix, 1, 1, consecutive);

            Assert::AreEqual(1, consecutive[0][0]);
        }

        // Проверяем, что функия корректно считает при вектор-столбце.
        TEST_METHOD(SingleColumnMultipleRows) {
            std::vector<std::vector<int>> matrix = { {10}, {20}, {10} };
            std::vector<std::vector<int>> consecutive(3, std::vector<int>(1, 0));

            calculateRightLengths(matrix, 3, 1, consecutive);

            Assert::AreEqual(1, consecutive[0][0]);
            Assert::AreEqual(1, consecutive[1][0]);
            Assert::AreEqual(1, consecutive[2][0]);
        }

        // Проверка на пустые данные.
        TEST_METHOD(EmptyMatrixDoesNotCrash) {
            std::vector<std::vector<int>> matrix = {};
            std::vector<std::vector<int>> consecutive = {};

            calculateRightLengths(matrix, 0, 0, consecutive);

            Assert::IsTrue(consecutive.empty());
        }

        // Проверяем независимость строк друг от друга, корректное накопление длин.
        TEST_METHOD(ChainBreaksOnNewLine) {
            std::vector<std::vector<int>> matrix = {
                {1, 1, 1},
                {5, 6, 7}
            };
            std::vector<std::vector<int>> consecutive(2, std::vector<int>(3, 0));

            calculateRightLengths(matrix, 2, 3, consecutive);

            Assert::AreEqual(3, consecutive[0][0]);
            Assert::AreEqual(2, consecutive[0][1]);
            Assert::AreEqual(1, consecutive[0][2]);
            Assert::AreEqual(1, consecutive[1][0]);
            Assert::AreEqual(1, consecutive[1][1]);
            Assert::AreEqual(1, consecutive[1][2]);
        }

        // Проверяем корректную обработку чередующихся паттернов.
        TEST_METHOD(RowsWithAlternatingData) {
            std::vector<std::vector<int>> matrix = {
                {5, 5, 1, 1},
                {8, 8, 8, 2}
            };
            std::vector<std::vector<int>> consecutive(2, std::vector<int>(4, 0));

            calculateRightLengths(matrix, 2, 4, consecutive);

            Assert::AreEqual(2, consecutive[0][0]);
            Assert::AreEqual(1, consecutive[0][1]);
            Assert::AreEqual(2, consecutive[0][2]);
            Assert::AreEqual(1, consecutive[0][3]);

            Assert::AreEqual(3, consecutive[1][0]);
            Assert::AreEqual(2, consecutive[1][1]);
            Assert::AreEqual(1, consecutive[1][2]);
            Assert::AreEqual(1, consecutive[1][3]);
        }

		// Проверяем, что функция корректно работает с идентичными строками, и не путает цепочки.
        TEST_METHOD(IdenticalRowsCheck) {
            std::vector<std::vector<int>> matrix = {
                {3, 3, 3},
                {3, 3, 3}
            };
            std::vector<std::vector<int>> consecutive(2, std::vector<int>(3, 0));

            calculateRightLengths(matrix, 2, 3, consecutive);

            for (int i = 0; i < 2; i++) {
                Assert::AreEqual(3, consecutive[i][0]);
                Assert::AreEqual(2, consecutive[i][1]);
                Assert::AreEqual(1, consecutive[i][2]);
            }
        }

		// Проверяем, что функция корректно работает с отрицательными числами и нулями.
        TEST_METHOD(NegativeAndZeroElements) {
            std::vector<std::vector<int>> matrix = { {-5, -5, 0, 0, 0} };
            std::vector<std::vector<int>> consecutive(1, std::vector<int>(5, 0));

            calculateRightLengths(matrix, 1, 5, consecutive);

            Assert::AreEqual(2, consecutive[0][0]);
            Assert::AreEqual(1, consecutive[0][1]);
            Assert::AreEqual(3, consecutive[0][2]);
            Assert::AreEqual(2, consecutive[0][3]);
            Assert::AreEqual(1, consecutive[0][4]);
        }

		// Проверяем корректное затирание старых данных.
        TEST_METHOD(OverwritesPreExistingData) {
            std::vector<std::vector<int>> matrix = { {2, 2, 3} };

            // Инициализируем "мусором" (например, девятками)
            std::vector<std::vector<int>> consecutive = { {99, 99, 99} };

            calculateRightLengths(matrix, 1, 3, consecutive);

            Assert::AreEqual(2, consecutive[0][0]);
            Assert::AreEqual(1, consecutive[0][1]);
            Assert::AreEqual(1, consecutive[0][2]);
        }
    };
}