#ifndef BISHOP_H
#define BISHOP_H

#include "SlidingPiece.h"

/**
 * @brief Шахматный слон
 */
class Bishop : public SlidingPiece
{
public:
    /**
     * @brief Создает слона заданного цвета и позиции
     *
     * @param color Цвет слона
     * @param x Столбец
     * @param y Строка
     */
    Bishop(Color color, int x, int y);

    /**
     * @brief Проверяет ход слона по диагонали
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если направление хода допустимо
     */
    bool canMoveTo(int newX, int newY) const override;

    /**
     * @brief Возвращает обозначение слона с учетом цвета
     *
     * @return B для белого слона, b для черного
     */
    char getSymbol() const override;

    /**
     * @brief Выводит название и данные слона
     */
    void printInfo() const override;
};

#endif
