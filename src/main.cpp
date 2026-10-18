#include <iostream>
#include <windows.h>
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Board.h"

/**
 * @brief Выполняет ход и выводит его результат
 *
 * @param board Доска
 * @param fromX Начальный столбец
 * @param fromY Начальная строка
 * @param x Столбец назначения
 * @param y Строка назначения
 */
void showMove(Board& board, int fromX, int fromY, int x, int y)
{
    std::cout << "Ход (" << fromX << ", " << fromY << ") -> (" << x << ", " << y << "): ";
    std::cout << (board.movePiece(fromX, fromY, x, y) ? "выполнен" : "отклонен") << '\n';
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
        Bishop bishop(Color::Black, 0, 3);
        Knight knight(Color::White, 1, 0);
        Queen queen(Color::Black, 3, 7);
        King whiteKing(Color::White, 4, 0);
        King blackKing(Color::Black, 4, 7);
        std::vector<ChessPiece*> pieces = {&rook, &bishop, &knight, &queen, &whiteKing, &blackKing};
        Board board(pieces);

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

        std::cout << "\nНачальная доска:\n";
        board.print();
        std::cout << "\nХоды на доске:\n";
        showMove(board, 0, 0, 2, 0);
        showMove(board, 0, 0, 1, 0);
        showMove(board, 1, 0, 2, 2);
        showMove(board, 0, 0, 0, 5);
        showMove(board, 0, 0, 0, 3);
        showMove(board, 4, 0, 4, 1);
        showMove(board, 4, 1, 4, 3);
        showMove(board, 3, 7, 3, 4);
        showMove(board, 3, 4, 8, 4);

        std::cout << "\nДоска после ходов:\n";
        board.print();
        std::cout << "Состояние доски: " << (board.isValid() ? "допустимое" : "неправильное") << '\n';
        {
            King secondWhiteKing(Color::White, 7, 0);
            std::vector<ChessPiece*> wrong = {&whiteKing, &blackKing, &secondWhiteKing};
            std::cout << "Доска с двумя белыми королями: "
                      << (ChessPiece::validateBoardState(wrong) ? "допустимая" : "отклонена") << '\n';
        }
    }

    std::cout << "\nПосле завершения: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    return 0;
}
