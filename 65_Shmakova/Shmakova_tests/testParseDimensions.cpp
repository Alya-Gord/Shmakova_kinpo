#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testParseDimensions)
    {
    public:
        
		// Проверяем, что функция корректно парсит валидные строки с размерностями и записывает их в переданные переменные.
        TEST_METHOD(ValidDimensionsNormal) {
            FileIOHandler handler; 
            int cols = 0, rows = 0;
            handler.parseDimensions("5 10", cols, rows);
            Assert::AreEqual(5, cols); 
            Assert::AreEqual(10, rows);
        }

		// Проверяем, что функция корректно обрабатывает граничные значения допустимых размерностей.
        TEST_METHOD(ValidDimensionsMax) {
            FileIOHandler handler; 
            int cols = 0, rows = 0;
            handler.parseDimensions("100 100", cols, rows);
            Assert::AreEqual(100, cols); 
            Assert::AreEqual(100, rows);
        }

		// Проверяем, что функция корректно обрабатывает минимальные допустимые размерности.
        TEST_METHOD(ValidDimensionsMin) {
            FileIOHandler handler; 
            int cols = 0, rows = 0;
            handler.parseDimensions("1 1", cols, rows); 
            Assert::AreEqual(1, cols); 
            Assert::AreEqual(1, rows);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с тремя числами вместо двух.
        TEST_METHOD(ThreeInputsError) {
            FileIOHandler handler;
			bool incorrect = false;
            int cols = 0, rows = 0;
            try { 
                handler.parseDimensions("10 20 30", cols, rows);
                incorrect = false;
            }
            catch (const Error& e) { 
                incorrect = (e.type == DIMENSION_TOO_MANY_NUMBERS);
            }
            Assert::IsTrue(incorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с одним числом вместо двух.
        TEST_METHOD(OneInputError) {
            FileIOHandler handler;
			bool incorrect = false;
            int cols = 0, rows = 0;
            try { 
                handler.parseDimensions("10", cols, rows); 
                incorrect = false;
            }
            catch (const Error& e) { 
                incorrect = (e.type == DIMENSION_MISMATCH);
            }
            Assert::IsTrue(incorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с числом, выходящим за допустимый диапазон.
        TEST_METHOD(MoreThan100Error) {
            FileIOHandler handler; 
            int cols = 0, rows = 0;
			bool incorrect = false;
            try { 
                handler.parseDimensions("101 50", cols, rows); 
                incorrect = false;
            }
            catch (const Error& e) {
                incorrect = (e.type == OUT_OF_RANGE);
            }
            Assert::IsTrue(incorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с числом, выходящим за допустимый диапазон.
        TEST_METHOD(ZeroDimensionError) {
            FileIOHandler handler; 
			bool incorrect = false;
            int cols = 0, rows = 0;
            try { 
                handler.parseDimensions("0 10", cols, rows); 
                incorrect = false;
            }
           catch (const Error& e) { 
               incorrect = (e.type == DIMENSION_NEGATIVE); 
           }
            Assert::IsTrue(incorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с отрицательным числом.
        TEST_METHOD(NegativeDimensionError) {
            FileIOHandler handler;
			bool incorrect = false;
            int cols = 0, rows = 0;
            try { 
                handler.parseDimensions("10 -5", cols, rows); 
                incorrect = false;
            }
           catch (const Error& e) { 
               incorrect = (e.type == DIMENSION_NEGATIVE); 
           }
            Assert::IsTrue(incorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с вещественным числом вместо целого.
        TEST_METHOD(FloatDimensionError) {
            FileIOHandler handler;
			bool incorrect = false;
            int cols = 0, rows = 0;
            try { 
                handler.parseDimensions("5.5 10", cols, rows);
                incorrect = false;
            }
           catch (const Error& e) { 
               incorrect = (e.type == DIMENSION_NOT_INTEGER); 
           }
            Assert::IsTrue(incorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при передаче строки с нечисловыми символами.
        TEST_METHOD(GarbageDimensionError) {
            FileIOHandler handler;
			bool incorrect = false;
            int cols = 0, rows = 0;
            try { 
                handler.parseDimensions("abc 10", cols, rows); 
                incorrect = false;
            }
           catch (const Error& e) { 
               incorrect = (e.type == DIMENSION_NOT_INTEGER); 
           }
            Assert::IsTrue(incorrect);
        }
    };
}