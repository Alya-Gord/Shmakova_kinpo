#include "pch.h"
#include "CppUnitTest.h"
#include "Functions.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testUpdateMaxMatrix)
    {
    public:
        // Вспомогательный метод для полной проверки всей структуры
        void AssertSubmatrix(int exp_area, int exp_row, int exp_col, int exp_w, int exp_h, const SubmatrixResult& res) {
            Assert::AreEqual(exp_area, res.max_area);
            Assert::AreEqual(exp_row, res.min_row);
            Assert::AreEqual(exp_col, res.min_col);
            Assert::AreEqual(exp_w, res.max_width);
            Assert::AreEqual(exp_h, res.max_height);
        }

        // Первичное заполнение структуры из нулевого состояния.
        TEST_METHOD(InitialState) {
            SubmatrixResult res;
            res.max_area = 0;
            res.min_row = 0;
            res.min_col = 0;
            res.max_width = 0;
            res.max_height = 0;
            updateMaxMatrix(4, 2, 3, 2, 2, res);
            AssertSubmatrix(4, 2, 3, 2, 2, res);
        }

        // Появление новой подматрицы, площадь которой строго больше текущего рекорда.
        TEST_METHOD(StrictlyGreaterArea) {
            SubmatrixResult res;
            res.max_area = 4;
            res.min_row = 1;
            res.min_col = 1;
            res.max_width = 2;
            res.max_height = 2;
            updateMaxMatrix(9, 0, 5, 3, 3, res);
            AssertSubmatrix(9, 0, 5, 3, 3, res);
        }

        // Появление новой подматрицы с меньшей площадью.
        TEST_METHOD(SmallerAreaIsIgnored) {
            SubmatrixResult res;
            res.max_area = 8;
            res.min_row = 2;
            res.min_col = 3;
            res.max_width = 4;
            res.max_height = 2;
            updateMaxMatrix(5, 0, 0, 5, 1, res);
            AssertSubmatrix(8, 2, 3, 4, 2, res);
        }

        // Появление новой подматрицы с точно такой же площадью.
        TEST_METHOD(EqualAreaDoesNotOverwrite) {
            SubmatrixResult res;
            res.max_area = 6;
            res.min_row = 1;
            res.min_col = 2;
            res.max_width = 3;
            res.max_height = 2;
            updateMaxMatrix(6, 4, 5, 2, 3, res);
            AssertSubmatrix(6, 1, 2, 3, 2, res);
        }
    };
}