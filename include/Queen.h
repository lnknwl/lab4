#ifndef QUEEN_H
#define QUEEN_H

#include "SlidingPiece.h"
#include "CombinedPiece.h"

/**
 * @brief Шахматный ферзь с объединенным движением
 */
class Queen : public SlidingPiece, public CombinedPiece
{
public:
    /**
     * @brief Создает ферзя заданного цвета и позиции
     *
     * @param color Цвет ферзя
     * @param x Столбец
     * @param y Строка
     */
    Queen(Color color, int x, int y);

    /**
     * @brief Проверяет ход ферзя по прямой или диагонали
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если ход допустим
     */
    bool canMoveTo(int newX, int newY) const override;

    /**
     * @brief Объединяет правила движения ладьи и слона
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если направление прямое или диагональное
     */
    bool canMoveCombined(int newX, int newY) const override;

    /**
     * @brief Возвращает обозначение ферзя с учетом цвета
     *
     * @return Q для белого ферзя, q для черного
     */
    char getSymbol() const override;

    /**
     * @brief Выводит название и данные ферзя
     */
    void printInfo() const override;
};

#endif
