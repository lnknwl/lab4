#ifndef KNIGHT_H
#define KNIGHT_H

#include "JumpingPiece.h"

/**
 * @brief Шахматный конь
 */
class Knight : public JumpingPiece
{
public:
    /**
     * @brief Создает коня заданного цвета и позиции
     *
     * @param color Цвет коня
     * @param x Столбец
     * @param y Строка
     */
    Knight(Color color, int x, int y);

    /**
     * @brief Проверяет ход коня буквой Г
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если прыжок допустим
     */
    bool canMoveTo(int newX, int newY) const override;

    /**
     * @brief Возвращает обозначение коня с учетом цвета
     *
     * @return N для белого коня, n для черного
     */
    char getSymbol() const override;

    /**
     * @brief Выводит название и данные коня
     */
    void printInfo() const override;
};

#endif
