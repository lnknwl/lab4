#ifndef JUMPINGPIECE_H
#define JUMPINGPIECE_H

#include "ChessPiece.h"

/**
 * @brief Промежуточный класс фигур с прыжковым ходом
 */
class JumpingPiece : public ChessPiece
{
protected:
    /**
     * @brief Передает цвет и позицию базовому классу
     *
     * @param color Цвет фигуры
     * @param x Столбец
     * @param y Строка
     */
    JumpingPiece(Color color, int x, int y);

    /**
     * @brief Проверяет прыжок на две клетки по одной оси и одну по другой
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если конечная клетка и форма прыжка допустимы
     */
    bool isJumpMove(int newX, int newY) const;
};

#endif
