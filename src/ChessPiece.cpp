#include <iostream>
#include <stdexcept>
#include "ChessPiece.h"
#include "King.h"

int ChessPiece::whiteCount = 0;
int ChessPiece::blackCount = 0;

/**
 * @brief Проверяет позиции фигур и количество королей на доске
 */
bool ChessPiece::validateBoardState(const std::vector<ChessPiece*>& pieces)
{
    bool occupied[8][8] = {};
    int whiteKings = 0;
    int blackKings = 0;
    for (const ChessPiece* piece : pieces)
    {
        if (!piece || !isInsideBoard(piece->getX(), piece->getY()) ||
            (piece->getColor() != Color::White && piece->getColor() != Color::Black))
        {
            return false;
        }
        if (occupied[piece->getY()][piece->getX()])
        {
            return false;
        }
        occupied[piece->getY()][piece->getX()] = true;
        if (dynamic_cast<const King*>(piece))
        {
            if (piece->getColor() == Color::White)
            {
                ++whiteKings;
            }
            else
            {
                ++blackKings;
            }
        }
    }
    return whiteKings <= 1 && blackKings <= 1;
}

/**
 * @brief Создает фигуру заданного цвета и позиции
 */
ChessPiece::ChessPiece(Color color, int x, int y)
    : color(color), x(x), y(y)
{
    if (color != Color::White && color != Color::Black)
    {
        throw std::invalid_argument("Неизвестный цвет фигуры");
    }

    if (!isInsideBoard(x, y))
    {
        throw std::invalid_argument("Координаты должны быть от 0 до 7");
    }

    if (color == Color::White)
    {
        ++whiteCount;
    }
    else
    {
        ++blackCount;
    }
}

/**
 * @brief Копирует цвет и позицию фигуры
 */
ChessPiece::ChessPiece(const ChessPiece& other)
    : ChessPiece(other.color, other.x, other.y)
{
}

/**
 * @brief Присваивает цвет и позицию существующей фигуре
 */
ChessPiece& ChessPiece::operator=(const ChessPiece& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (color != other.color)
    {
        if (color == Color::White)
        {
            --whiteCount;
            ++blackCount;
        }
        else
        {
            --blackCount;
            ++whiteCount;
        }
    }

    color = other.color;
    x = other.x;
    y = other.y;
    return *this;
}

/**
 * @brief Завершает время жизни фигуры и уменьшает счетчик
 */
ChessPiece::~ChessPiece()
{
    if (color == Color::White)
    {
        --whiteCount;
    }
    else
    {
        --blackCount;
    }
}

/**
 * @brief Проверяет, находится ли клетка на доске
 */
bool ChessPiece::isInsideBoard(int x, int y)
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

/**
 * @brief Проверяет границы и отличие от текущей клетки
 */
bool ChessPiece::isValidDestination(int newX, int newY) const
{
    return isInsideBoard(newX, newY) && (newX != x || newY != y);
}

/**
 * @brief Выполняет разрешенный ход фигуры
 */
bool ChessPiece::moveTo(int newX, int newY)
{
    if (!isValidDestination(newX, newY) || !canMoveTo(newX, newY))
    {
        return false;
    }

    x = newX;
    y = newY;
    return true;
}

/**
 * @brief Возвращает цвет фигуры
 */
Color ChessPiece::getColor() const
{
    return color;
}

/**
 * @brief Возвращает координату столбца
 */
int ChessPiece::getX() const
{
    return x;
}

/**
 * @brief Возвращает координату строки
 */
int ChessPiece::getY() const
{
    return y;
}

/**
 * @brief Возвращает количество существующих белых фигур
 */
int ChessPiece::getWhiteCount()
{
    return whiteCount;
}

/**
 * @brief Возвращает количество существующих черных фигур
 */
int ChessPiece::getBlackCount()
{
    return blackCount;
}

/**
 * @brief Выводит цвет, обозначение и позицию фигуры
 */
void ChessPiece::printInfo() const
{
    std::cout << (color == Color::White ? "Белая" : "Черная") << " фигура "
              << getSymbol() << ": (" << x << ", " << y << ")\n";
}
