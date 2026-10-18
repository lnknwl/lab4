#include <iostream>
#include <memory>
#include <stdexcept>
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
 * @brief Показывает независимость копии и результат присваивания
 * @tparam T Конкретный тип фигуры
 * @param x Столбец для перемещения копии
 * @param y Строка для перемещения копии
 */
template <typename T>
void showCopying(int x, int y)
{
    T original(Color::White, 3, 3);
    T copy(original);
    T assigned(Color::Black, 0, 0);
    std::cout << "\nФигура " << original.getSymbol() << ": оригинал и копия (3, 3)\n";
    std::cout << "До присваивания: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    assigned = original;
    std::cout << "После присваивания: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    copy.moveTo(x, y);
    std::cout << "Оригинал: (" << original.getX() << ", " << original.getY()
              << "), копия: (" << copy.getX() << ", " << copy.getY()
              << "), присвоенная фигура: (" << assigned.getX() << ", " << assigned.getY() << ")\n";
    const T& same = assigned;
    assigned = same;
    std::cout << "После самоприсваивания: " << assigned.getSymbol() << " ("
              << assigned.getX() << ", " << assigned.getY() << ")\n";
}

/**
 * @brief Показывает отказ конструктора при неправильной позиции
 */
void showInvalidPosition()
{
    std::cout << "\nСоздание фигуры с неправильной позицией:\n";
    try
    {
        Rook wrong(Color::White, 8, 0);
    }
    catch (const std::invalid_argument& error)
    {
        std::cout << "Ошибка: " << error.what() << '\n';
    }
    std::cout << "После ошибки: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
}

/**
 * @brief Показывает удаление ферзя через указатель на базовый класс
 */
void showPolymorphicDeletion()
{
    std::cout << "\nУдаление через базовый указатель:\n";
    std::unique_ptr<ChessPiece> piece(new Queen(Color::Black, 3, 3));
    piece->printInfo();
    std::cout << "До удаления: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    piece.reset();
    std::cout << "После удаления: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
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
        showMove(board, 0, 3, 2, 5);
        showMove(board, 2, 5, 2, 6);
        showMove(board, 2, 5, 0, 3);
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
            try
            {
                Board wrongBoard(wrong);
            }
            catch (const std::invalid_argument& error)
            {
                std::cout << "Ошибка создания доски: " << error.what() << '\n';
            }
        }
    }

    std::cout << "\nПосле удаления фигур доски: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    std::cout << "\nКопирование и присваивание отдельных фигур:\n";
    showCopying<Rook>(3, 6);
    showCopying<Bishop>(5, 5);
    showCopying<Knight>(5, 4);
    showCopying<Queen>(6, 3);
    showCopying<King>(4, 4);
    showInvalidPosition();
    showPolymorphicDeletion();

    std::cout << "\nПосле завершения: белых " << ChessPiece::getWhiteCount()
              << ", черных " << ChessPiece::getBlackCount() << '\n';
    return ChessPiece::getWhiteCount() == 0 && ChessPiece::getBlackCount() == 0 ? 0 : 1;
}
