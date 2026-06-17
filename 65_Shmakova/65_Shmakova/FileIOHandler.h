#pragma once
#include "Structures.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <filesystem>


class FileIOHandler {
public:
    /*! Метод проверки расширения файла на соответствие заданному множеству расширений
    * \param[in] filename путь файла, расширение которого нужно проверить
    * \param[in] extensions множество допустимых расширений
    * \return true или false в зависимости от того, соответствует ли расширение файла одному из допустимых
    */
    bool checkExtension(const std::filesystem::path& filename, const std::unordered_set<std::string>& extensions) const;


    /*! Метод валидации полученных значений
    * \param[in] token строковое представление токена, который нужно проверить
    * \param[out] value переменная, в которую будет записано числовое значение
    * \param[out] float_errors множество для накопления ошибок вещественных чисел
    * \param[out] invalid_char_errors множество для накопления ошибок символов
    * \param[out] out_of_range_errors вектор для накопления ошибок выхода за диапазон
    * \return true, если валидация прошла успешно, иначе false
    */
    bool validateValue(const std::string& token, int& value, std::set<std::string>& float_errors,
        std::set<std::string>& invalid_char_errors, std::vector<int>& out_of_range_errors);

    /*! Метод чтения содержимого текстового файла
    * \param[in] filename путь к файлу, который необходимо прочитать
    * \return вектор строк, где каждый элемент соответствует одной строке файла
    */
    std::vector<std::string> readText(const std::string& filename) const;


    /*! Метод парсинга строки для получения размерностей матрицы
    * \param[in] line строка, содержащая размеры
    * \param[out] cols переменная, в которую будет записано количество столбцов
    * \param[out] rows переменная, в которую будет записано количество строк
    */
    void parseDimensions(const std::string& line, int& cols, int& rows) const;


    /*! Метод чтения размерностей и данных матрицы из списка строк
    * \param[in] lines вектор строк, содержащий данные файла
    * \param[out] cols переменная, в которую будет записано количество столбцов
    * \param[out] rows переменная, в которую будет записано количество строк
    * \param[out] matrix матрица, в которую будут записаны считанные данные
    */
    void readDimensionsAndMatrix(const std::vector<std::string>& lines, int& cols, int& rows, std::vector<std::vector<int>>& matrix);


    /*! Метод вывода результата в файл
    * \param[in] filename имя файла, в который нужно записать результат
    * \param[in] matrix матрица, из которой берется подматрица
    * \param[in] res ссылка на структуру с параметрами максимальной подматрицы для вывода
    */
    void writeResult(const std::string& filename, const std::vector<std::vector<int>>& matrix, const SubmatrixResult& res);
};