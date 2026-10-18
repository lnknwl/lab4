#ifndef BOARD_H
#define BOARD_H

#include "ChessPiece.h"

/**
 * @brief Доска с указателями на существующие фигуры
 *
 * Фигуры должны жить дольше доски. Доска их не удаляет.
 */
class Board
{
private:
    std::vector<ChessPiece*> pieces;

public:
    /**
     * @brief Создает доску из фигур
     * @param pieces Фигуры на доске
     * @throws std::invalid_argument Если состояние неправильное
     */
    explicit Board(const std::vector<ChessPiece*>& pieces = {});

    /**
     * @brief Находит фигуру в клетке
     * @param x Столбец
     * @param y Строка
     * @return Указатель на фигуру или nullptr для пустой клетки и выхода за границы
     */
    const ChessPiece* getPieceAt(int x, int y) const;

    /**
     * @brief Выполняет ход с учетом препятствий и цвета занятой клетки
     * @param fromX Начальный столбец
     * @param fromY Начальная строка
     * @param toX Конечный столбец
     * @param toY Конечная строка
     * @return true если ход выполнен
     */
    bool movePiece(int fromX, int fromY, int toX, int toY);

    /**
     * @brief Проверяет текущее состояние доски
     * @return true если состояние допустимо
     */
    bool isValid() const;

    /**
     * @brief Выводит доску, пустые клетки обозначаются точками
     */
    void print() const;
};

#endif
