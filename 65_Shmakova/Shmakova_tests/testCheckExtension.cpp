#include "pch.h"
#include "CppUnitTest.h"
#include "FileIOHandler.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Shmakova_tests
{
    TEST_CLASS(testCheckExtension)
    {
    public:
		// Проверка на корректное распознавание допустимого расширения.
        TEST_METHOD(ValidExtension) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".txt" };
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("matrix.txt"), exts));
        }

		// Проверка на корректную обработку полного пути к файлу.
        TEST_METHOD(ValidExtensionWithPath) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".csv" };
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("C:/data/files/matrix.csv"), exts));
        }

		// Проверка на чувствительность к регистру.
        TEST_METHOD(CaseInsensitivity) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".txt" };
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("FILE.TXT"), exts));
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("file.Txt"), exts));
        }

        // Проверка файла без раширения.
        TEST_METHOD(NoExtension) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".txt" };
            Assert::IsFalse(handler.checkExtension(std::filesystem::path("matrix"), exts));
        }

		// Проверка, что файл с точкой, но без расширения, не распознается как имеющий допустимое расширение.
        TEST_METHOD(DotButEmptyExtension) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".txt" };
            Assert::IsFalse(handler.checkExtension(std::filesystem::path("matrix."), exts));
        }

		// Проверка на несколько расширений в множестве.
        TEST_METHOD(SetContainsMultipleExtensions) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".txt", ".jpg", ".png", ".pdf", ".zip" };
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("data.txt"), exts));
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("image.jpg"), exts));
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("archive.zip"), exts));
        }

        // Проверка расширений с разной длиной.
        TEST_METHOD(DifferentLengthExtensions) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".c", ".html", ".jpeg" };
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("main.c"), exts));
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("picture.jpeg"), exts));
        }

        // Проверка на составное расширение.
        TEST_METHOD(MultipleDotsInFilename) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".gz" };
            Assert::IsTrue(handler.checkExtension(std::filesystem::path("backup.tar.gz"), exts));
        }

		// Проверка на частичное совпадение расширения.
        TEST_METHOD(PartialExtensionMismatch) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".jpg" };
            Assert::IsFalse(handler.checkExtension(std::filesystem::path("image.jpeg"), exts));
        }

		// Проверка, что наличие точки в пути, не приводит к ошибочному распознаванию расширения.
        TEST_METHOD(PathContainsDotButNoExtension) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".txt" };
            Assert::IsFalse(handler.checkExtension(std::filesystem::path("/projects/ver.1.0/README"), exts));
        }

		// Проверка на скрытые файлы, которые начинаются с точки, но не имеют расширения.
        TEST_METHOD(HiddenFiles) {
            FileIOHandler handler;
            std::unordered_set<std::string> exts = { ".gitignore" };
            Assert::IsFalse(handler.checkExtension(std::filesystem::path(".gitignore"), exts));
        }

        // Проверка на пустые входные данные. 
        TEST_METHOD(EmptyInputs) {
            FileIOHandler handler;

            std::unordered_set<std::string> exts = { ".txt" };
            Assert::IsFalse(handler.checkExtension(std::filesystem::path(""), exts));

            std::unordered_set<std::string> empty_exts = {};
            Assert::IsFalse(handler.checkExtension(std::filesystem::path("file.txt"), empty_exts));
        }
    };
}