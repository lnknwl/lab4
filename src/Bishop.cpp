#include <iostream>
#include "Bishop.h"

/**
 * @brief Создает слона заданного цвета и позиции
 */
Bishop::Bishop(Color color, int x, int y)
    : SlidingPiece(color, x, y)
{
}

/**
 * @brief Проверяет ход слона по диагонали
 */
bool Bishop::canMoveTo(int newX, int newY) const
{
    return isDiagonalMove(newX, newY);
}

/**
 * @brief Возвращает обозначение слона с учетом цвета
 */
char Bishop::getSymbol() const
{
    return getColor() == Color::White ? 'B' : 'b';
}

/**
 * @brief Выводит название и данные слона
 */
void Bishop::printInfo() const
{
    std::cout << "Слон: ";
    ChessPiece::printInfo();
}
