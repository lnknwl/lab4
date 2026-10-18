#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "Board.h"
#include "SlidingPiece.h"

/**
 * @brief Создает доску из фигур
 */
Board::Board(const std::vector<ChessPiece*>& pieces) : pieces(pieces)
{
    if (!isValid())
    {
        throw std::invalid_argument("Неправильное состояние доски");
    }
}

/**
 * @brief Находит фигуру в клетке
 */
const ChessPiece* Board::getPieceAt(int x, int y) const
{
    if (!ChessPiece::isInsideBoard(x, y))
    {
        return nullptr;
    }
    for (const ChessPiece* piece : pieces)
    {
        if (piece->getX() == x && piece->getY() == y)
        {
            return piece;
        }
    }
    return nullptr;
}

/**
 * @brief Выполняет ход с учетом препятствий и цвета занятой клетки
 */
bool Board::movePiece(int fromX, int fromY, int toX, int toY)
{
    if (!isValid() || !ChessPiece::isInsideBoard(fromX, fromY) ||
        !ChessPiece::isInsideBoard(toX, toY))
    {
        return false;
    }
    auto source = std::find_if(pieces.begin(), pieces.end(), [=](const ChessPiece* piece) {
        return piece->getX() == fromX && piece->getY() == fromY;
    });
    if (source == pieces.end())
    {
        return false;
    }
    ChessPiece* moving = *source;
    const ChessPiece* target = getPieceAt(toX, toY);
    if ((target && target->getColor() == moving->getColor()) ||
        !moving->canMoveTo(toX, toY))
    {
        return false;
    }

    if (dynamic_cast<const SlidingPiece*>(moving))
    {
        int stepX = (toX > fromX) - (toX < fromX);
        int stepY = (toY > fromY) - (toY < fromY);
        for (int x = fromX + stepX, y = fromY + stepY; x != toX || y != toY;
             x += stepX, y += stepY)
        {
            if (getPieceAt(x, y))
            {
                return false;
            }
        }
    }

    if (!moving->moveTo(toX, toY))
    {
        return false;
    }
    if (target)
    {
        pieces.erase(std::remove(pieces.begin(), pieces.end(), target), pieces.end());
    }
    return true;
}

/**
 * @brief Проверяет текущее состояние доски
 */
bool Board::isValid() const
{
    return ChessPiece::validateBoardState(pieces);
}

/**
 * @brief Выводит доску, пустые клетки обозначаются точками
 */
void Board::print() const
{
    std::cout << "  0 1 2 3 4 5 6 7\n";
    for (int y = 7; y >= 0; --y)
    {
        std::cout << y << ' ';
        for (int x = 0; x < 8; ++x)
        {
            const ChessPiece* piece = getPieceAt(x, y);
            std::cout << (piece ? piece->getSymbol() : '.');
            if (x < 7)
            {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
}
