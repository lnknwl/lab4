#include <cstdlib>
#include "SlidingPiece.h"

/**
 * @brief Передает цвет и позицию базовому классу
 */
SlidingPiece::SlidingPiece(Color color, int x, int y)
    : ChessPiece(color, x, y)
{
}

/**
 * @brief Проверяет движение по горизонтали или вертикали
 */
bool SlidingPiece::isStraightMove(int newX, int newY) const
{
    return isValidDestination(newX, newY) &&
           (newX == getX() || newY == getY());
}

/**
 * @brief Проверяет движение по диагонали
 */
bool SlidingPiece::isDiagonalMove(int newX, int newY) const
{
    return isValidDestination(newX, newY) &&
           std::abs(newX - getX()) == std::abs(newY - getY());
}
