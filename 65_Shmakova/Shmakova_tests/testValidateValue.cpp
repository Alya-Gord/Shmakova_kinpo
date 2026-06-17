#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testValidateValue)
    {
    public:
        
		// Проверяем, что функция корректно обрабатывает валидные положительные целые числа.
        TEST_METHOD(ValidPositiveValue) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsTrue(handler.validateValue("52", val, floats, chars, ranges));
            Assert::AreEqual(52, val);
            Assert::IsTrue(floats.empty() && chars.empty() && ranges.empty());
        }

		//  Проверяем, что функция корректно обрабатывает валидные нулевые и отрицательные целые числа.
        TEST_METHOD(ValidZeroAndNegativeValues) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsTrue(handler.validateValue("0", val, floats, chars, ranges));
            Assert::IsTrue(handler.validateValue("-999", val, floats, chars, ranges));
        }

		// Проверяем, что функция корректно обрабатывает граничные значения допустимого диапазона.
        TEST_METHOD(ValidBoundaryValues) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsTrue(handler.validateValue("1000", val, floats, chars, ranges));
            Assert::IsTrue(handler.validateValue("-1000", val, floats, chars, ranges));
        }

		// Проверяем, что функция корректно обрабатывает числа, выходящие за допустимый диапазон.
        TEST_METHOD(OutOfBounds) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("1001", val, floats, chars, ranges));
            Assert::AreEqual(1001, ranges[0]);

            ranges.clear();
            Assert::IsFalse(handler.validateValue("-1001", val, floats, chars, ranges));
            Assert::AreEqual(-1001, ranges[0]);
        }

		// Проверяем, что функция корректно обрабатывает вещественные числа и добавляет их в множество ошибок вещественных чисел.
        TEST_METHOD(FloatNumbersRejected) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("3.14", val, floats, chars, ranges));
            Assert::IsTrue(floats.find("3.14") != floats.end()); 

            floats.clear();
            Assert::IsFalse(handler.validateValue("5,5", val, floats, chars, ranges));
            Assert::IsTrue(floats.find("5,5") != floats.end());
        }

		// Проверяем, что функция корректно обрабатывает строки, не являющиеся числами, и добавляет их в множество ошибок недопустимых символов.
        TEST_METHOD(GarbageLetters) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("abc", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("abc") != chars.end()); 
        }

		// Проверяем, что функция корректно обрабатывает строки, содержащие число с мусором, и добавляет их в множество ошибок недопустимых символов.
        TEST_METHOD(PartialGarbage) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("12px", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("12px") != chars.end());
        }

		// Проверяем, что функция корректно обрабатывает очень большие числа, которые не помещаются в тип int, и добавляет их в множество ошибок недопустимых символов.
        TEST_METHOD(ExtremeLargeNumberOverflow) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("999999999999999999", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("999999999999999999") != chars.end());
        }

		// Проверяем, что функция корректно обрабатывает пустую строку и добавляет ее в множество ошибок недопустимых символов.
        TEST_METHOD(EmptyToken) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("") != chars.end());
        }
    };
}