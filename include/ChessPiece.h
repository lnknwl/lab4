#ifndef CHESSPIECE_H
#define CHESSPIECE_H

/**
 * @brief Цвет шахматной фигуры
 */
enum class Color
{
    White,
    Black
};

/**
 * @brief Абстрактный класс шахматной фигуры
 */
class ChessPiece
{
private:
    Color color;
    int x;
    int y;
    static int whiteCount;
    static int blackCount;

protected:
    /**
     * @brief Создает фигуру заданного цвета и позиции
     *
     * @param color Цвет фигуры
     * @param x Координата столбца от 0 до 7
     * @param y Координата строки от 0 до 7
     * @throws std::invalid_argument Если цвет или координаты неправильные
     */
    ChessPiece(Color color, int x, int y);

    /**
     * @brief Копирует цвет и позицию фигуры
     *
     * @param other Фигура для копирования
     */
    ChessPiece(const ChessPiece& other);

    /**
     * @brief Присваивает цвет и позицию существующей фигуре
     *
     * @param other Фигура для копирования
     * @return Ссылка на измененную фигуру
     */
    ChessPiece& operator=(const ChessPiece& other);

    /**
     * @brief Проверяет границы и отличие от текущей клетки
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если клетка допустима
     */
    bool isValidDestination(int newX, int newY) const;

public:
    /**
     * @brief Завершает время жизни фигуры и уменьшает счетчик
     */
    virtual ~ChessPiece();

    /**
     * @brief Проверяет возможность хода по правилам фигуры
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если фигура может так ходить
     */
    virtual bool canMoveTo(int newX, int newY) const = 0;

    /**
     * @brief Возвращает обозначение фигуры
     *
     * @return Символ фигуры
     */
    virtual char getSymbol() const = 0;

    /**
     * @brief Выводит цвет, обозначение и позицию фигуры
     */
    virtual void printInfo() const;

    /**
     * @brief Выполняет разрешенный ход фигуры
     *
     * @param newX Столбец назначения
     * @param newY Строка назначения
     * @return true если позиция изменена
     */
    bool moveTo(int newX, int newY);

    /**
     * @brief Возвращает цвет фигуры
     *
     * @return Цвет фигуры
     */
    Color getColor() const;

    /**
     * @brief Возвращает координату столбца
     *
     * @return Столбец от 0 до 7
     */
    int getX() const;

    /**
     * @brief Возвращает координату строки
     *
     * @return Строка от 0 до 7
     */
    int getY() const;

    /**
     * @brief Проверяет, находится ли клетка на доске
     *
     * @param x Столбец
     * @param y Строка
     * @return true если обе координаты от 0 до 7
     */
    static bool isInsideBoard(int x, int y);

    /**
     * @brief Возвращает количество существующих белых фигур
     *
     * @return Количество белых фигур
     */
    static int getWhiteCount();

    /**
     * @brief Возвращает количество существующих черных фигур
     *
     * @return Количество черных фигур
     */
    static int getBlackCount();
};

#endif
