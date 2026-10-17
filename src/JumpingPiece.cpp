#include <cstdlib>
#include "JumpingPiece.h"

/**
 * @brief Передает цвет и позицию базовому классу
 */
JumpingPiece::JumpingPiece(Color color, int x, int y)
    : ChessPiece(color, x, y)
{
}

/**
 * @brief Проверяет прыжок на две клетки по одной оси и одну по другой
 */
bool JumpingPiece::isJumpMove(int newX, int newY) const
{
    if (!isValidDestination(newX, newY))
    {
        return false;
    }

    int dx = std::abs(newX - getX());
    int dy = std::abs(newY - getY());
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}
