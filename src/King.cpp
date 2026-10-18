#include <cstdlib>
#include <iostream>
#include "King.h"

/**
 * @brief Создает короля заданного цвета и позиции
 */
King::King(Color color, int x, int y) : ChessPiece(color, x, y)
{
}

/**
 * @brief Проверяет ход на соседнюю клетку
 */
bool King::canMoveTo(int newX, int newY) const
{
    return isValidDestination(newX, newY) &&
           std::abs(newX - getX()) <= 1 && std::abs(newY - getY()) <= 1;
}

/**
 * @brief Возвращает обозначение короля
 */
char King::getSymbol() const
{
    return getColor() == Color::White ? 'K' : 'k';
}

/**
 * @brief Выводит название и данные короля
 */
void King::printInfo() const
{
    std::cout << "Король: ";
    ChessPiece::printInfo();
}
