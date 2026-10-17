#include <iostream>
#include "Knight.h"

/**
 * @brief Создает коня заданного цвета и позиции
 */
Knight::Knight(Color color, int x, int y)
    : JumpingPiece(color, x, y)
{
}

/**
 * @brief Проверяет ход коня буквой Г
 */
bool Knight::canMoveTo(int newX, int newY) const
{
    return isJumpMove(newX, newY);
}

/**
 * @brief Возвращает обозначение коня с учетом цвета
 */
char Knight::getSymbol() const
{
    return getColor() == Color::White ? 'N' : 'n';
}

/**
 * @brief Выводит название и данные коня
 */
void Knight::printInfo() const
{
    std::cout << "Конь: ";
    ChessPiece::printInfo();
}
