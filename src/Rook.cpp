#include <iostream>
#include "Rook.h"

/**
 * @brief Создает ладью заданного цвета и позиции
 */
Rook::Rook(Color color, int x, int y)
    : SlidingPiece(color, x, y)
{
}

/**
 * @brief Проверяет ход ладьи по горизонтали или вертикали
 */
bool Rook::canMoveTo(int newX, int newY) const
{
    return isStraightMove(newX, newY);
}

/**
 * @brief Возвращает обозначение ладьи с учетом цвета
 */
char Rook::getSymbol() const
{
    return getColor() == Color::White ? 'R' : 'r';
}

/**
 * @brief Выводит название и данные ладьи
 */
void Rook::printInfo() const
{
    std::cout << "Ладья: ";
    ChessPiece::printInfo();
}
