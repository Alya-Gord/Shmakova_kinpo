#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testValidateValue)
    {
    public:
		// Проверяем, что валидные целые числа корректно парсятся и не добавляются в множество ошибок.
        TEST_METHOD(ValidPositiveInteger) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsTrue(handler.validateValue("52", val, errs));
            Assert::AreEqual(52, val);
            Assert::IsTrue(errs.empty());
        }

		// Проверяем, что валидные отрицательные целые числа корректно парсятся и не добавляются в множество ошибок.
        TEST_METHOD(ValidNegativeInteger) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsTrue(handler.validateValue("-52", val, errs));
            Assert::AreEqual(-52, val);
        }

		// Проверяем, что валидное нулевое значение корректно парсится и не добавляется в множество ошибок.
        TEST_METHOD(ValidZero) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsTrue(handler.validateValue("0", val, errs));
            Assert::AreEqual(0, val);
        }

		// Проверяем, что граничные значения -1000 и 1000 корректно парсятся и не добавляются в множество ошибок.
        TEST_METHOD(BoundaryValuesAccepted) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsTrue(handler.validateValue("1000", val, errs));
            Assert::IsTrue(handler.validateValue("-1000", val, errs));
        }

		// Проверяем, что значения, выходящие за границы допустимого диапазона, не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(OutOfBoundsRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("1001", val, errs));
            Assert::IsTrue(errs.find("1001") != errs.end());

            errs.clear();
            Assert::IsFalse(handler.validateValue("-1001", val, errs));
            Assert::IsTrue(errs.find("-1001") != errs.end());
        }

		// Проверяем, что числа с точкой не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(FloatsWithDotRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("3.14", val, errs));
            Assert::IsTrue(errs.find("3.14") != errs.end());
        }

		// Проверяем, что числа с запятой не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(FloatsWithCommaRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("5,5", val, errs));
            Assert::IsTrue(errs.find("5,5") != errs.end());
        }

		// Проверяем, что строки, не являющиеся числами, не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(GarbageLettersRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("abc", val, errs));
        }

		// Проверяем, что строки, содержащие числа с буквами, не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(PartialGarbageRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("12a", val, errs));
        }
        
		// Проверяем, что пустые строки не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(EmptyStringRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("", val, errs));
        }

		// Проверяем, что строки, выходящие за пределы диапазона int, не проходят валидацию и добавляются в множество ошибок.
        TEST_METHOD(IntOverflowRejected) {
            FileIOHandler handler;
            std::set<std::string> errs;
            int val;
            Assert::IsFalse(handler.validateValue("999999999999999999", val, errs));
            Assert::IsTrue(errs.find("999999999999999999") != errs.end());
        }
    };
}