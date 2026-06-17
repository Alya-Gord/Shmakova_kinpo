#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testReadDimensionsAndMatrix)
    {
    public:
        
		// Проверяем, что функция корректно парсит валидные строки с размерностями и данными матрицы, записывая их в переданные переменные.
        TEST_METHOD(ValidMatrixParsing) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 2"}, 
                {"1 2"}, 
                {"3 4"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            std::vector<std::vector<int>> exp = { 
                {1, 2}, 
                {3, 4} 
            };

            handler.readDimensionsAndMatrix(text, cols, rows, matrix);

            Assert::AreEqual(2, cols);
            Assert::AreEqual(2, rows);
            Assert::IsTrue(matrix == exp);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче пустого вектора строк.
        TEST_METHOD(EmptyTextVector) {
            FileIOHandler handler;
            std::vector<std::vector<int>> matrix;
            std::vector<std::string> text = {};
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == NO_INPUT_DATA); 
            }
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче вектора строк, содержащего только строку с размерностями, но без данных матрицы.
        TEST_METHOD(MatrixMissingBody) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 2"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == MATRIX_MISSING); 
            }
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с количеством элементов, не совпадающим с заявленным количеством столбцов в одной строке.
        TEST_METHOD(RowFormatMismatch) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 2"}, 
                {"1 2 3"}, 
                {"4 5"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == ROW_FORMAT_ERROR); 
            }
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с количеством элементов, не совпадающим с заявленным количеством строк и столбцов.
        TEST_METHOD(RowCountMismatch) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 3"}, 
                {"1 2"}, 
                {"3 4"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == DIMENSION_MISMATCH);
            }
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с недопустимыми символами.
        TEST_METHOD(InvalidCharInMatrix) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 2"}, 
                {"1 text"}, 
                {"3 4"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == INVALID_CHARACTER); 
            }
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с числом в формате с плавающей точкой.
        TEST_METHOD(FloatInMatrix) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 2"}, 
                {"1 2.5"}, 
                {"3 4"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == FLOAT_NUMBER); 
            }
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с числом, выходящим за допустимый диапазон.
        TEST_METHOD(OutOfRangeInMatrix) {
            FileIOHandler handler;
            std::vector<std::string> text = { 
                {"2 2"}, 
                {"1 2"}, 
                {"3000 4"} 
            };
            std::vector<std::vector<int>> matrix;
            int cols = 0, rows = 0;
            try { 
                handler.readDimensionsAndMatrix(text, cols, rows, matrix); Assert::Fail(); 
            }
            catch (const Error& e) { 
                Assert::IsTrue(e.type == OUT_OF_RANGE); 
            }
        }
    };
}