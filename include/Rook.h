#ifndef ROOK_H
#define ROOK_H

#include "SlidingPiece.h"

/**
 * @brief Шахматная ладья
 */
class Rook : public SlidingPiece
{
public:
    /**
     * @brief Создает ладью заданного цвета и позиции
     *
     * @param color Цвет ладьи
     * @param x Столбец
     * @param y Строка
     */
    Rook(Color color, int x, int y);

    /**
     * @brief Проверяет ход ладьи по горизонтали или вертикали
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если направление хода допустимо
     */
    bool canMoveTo(int newX, int newY) const override;

    /**
     * @brief Возвращает обозначение ладьи с учетом цвета
     *
     * @return R для белой ладьи, r для черной
     */
    char getSymbol() const override;

    /**
     * @brief Выводит название и данные ладьи
     */
    void printInfo() const override;
};

#endif
