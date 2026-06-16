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
    * \param[in] string_token строковое представление токена, который нужно проверить
    * \param[in] parsed_value переменная, в которую будет записано числовое значение, если валидация прошла успешно
    * \param[in] error_elements множество, в которое будет добавлено строковое представление токена, если валидация не прошла
    * \return true или false в зависимости от того, прошло ли значение валидацию
    */
    bool validateValue(const std::string& string_token, int& parsed_value, std::set<std::string>& error_elements);


    /*!
     * \param[in] filename имя файла, из которого нужно прочитать матрицу
     * \param[in] cols переменная, в которую будет записано количество столбцов
     * \param[in] rows переменная, в которую будет записано количество строк
     * \param[in] matrix переменная, в которую будет записана считанная матрица
     */
    void readMatrix(const std::string& filename, int& cols, int& rows, std::vector<std::vector<int>>& matrix);

    /*!
    * \param[in] input_file открытие файлового потока, указывающий на начало данных матрицы
    * \param[in] cols ожидаемое количество столбцов в каждой строке
    * \param[in] rows ожидаемое общее количество строк
    * \param[in] matrix ссылка на двумерный вектор, в который будут записаны валидные данные
    */
    void readMatrixRows(std::ifstream& input_file, int cols, int rows, std::vector<std::vector<int>>& matrix);

    /*!
    * \param[in] line строка, содержащая размерности матрицы
    * \param[in] cols переменная, в которую будет записано количество столбцов
    * \param[in] rows переменная, в которую будет записано количество строк
    */
    void parseDimensions(const std::string& line, int& cols, int& rows);


    /*! Метод вывода результата в файл
    * \param[in] filename имя файла, в который нужно записать результат
    * \param[in] matrix матрица, из которой берется подматрица
    * \param[in] res ссылка на структуру с параметрами максимальной подматрицы для вывода
    */
    void writeResult(const std::string& filename, const std::vector<std::vector<int>>& matrix, const SubmatrixResult& res);
};