#ifndef COMBINEDPIECE_H
#define COMBINEDPIECE_H

/**
 * @brief Интерфейс фигуры, объединяющей несколько направлений движения
 */
class CombinedPiece
{
public:
    /**
     * @brief Позволяет удалять фигуру через указатель на интерфейс
     */
    virtual ~CombinedPiece() = default;

    /**
     * @brief Проверяет ход по объединенным правилам фигуры
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если ход допустим
     */
    virtual bool canMoveCombined(int newX, int newY) const = 0;
};

#endif
