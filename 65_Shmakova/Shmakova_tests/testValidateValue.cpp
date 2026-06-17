#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testValidateValue)
    {
    public:
        
        TEST_METHOD(ValidPositiveValue) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsTrue(handler.validateValue("52", val, floats, chars, ranges));
            Assert::AreEqual(52, val);
            Assert::IsTrue(floats.empty() && chars.empty() && ranges.empty());
        }

        TEST_METHOD(ValidZeroAndNegativeValues) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsTrue(handler.validateValue("0", val, floats, chars, ranges));
            Assert::IsTrue(handler.validateValue("-999", val, floats, chars, ranges));
        }

        TEST_METHOD(ValidBoundaryValues) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsTrue(handler.validateValue("1000", val, floats, chars, ranges));
            Assert::IsTrue(handler.validateValue("-1000", val, floats, chars, ranges));
        }

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

        TEST_METHOD(GarbageLetters) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("abc", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("abc") != chars.end()); 
        }

        TEST_METHOD(PartialGarbage) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("12px", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("12px") != chars.end());
        }

        TEST_METHOD(ExtremeLargeNumberOverflow) {
            FileIOHandler handler;
            std::set<std::string> floats, chars; 
            std::vector<int> ranges; 
            int val;
            Assert::IsFalse(handler.validateValue("999999999999999999", val, floats, chars, ranges));
            Assert::IsTrue(chars.find("999999999999999999") != chars.end());
        }

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