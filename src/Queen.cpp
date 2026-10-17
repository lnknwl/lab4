#include <iostream>
#include "Queen.h"

/**
 * @brief Создает ферзя заданного цвета и позиции
 */
Queen::Queen(Color color, int x, int y)
    : SlidingPiece(color, x, y)
{
}

/**
 * @brief Проверяет ход ферзя по прямой или диагонали
 */
bool Queen::canMoveTo(int newX, int newY) const
{
    return canMoveCombined(newX, newY);
}

/**
 * @brief Объединяет правила движения ладьи и слона
 */
bool Queen::canMoveCombined(int newX, int newY) const
{
    return isStraightMove(newX, newY) || isDiagonalMove(newX, newY);
}

/**
 * @brief Возвращает обозначение ферзя с учетом цвета
 */
char Queen::getSymbol() const
{
    return getColor() == Color::White ? 'Q' : 'q';
}

/**
 * @brief Выводит название и данные ферзя
 */
void Queen::printInfo() const
{
    std::cout << "Ферзь: ";
    ChessPiece::printInfo();
}
