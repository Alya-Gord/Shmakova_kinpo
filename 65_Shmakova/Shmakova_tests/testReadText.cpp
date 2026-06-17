#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"
#include <fstream>
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testReadText)
    {
    public:
		// Проверяем, что функция корректно читает строки из файла и возвращает их в виде вектора строк.
        TEST_METHOD(CheckReadAndWrite) {
            FileIOHandler handler;
            std::string filename = "input.txt";

            std::ofstream out(filename);
            out << "2 2\n";
            out << "1 2\n";
            out << "3 4\n";
            out.close();

            std::vector<std::string> lines = handler.readText(filename);

            Assert::AreEqual((size_t)3, lines.size());

            Assert::AreEqual(std::string("2 2"), lines[0]);
            Assert::AreEqual(std::string("1 2"), lines[1]);
            Assert::AreEqual(std::string("3 4"), lines[2]);

            std::remove(filename.c_str());
        }

		// Проверяем, что функция корректно фильтрует пустые строки и строки, состоящие только из пробелов.
        TEST_METHOD(CheckDataFiltering) {
            FileIOHandler handler;
            std::string filename = "input.txt";

            std::ofstream out(filename);
            out << "2 2\n\n  \n1 2\n3 4\n";
            out.close();

            std::vector<std::string> lines = handler.readText(filename);

            Assert::AreEqual((size_t)3, lines.size());

            Assert::AreEqual(std::string("2 2"), lines[0]);
            Assert::AreEqual(std::string("1 2"), lines[1]);
            Assert::AreEqual(std::string("3 4"), lines[2]);

            std::remove(filename.c_str());
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при попытке прочитать файл с недопустимым расширением.
        TEST_METHOD(InvalidExtension) {
            FileIOHandler handler;
            bool isCorrect = false;

            try { 
                handler.readText("input.bin");
                isCorrect = true;
            }
            catch (const Error& e) { 
                isCorrect = (e.type == INVALID_EXTENSION);
            }
            Assert::IsTrue(isCorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при попытке прочитать несуществующий файл.
        TEST_METHOD(FileNotFound) {
            FileIOHandler handler;
			bool isCorrect = false;

            try { 
				handler.readText("input.txt");
				isCorrect = true;
            }
            catch (const Error& e) { 
                isCorrect = (e.type == FILE_NOT_FOUND); 
            }
            Assert::IsTrue(isCorrect);
        }

		// Проверяем, что функция выбрасывает исключение с правильным типом ошибки при попытке прочитать файл, который существует, но не содержит полезных данных.
        TEST_METHOD(EmptyFile) {
            FileIOHandler handler;
			bool isCorrect = false;
            std::string filename = "input.txt";
            std::ofstream out(filename); 
            out.close();

            try {
                handler.readText(filename);
                std::remove(filename.c_str());
				isCorrect = true;
            }
            catch (const Error& e) {
                std::remove(filename.c_str());
                isCorrect = (e.type == NO_INPUT_DATA);
            }
            Assert::IsTrue(isCorrect);
        }
    };
}