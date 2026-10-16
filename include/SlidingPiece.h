#ifndef SLIDINGPIECE_H
#define SLIDINGPIECE_H

#include "ChessPiece.h"

/**
 * @brief Промежуточный класс фигур, движущихся по линии
 */
class SlidingPiece : public ChessPiece
{
protected:
    /**
     * @brief Передает цвет и позицию базовому классу
     *
     * @param color Цвет фигуры
     * @param x Столбец
     * @param y Строка
     */
    SlidingPiece(Color color, int x, int y);

    /**
     * @brief Проверяет движение по горизонтали или вертикали
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если направление прямое и клетка допустима
     */
    bool isStraightMove(int newX, int newY) const;

    /**
     * @brief Проверяет движение по диагонали
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если направление диагональное и клетка допустима
     */
    bool isDiagonalMove(int newX, int newY) const;
};

#endif
