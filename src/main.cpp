#include <iostream>
#include <type_traits>
#include <windows.h>
#include "ChessPiece.h"

/**
 * @brief Показывает начальную информацию о модели
 *
 * @return 0 если программа завершилась успешно
 */
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Лабораторная работа №4. Вариант 4\n";
    std::cout << "Шахматные фигуры\n\n";
    std::cout << "Базовый класс абстрактный: "
              << (std::is_abstract<ChessPiece>::value ? "да" : "нет") << '\n';
    std::cout << "Размер доски: 8 на 8\n";
    std::cout << "Белых фигур: " << ChessPiece::getWhiteCount() << '\n';
    std::cout << "Черных фигур: " << ChessPiece::getBlackCount() << '\n';
    std::cout << "Конкретные фигуры будут добавлены в следующих частях\n";
    return 0;
}
