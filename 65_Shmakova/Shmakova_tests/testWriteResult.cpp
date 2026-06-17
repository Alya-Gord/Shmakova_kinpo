#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testWriteResult)
    {
    public:
        // Проверка, что при корректных данных результат записывается в файл без выбрасывания исключений.
        TEST_METHOD(CorrectDataNotThrowException) {
            FileIOHandler handler;
			bool correctWrite = false;
            std::string filename = "output.txt";
            std::vector<std::vector<int>> matrix = {
                {1, 2},
                {3, 4}
            };
            SubmatrixResult res = { 0, 0, 1, 1, 1 };

            try {
                handler.writeResult(filename, matrix, res);

                std::ifstream fileCheck(filename);
                correctWrite = fileCheck.good();
                fileCheck.close();
                std::remove(filename.c_str());
            }
            catch (const Error& e) {
                std::remove(filename.c_str());
                correctWrite = false;
            }
			Assert::IsTrue(correctWrite);
        }

        // Проверка, что при попытке записать результат в файл с недопустимым расширением выбрасывается исключение.
        TEST_METHOD(ThrowExtensionException) {
            FileIOHandler handler;
			bool incorrectWrite = false;
            std::string filename = "output.csv";
            std::vector<std::vector<int>> matrix = {
                {1, 2},
                {3, 4}
            };
            SubmatrixResult res = { 0, 0, 1, 1, 1 };

            try {
                handler.writeResult(filename, matrix, res);
                incorrectWrite = false;
            }
            catch (const Error& e) {
                incorrectWrite = (e.type == INVALID_EXTENSION);
            }
            Assert::IsTrue(incorrectWrite);
        }

        // Проверка, что при попытке записать результат в недопустимый файл выбрасывается исключение.
        TEST_METHOD(ThrowExceptionOutputFile) {
            FileIOHandler handler;
			bool incorrectWrite = false;
            std::string filename = "output?.txt";
            std::vector<std::vector<int>> matrix = {
                {1, 2},
                {3, 4}
            };
            SubmatrixResult res = { 0, 0, 1, 1, 1 };

            try {
                handler.writeResult(filename, matrix, res);
                std::remove(filename.c_str());
                incorrectWrite = false;
            }
            catch (const Error& e) {
                incorrectWrite = (e.type == OUTPUT_FILE_ERROR);
            }
            Assert::IsTrue(incorrectWrite);
        }
    };
}