#include <iostream>
#include <windows.h>
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"

/**
 * @brief Выполняет ход и выводит его результат
 *
 * @param piece Фигура
 * @param x Столбец назначения
 * @param y Строка назначения
 */
void showMove(ChessPiece& piece, int x, int y)
{
    std::cout << "Ход " << piece.getSymbol() << ": (" << piece.getX() << ", "
              << piece.getY() << ") -> (" << x << ", " << y << "): ";
    std::cout << (piece.moveTo(x, y) ? "выполнен" : "отклонен") << '\n';
}

/**
 * @brief Показывает фигуры и полиморфные вызовы ходов
 *
 * @return 0 если программа завершилась успешно
 */
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Лабораторная работа №4. Вариант 4\n";
    std::cout << "Шахматные фигуры\n\n";
    std::cout << "Размер доски: 8 на 8\n";

    {
        Rook rook(Color::White, 0, 0);
        Bishop bishop(Color::Black, 2, 0);
        Knight knight(Color::White, 1, 0);
        Queen queen(Color::Black, 3, 7);
        ChessPiece* pieces[] = {&rook, &bishop, &knight, &queen};

        std::cout << "\nНачальные фигуры:\n";
        for (const ChessPiece* piece : pieces)
        {
            piece->printInfo();
        }
        std::cout << "Белых фигур: " << ChessPiece::getWhiteCount() << '\n';
        std::cout << "Черных фигур: " << ChessPiece::getBlackCount() << '\n';

        CombinedPiece* combined = &queen;
        std::cout << "Ход ферзя из (3, 7) в (7, 3) через CombinedPiece: "
                  << (combined->canMoveCombined(7, 3) ? "возможен" : "невозможен") << '\n';

        std::cout << "\nХоды фигур:\n";
        showMove(*pieces[0], 0, 5);
        showMove(*pieces[0], 2, 6);
        showMove(*pieces[1], 5, 3);
        showMove(*pieces[1], 5, 4);
        showMove(*pieces[0], -1, 5);
        showMove(*pieces[2], 2, 2);
        showMove(*pieces[2], 2, 3);
        showMove(*pieces[2], 0, 1);
        showMove(*pieces[2], -1, 3);
        showMove(*pieces[3], 3, 4);
        showMove(*pieces[3], 7, 4);
        showMove(*pieces[3], 4, 1);
        showMove(*pieces[3], 5, 3);
        showMove(*pieces[3], 8, 5);

        std::cout << "\nПосле ходов:\n";
        for (const ChessPiece* piece : pieces)
        {
            piece->printInfo();
        }
    }

    std::cout << "\nПосле завершения: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    return 0;
}
