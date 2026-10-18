#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

/**
 * @brief Король с ходом на одну клетку
 */
class King : public ChessPiece
{
public:
    /**
     * @brief Создает короля заданного цвета и позиции
     * @param color Цвет короля
     * @param x Столбец
     * @param y Строка
     */
    King(Color color, int x, int y);

    /**
     * @brief Проверяет ход на соседнюю клетку
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если клетка соседняя
     */
    bool canMoveTo(int newX, int newY) const override;

    /**
     * @brief Возвращает обозначение короля
     * @return K для белого, k для черного
     */
    char getSymbol() const override;

    /**
     * @brief Выводит название и данные короля
     */
    void printInfo() const override;
};

#endif
