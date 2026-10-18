#include <iostream>
#include <algorithm>
#include <memory>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <sstream>
#include <windows.h>
#include "ChessPiece.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Board.h"

/**
 * @brief Вспомогательная фигура для проверки базового класса
 */
class TestPiece : public ChessPiece
{
public:
    /**
     * @brief Передает цвет и позицию базовому классу
     *
     * @param color Цвет фигуры
     * @param x Столбец
     * @param y Строка
     */
    TestPiece(Color color, int x, int y)
        : ChessPiece(color, x, y)
    {
    }

    /**
     * @brief Разрешает ход на одну клетку по горизонтали
     *
     * @param newX Новый столбец
     * @param newY Новая строка
     * @return true если ход разрешен
     */
    bool canMoveTo(int newX, int newY) const override
    {
        return isValidDestination(newX, newY) && newY == getY() &&
               (newX == getX() + 1 || newX == getX() - 1);
    }

    /**
     * @brief Возвращает вспомогательное обозначение
     *
     * @return Символ T
     */
    char getSymbol() const override
    {
        return 'T';
    }
};

/**
 * @brief Сообщает об ошибке, если условие не выполнено
 *
 * @param condition Результат условия
 * @param message Название проверки
 * @param errors Количество ошибок
 */
void check(bool condition, const std::string& message, int& errors)
{
    if (!condition)
    {
        std::cout << "Ошибка: " << message << '\n';
        ++errors;
    }
}

/**
 * @brief Проверяет отказ конструктора при неправильных данных
 *
 * @param color Цвет фигуры
 * @param x Столбец
 * @param y Строка
 * @return true если объект не создан и счетчики сохранены
 */
bool rejectsPiece(Color color, int x, int y)
{
    int white = ChessPiece::getWhiteCount();
    int black = ChessPiece::getBlackCount();
    try
    {
        TestPiece wrong(color, x, y);
    }
    catch (const std::invalid_argument&)
    {
        return ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black;
    }
    return false;
}

/**
 * @brief Проверяет ладью, слона и полиморфизм
 *
 * @param errors Количество ошибок
 */
void testSlidingPieces(int& errors)
{
    int white = ChessPiece::getWhiteCount();
    int black = ChessPiece::getBlackCount();
    check(std::is_abstract<SlidingPiece>::value &&
          std::is_base_of<ChessPiece, SlidingPiece>::value &&
          std::is_base_of<SlidingPiece, Rook>::value &&
          std::is_base_of<SlidingPiece, Bishop>::value,
          "В иерархии три уровня, SlidingPiece абстрактный", errors);

    {
        Rook rook(Color::White, 3, 3);
        Bishop bishop(Color::Black, 3, 3);
        ChessPiece* pieces[] = {&rook, &bishop};
        check(pieces[0]->getSymbol() == 'R' && pieces[1]->getSymbol() == 'b',
              "Символы фигур получены через базовые указатели", errors);
        check(ChessPiece::getWhiteCount() == white + 1 && ChessPiece::getBlackCount() == black + 1,
              "Наследники учитываются по одному разу", errors);

        const int straightMoves[][2] = {{0, 3}, {7, 3}, {3, 0}, {3, 7}};
        for (const auto& target : straightMoves)
        {
            check(pieces[0]->canMoveTo(target[0], target[1]) &&
                  !pieces[1]->canMoveTo(target[0], target[1]),
                  "Ладья ходит по прямой, слон такой ход отклоняет", errors);
        }
        const int diagonalMoves[][2] = {{0, 0}, {0, 6}, {6, 0}, {7, 7}};
        for (const auto& target : diagonalMoves)
        {
            check(pieces[1]->canMoveTo(target[0], target[1]) &&
                  !pieces[0]->canMoveTo(target[0], target[1]),
                  "Слон ходит по диагонали, ладья такой ход отклоняет", errors);
        }
        check(rook.getX() == 3 && rook.getY() == 3 && bishop.getX() == 3 && bishop.getY() == 3,
              "Вызов canMoveTo не изменяет позицию", errors);

        const int wrongMoves[][2] = {{3, 3}, {4, 5}, {-1, 3}, {8, 3}, {3, -1}, {3, 8}, {-1, -1}, {8, 8}};
        for (const auto& target : wrongMoves)
        {
            check(!pieces[0]->moveTo(target[0], target[1]) &&
                  !pieces[1]->moveTo(target[0], target[1]) &&
                  rook.getX() == 3 && rook.getY() == 3 && bishop.getX() == 3 && bishop.getY() == 3,
                  "Неправильный ход не изменил фигуры", errors);
        }

        std::ostringstream display;
        std::streambuf* oldBuffer = std::cout.rdbuf(display.rdbuf());
        pieces[0]->printInfo();
        pieces[1]->printInfo();
        std::cout.rdbuf(oldBuffer);
        check(display.str().find("Ладья:") != std::string::npos &&
              display.str().find("Слон:") != std::string::npos,
              "Наследники переопределили виртуальный вывод с базовой реализацией", errors);

        check(pieces[0]->moveTo(3, 7) && rook.getY() == 7 &&
              pieces[1]->moveTo(0, 0) && bishop.getX() == 0 && bishop.getY() == 0,
              "moveTo использует правила нужного наследника", errors);
        check(bishop.moveTo(7, 7) && bishop.moveTo(0, 0),
              "Слон ходит между углами доски в обе стороны", errors);

        Rook rookCopy(rook);
        check(rookCopy.moveTo(7, 7) && rook.getX() == 3 && rook.getY() == 7,
              "Копия ладьи изменяется независимо", errors);
        Bishop bishopCopy(Color::White, 1, 1);
        bishopCopy = bishop;
        check(bishopCopy.getColor() == Color::Black && bishopCopy.moveTo(1, 1) &&
              bishop.getX() == 0 && bishop.getY() == 0,
              "Присваивание слона скопировало цвет и сохранило независимость", errors);
        Rook blackRook(Color::Black, 0, 0);
        Bishop whiteBishop(Color::White, 7, 7);
        check(blackRook.getSymbol() == 'r' && whiteBishop.getSymbol() == 'B',
              "Обозначения зависят от цвета", errors);
    }

    check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
          "После уничтожения наследников счетчики восстановлены", errors);
    try
    {
        Rook wrong(static_cast<Color>(7), 0, 0);
        check(false, "Неверный цвет ладьи отклонен", errors);
    }
    catch (const std::invalid_argument&)
    {
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Неверный цвет наследника не изменил счетчики", errors);
    }
    try
    {
        Bishop wrong(Color::Black, 8, 0);
        check(false, "Неверная позиция слона отклонена", errors);
    }
    catch (const std::invalid_argument&)
    {
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Неверная позиция наследника не изменила счетчики", errors);
    }
}

/**
 * @brief Проверяет коня, копирование и полиморфизм
 *
 * @param errors Количество ошибок
 */
void testKnight(int& errors)
{
    int white = ChessPiece::getWhiteCount();
    int black = ChessPiece::getBlackCount();
    check(std::is_abstract<JumpingPiece>::value &&
          std::is_base_of<ChessPiece, JumpingPiece>::value &&
          std::is_base_of<JumpingPiece, Knight>::value,
          "Конь образует отдельную цепочку из трех уровней", errors);

    {
        Knight knight(Color::White, 3, 3);
        ChessPiece* piece = &knight;
        const int targets[][2] = {{1, 2}, {1, 4}, {2, 1}, {2, 5}, {4, 1}, {4, 5}, {5, 2}, {5, 4}};
        for (const auto& target : targets)
        {
            check(piece->canMoveTo(target[0], target[1]),
                  "Конь ходит в каждом из восьми направлений", errors);
        }
        check(knight.getX() == 3 && knight.getY() == 3 && piece->getSymbol() == 'N',
              "Запрос хода через базовый указатель не меняет позицию", errors);
        const int wrongTargets[][2] = {
            {3, 3}, {3, 5}, {5, 3}, {4, 4}, {6, 6}, {-1, 2}, {8, 2},
            {2, -1}, {2, 8}, {std::numeric_limits<int>::min(), 3},
            {3, std::numeric_limits<int>::max()}
        };
        for (const auto& target : wrongTargets)
        {
            check(!piece->canMoveTo(target[0], target[1]) &&
                  !piece->moveTo(target[0], target[1]) && knight.getX() == 3 && knight.getY() == 3,
                  "Неправильный прыжок не меняет позицию коня", errors);
        }
        check(piece->moveTo(5, 4) && knight.getX() == 5 && knight.getY() == 4,
              "Разрешенный прыжок перемещает коня", errors);

        Knight copy(knight);
        check(copy.moveTo(3, 3) && knight.getX() == 5 && knight.getY() == 4,
              "Копия коня перемещается независимо", errors);
        Knight blackKnight(Color::Black, 0, 0);
        check(blackKnight.getSymbol() == 'n', "Черный конь имеет обозначение n", errors);
        blackKnight = knight;
        check(blackKnight.getColor() == Color::White && blackKnight.getSymbol() == 'N' &&
              blackKnight.getX() == 5 && blackKnight.getY() == 4 &&
              ChessPiece::getWhiteCount() == white + 3 && ChessPiece::getBlackCount() == black,
              "Присваивание коня копирует цвет, позицию и обновляет статистику", errors);
        check(blackKnight.moveTo(4, 2) && knight.getX() == 5 && knight.getY() == 4,
              "После присваивания конь независим от оригинала", errors);
        const Knight& sameKnight = knight;
        knight = sameKnight;
        check(knight.getX() == 5 && knight.getY() == 4 && ChessPiece::getWhiteCount() == white + 3,
              "Самоприсваивание коня сохраняет данные", errors);

        std::ostringstream display;
        std::streambuf* oldBuffer = std::cout.rdbuf(display.rdbuf());
        piece->printInfo();
        std::cout.rdbuf(oldBuffer);
        check(display.str().find("Конь:") != std::string::npos,
              "Конь переопределяет виртуальный вывод информации", errors);
    }

    const int corners[][6] = {{0, 0, 1, 2, 2, 1}, {7, 0, 5, 1, 6, 2},
                             {0, 7, 1, 5, 2, 6}, {7, 7, 5, 6, 6, 5}};
    for (const auto& corner : corners)
    {
        Knight knight(Color::Black, corner[0], corner[1]);
        check(knight.canMoveTo(corner[2], corner[3]) && knight.canMoveTo(corner[4], corner[5]),
              "Из угла доступны оба хода коня", errors);
    }

    {
        std::unique_ptr<ChessPiece> piece(new Knight(Color::Black, 1, 0));
        check(piece->moveTo(2, 2) && ChessPiece::getBlackCount() == black + 1,
              "Динамический конь используется через ChessPiece", errors);
    }
    check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
          "После удаления коней счетчики восстановлены", errors);
    try
    {
        Knight wrong(Color::White, 0, 8);
        check(false, "Конь с неправильной позицией отклонен", errors);
    }
    catch (const std::invalid_argument&)
    {
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Ошибка конструктора коня не меняет счетчики", errors);
    }
    try
    {
        Knight wrong(static_cast<Color>(5), 0, 0);
        check(false, "Неизвестный цвет коня отклонен", errors);
    }
    catch (const std::invalid_argument&)
    {
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Неправильный цвет коня не меняет счетчики", errors);
    }
}

/**
 * @brief Проверяет ферзя и множественное наследование
 *
 * @param errors Количество ошибок
 */
void testQueen(int& errors)
{
    int white = ChessPiece::getWhiteCount();
    int black = ChessPiece::getBlackCount();
    check(std::is_abstract<CombinedPiece>::value &&
          std::has_virtual_destructor<CombinedPiece>::value &&
          std::is_base_of<SlidingPiece, Queen>::value && std::is_base_of<CombinedPiece, Queen>::value,
          "Ферзь наследует класс и абстрактный интерфейс", errors);

    {
        Queen queen(Color::White, 3, 3);
        ChessPiece* piece = &queen;
        CombinedPiece* combined = &queen;
        check(ChessPiece::getWhiteCount() == white + 1 && ChessPiece::getBlackCount() == black,
              "Две базы не дублируют фигуру и счетчик", errors);
        check(dynamic_cast<ChessPiece*>(combined) == piece &&
              dynamic_cast<CombinedPiece*>(piece) == combined,
              "Обе базы ссылаются на один объект ферзя", errors);

        const int targets[][2] = {{0, 3}, {7, 3}, {3, 0}, {3, 7}, {0, 0}, {0, 6}, {6, 0}, {7, 7}};
        for (const auto& target : targets)
        {
            check(piece->canMoveTo(target[0], target[1]) &&
                  combined->canMoveCombined(target[0], target[1]),
                  "Ферзь доступен по всем направлениям через обе базы", errors);
        }
        check(queen.getX() == 3 && queen.getY() == 3 && piece->getSymbol() == 'Q',
              "Запрос хода не меняет позицию ферзя", errors);
        const int wrongTargets[][2] = {{3, 3}, {4, 5}, {5, 4}, {-1, 3}, {8, 3}, {3, -1}, {3, 8},
                                      {std::numeric_limits<int>::min(), 3},
                                      {3, std::numeric_limits<int>::max()}};
        for (const auto& target : wrongTargets)
        {
            check(!combined->canMoveCombined(target[0], target[1]) &&
                  !piece->moveTo(target[0], target[1]) && queen.getX() == 3 && queen.getY() == 3,
                  "Неправильный ход ферзя сохраняет позицию", errors);
        }

        check(piece->moveTo(3, 7) && piece->moveTo(7, 7) && piece->moveTo(0, 0),
              "Ферзь последовательно ходит по вертикали, горизонтали и диагонали", errors);
        Queen copy(queen);
        check(copy.moveTo(7, 7) && queen.getX() == 0 && queen.getY() == 0,
              "Копия ферзя перемещается независимо", errors);
        Queen blackQueen(Color::Black, 1, 1);
        check(blackQueen.getSymbol() == 'q', "Черный ферзь имеет обозначение q", errors);
        blackQueen = queen;
        check(blackQueen.getColor() == Color::White && blackQueen.getX() == 0 && blackQueen.getY() == 0 &&
              ChessPiece::getWhiteCount() == white + 3 && ChessPiece::getBlackCount() == black,
              "Присваивание ферзя корректно обрабатывает обе базы и статистику", errors);
        check(blackQueen.moveTo(0, 7) && queen.getY() == 0,
              "После присваивания ферзь независим от оригинала", errors);
        const Queen& sameQueen = queen;
        queen = sameQueen;
        check(queen.getX() == 0 && queen.getY() == 0 && ChessPiece::getWhiteCount() == white + 3,
              "Самоприсваивание ферзя сохраняет данные", errors);

        std::ostringstream display;
        std::streambuf* oldBuffer = std::cout.rdbuf(display.rdbuf());
        piece->printInfo();
        std::cout.rdbuf(oldBuffer);
        check(display.str().find("Ферзь:") != std::string::npos,
              "Ферзь переопределяет виртуальный вывод", errors);
    }
    check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
          "После уничтожения ферзей счетчики восстановлены", errors);

    {
        std::unique_ptr<CombinedPiece> combined(new Queen(Color::Black, 0, 0));
        check(combined->canMoveCombined(7, 7) && ChessPiece::getBlackCount() == black + 1,
              "Ферзь используется через указатель на вторую базу", errors);
    }
    check(ChessPiece::getBlackCount() == black,
          "Удаление через интерфейс вызывает деструктор фигуры", errors);
    try
    {
        Queen wrong(Color::White, -1, 0);
        check(false, "Неверная позиция ферзя отклонена", errors);
    }
    catch (const std::invalid_argument&)
    {
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Ошибка позиции ферзя не меняет счетчики", errors);
    }
    try
    {
        Queen wrong(static_cast<Color>(5), 0, 0);
        check(false, "Неизвестный цвет ферзя отклонен", errors);
    }
    catch (const std::invalid_argument&)
    {
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Неправильный цвет ферзя не меняет счетчики", errors);
    }
}

/**
 * @brief Проверяет короля, препятствия, взятие и состояние доски
 * @param errors Количество ошибок
 */
void testBoard(int& errors)
{
    {
        King king(Color::White, 3, 3);
        ChessPiece* base = &king;
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                check(base->canMoveTo(3 + dx, 3 + dy) == (dx != 0 || dy != 0),
                      "Король ходит на все восемь соседних клеток", errors);
            }
        }
        check(!base->moveTo(3, 5) && !base->moveTo(-1, 3) &&
              !base->canMoveTo(std::numeric_limits<int>::max(), 3) &&
              !base->canMoveTo(3, std::numeric_limits<int>::min()) && king.getY() == 3,
              "Неверные ходы короля не меняют позицию", errors);
        King edge(Color::Black, 0, 0);
        check(edge.getSymbol() == 'k' && base->getSymbol() == 'K' &&
              edge.canMoveTo(1, 0) && edge.canMoveTo(0, 1) && edge.canMoveTo(1, 1) &&
              !edge.canMoveTo(-1, 0), "Король в углу учитывает границы", errors);
        std::ostringstream display;
        std::streambuf* previous = std::cout.rdbuf(display.rdbuf());
        base->printInfo();
        std::cout.rdbuf(previous);
        check(display.str().find("Король:") != std::string::npos,
              "Король переопределяет виртуальный вывод", errors);
        check(ChessPiece::validateBoardState({&king, &edge}),
              "Разные цвета королей допустимы", errors);
        King white2(Color::White, 7, 7);
        King black2(Color::Black, 0, 7);
        check(!ChessPiece::validateBoardState({&king, &white2}) &&
              !ChessPiece::validateBoardState({&edge, &black2}),
              "Два короля одного цвета отклоняются для обоих цветов", errors);
        check(ChessPiece::validateBoardState({}) && ChessPiece::validateBoardState({&king}) &&
              !ChessPiece::validateBoardState({nullptr}) &&
              !ChessPiece::validateBoardState({&king, &king}),
              "Пустая доска допустима, пустые и повторные указатели отклоняются", errors);
        Rook sameCell(Color::Black, 3, 3);
        check(!ChessPiece::validateBoardState({&king, &sameCell}),
              "Совпадение клеток разных объектов отклоняется", errors);
        try
        {
            Board wrong({&king, &white2});
            check(false, "Конструктор доски отклоняет неправильное состояние", errors);
        }
        catch (const std::invalid_argument&)
        {
        }
    }

    {
        Rook rook(Color::White, 0, 0);
        Knight knight(Color::White, 1, 0);
        Bishop enemy(Color::Black, 0, 2);
        Board board({&rook, &knight, &enemy});
        int white = ChessPiece::getWhiteCount();
        int black = ChessPiece::getBlackCount();
        check(!board.movePiece(0, 0, 2, 0) && !board.movePiece(0, 0, 1, 0) &&
              !board.movePiece(0, 0, 0, 3) && rook.getX() == 0 && rook.getY() == 0,
              "Нельзя пройти сквозь фигуры любого цвета или занять свою клетку", errors);
        check(board.movePiece(1, 0, 0, 2) && board.getPieceAt(0, 2) == &knight &&
              board.getPieceAt(1, 0) == nullptr && board.isValid(),
              "Конь перепрыгивает ладью и берет чужую фигуру", errors);
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black &&
              enemy.getX() == 0 && enemy.getY() == 2,
              "Взятие убирает указатель с доски, но не уничтожает внешний объект", errors);
        check(!board.movePiece(0, 0, 0, 2) && !board.movePiece(0, 0, 0, 3),
              "После взятия клетка и путь заняты своим конем", errors);
        check(board.movePiece(0, 2, 2, 3) && board.movePiece(0, 0, 0, 7) &&
              board.movePiece(0, 7, 0, 0), "Ладья проходит по освободившемуся пути в обе стороны", errors);
        check(!board.movePiece(0, 0, 0, 0) && !board.movePiece(7, 7, 6, 7) &&
              !board.movePiece(-1, 0, 0, 0) && !board.movePiece(0, 0, 8, 0) &&
              board.getPieceAt(8, 0) == nullptr && board.isValid(),
              "Пустой источник, прежняя клетка и выход за границы отклоняются", errors);
    }

    {
        Bishop bishop(Color::White, 0, 0);
        Rook blocker(Color::Black, 2, 2);
        Board board({&bishop, &blocker});
        check(!board.movePiece(0, 0, 7, 7) && board.movePiece(0, 0, 2, 2) &&
              board.movePiece(2, 2, 7, 7) && board.movePiece(7, 7, 0, 0) && board.isValid(),
              "Слон не проходит препятствие, но берет его и ходит в обе стороны", errors);
    }
    {
        Queen queen(Color::White, 3, 3);
        Rook blocker(Color::Black, 3, 5);
        Bishop diagonal(Color::White, 2, 2);
        Board board({&queen, &blocker, &diagonal});
        check(!board.movePiece(3, 3, 3, 7) && !board.movePiece(3, 3, 0, 0) &&
              !board.movePiece(3, 3, 2, 2) && board.movePiece(3, 3, 3, 5) &&
              board.movePiece(3, 5, 5, 7) && board.isValid(),
              "Ферзь учитывает препятствия и взятие по прямой и диагонали", errors);
    }
    {
        King king(Color::White, 3, 3);
        Rook own(Color::White, 4, 3);
        Knight enemy(Color::Black, 2, 2);
        Board board({&king, &own, &enemy});
        check(!board.movePiece(3, 3, 4, 3) && board.movePiece(3, 3, 2, 2) && board.isValid(),
              "Король не занимает свою фигуру и берет чужую на соседней клетке", errors);
        own.moveTo(2, 3);
        own.moveTo(2, 2);
        check(!board.isValid() && !board.movePiece(2, 2, 1, 1),
              "Доска замечает неправильное внешнее изменение фигур и отклоняет ход", errors);
    }
    Board empty;
    check(empty.isValid() && !empty.movePiece(0, 0, 0, 1),
          "На пустой доске нельзя сделать ход", errors);
    std::ostringstream display;
    std::streambuf* previous = std::cout.rdbuf(display.rdbuf());
    empty.print();
    std::cout.rdbuf(previous);
    const std::string boardText = display.str();
    check(boardText.find("0 1 2 3 4 5 6 7") != std::string::npos &&
          std::count(boardText.begin(), boardText.end(), '.') == 64,
          "Пустая доска выводит координаты и 64 клетки", errors);
}

/**
 * @brief Проверяет копирование и счетчики одного конкретного типа
 * @tparam T Тип фигуры
 * @param x Столбец для разрешенного хода из (3, 3)
 * @param y Строка для разрешенного хода из (3, 3)
 * @param errors Количество ошибок
 */
template <typename T>
void testCopying(int x, int y, int& errors)
{
    const int white = ChessPiece::getWhiteCount();
    const int black = ChessPiece::getBlackCount();
    {
        const T original(Color::White, 3, 3);
        T copy(original);
        T assigned(Color::Black, 0, 0);
        check(copy.getColor() == original.getColor() && copy.getX() == 3 && copy.getY() == 3 &&
              copy.getSymbol() == original.getSymbol() &&
              ChessPiece::getWhiteCount() == white + 2 && ChessPiece::getBlackCount() == black + 1,
              "Копия конкретного типа сохраняет данные и увеличивает счетчик один раз", errors);
        check(copy.moveTo(x, y) && original.getX() == 3 && original.getY() == 3,
              "Перемещение копии не меняет оригинал", errors);
        check(&(assigned = original) == &assigned && assigned.getX() == 3 && assigned.getY() == 3 &&
              assigned.getColor() == Color::White && assigned.getSymbol() == original.getSymbol() &&
              ChessPiece::getWhiteCount() == white + 3 && ChessPiece::getBlackCount() == black,
              "Присваивание возвращает ссылку, копирует данные и меняет счетчик цвета", errors);
        check(assigned.moveTo(x, y) && original.getX() == 3 && original.getY() == 3,
              "Присвоенная фигура независима от оригинала", errors);
        const T& same = assigned;
        check(&(assigned = same) == &assigned && assigned.getX() == x && assigned.getY() == y &&
              ChessPiece::getWhiteCount() == white + 3 && ChessPiece::getBlackCount() == black,
              "Самоприсваивание сохраняет позицию, ссылку и счетчики", errors);
        const T blackSource(Color::Black, 3, 3);
        assigned = blackSource;
        check(assigned.getColor() == Color::Black && assigned.getSymbol() == blackSource.getSymbol() &&
              assigned.getX() == 3 && assigned.getY() == 3 &&
              ChessPiece::getWhiteCount() == white + 2 && ChessPiece::getBlackCount() == black + 2,
              "Обратная смена цвета при присваивании учитывается правильно", errors);
        assigned = blackSource;
        check(ChessPiece::getWhiteCount() == white + 2 && ChessPiece::getBlackCount() == black + 2,
              "Повторное присваивание того же цвета не меняет счетчики", errors);
        check(assigned.moveTo(x, y) && blackSource.getX() == 3 && blackSource.getY() == 3,
              "Черная присвоенная фигура независима от источника", errors);
        std::unique_ptr<ChessPiece> dynamicCopy(new T(blackSource));
        check(dynamicCopy->getSymbol() == blackSource.getSymbol() &&
              dynamicCopy->moveTo(x, y) && blackSource.getX() == 3 && blackSource.getY() == 3 &&
              ChessPiece::getBlackCount() == black + 3,
              "Динамическая копия доступна через базовый указатель", errors);
        dynamicCopy.reset();
        check(ChessPiece::getBlackCount() == black + 2,
              "Удаление копии через базовый указатель уменьшает счетчик один раз", errors);
    }
    check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
          "После уничтожения всех копий оба счетчика восстановлены", errors);
    for (int invalid = 0; invalid < 2; ++invalid)
    {
        try
        {
            T wrong(invalid == 0 ? static_cast<Color>(10) : Color::White,
                    invalid == 0 ? 0 : 8, 0);
            check(false, "Неправильные данные отклонены конструктором наследника", errors);
        }
        catch (const std::invalid_argument&)
        {
        }
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Неудачный конструктор наследника сохраняет счетчики", errors);
    }
}

/**
 * @brief Проверяет совместную статистику всей иерархии
 * @param errors Количество ошибок
 */
void testHierarchyStatistics(int& errors)
{
    const int white = ChessPiece::getWhiteCount();
    const int black = ChessPiece::getBlackCount();
    {
        std::vector<std::unique_ptr<ChessPiece>> pieces;
        pieces.emplace_back(new Rook(Color::White, 0, 0));
        pieces.emplace_back(new Bishop(Color::Black, 1, 0));
        pieces.emplace_back(new Knight(Color::White, 2, 0));
        pieces.emplace_back(new Queen(Color::Black, 3, 0));
        pieces.emplace_back(new King(Color::White, 4, 0));
        check(ChessPiece::getWhiteCount() == white + 3 && ChessPiece::getBlackCount() == black + 2,
              "Статистика общая для разных ветвей наследования", errors);
        pieces.erase(pieces.begin() + 3);
        check(ChessPiece::getWhiteCount() == white + 3 && ChessPiece::getBlackCount() == black + 1,
              "Удаление ферзя с двумя базами уменьшает общий счетчик один раз", errors);
        pieces.clear();
        check(ChessPiece::getWhiteCount() == white && ChessPiece::getBlackCount() == black,
              "Удаление смешанного набора через базовые указатели восстанавливает статистику", errors);
    }
}

/**
 * @brief Проверяет базовый класс и фигуры
 *
 * @return 0 если все проверки прошли, иначе 1
 */
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int errors{};

    check(std::is_abstract<ChessPiece>::value, "ChessPiece является абстрактным", errors);
    check(std::has_virtual_destructor<ChessPiece>::value, "Деструктор базового класса виртуальный", errors);
    check(ChessPiece::getWhiteCount() == 0 && ChessPiece::getBlackCount() == 0,
          "До создания фигур счетчики равны нулю", errors);
    check(ChessPiece::isInsideBoard(0, 0) && ChessPiece::isInsideBoard(7, 7),
          "Угловые клетки находятся на доске", errors);

    {
        TestPiece white(Color::White, 0, 0);
        TestPiece black(Color::Black, 7, 7);
        check(white.getColor() == Color::White && white.getX() == 0 && white.getY() == 0,
              "Конструктор сохранил цвет и позицию", errors);
        check(ChessPiece::getWhiteCount() == 1 && ChessPiece::getBlackCount() == 1,
              "Фигуры учитываются по цветам", errors);

        ChessPiece* piece = &white;
        piece->printInfo();
        check(piece->getSymbol() == 'T' && piece->canMoveTo(1, 0),
              "Чисто виртуальные функции вызываются через базовый указатель", errors);
        check(piece->moveTo(1, 0) && white.getX() == 1 && white.getY() == 0,
              "Разрешенный ход изменил позицию", errors);
        check(!piece->moveTo(1, 0) && !piece->moveTo(1, 1) &&
              white.getX() == 1 && white.getY() == 0,
              "Ход в ту же клетку и неправильный ход отклонены", errors);

        TestPiece copy(white);
        check(copy.getColor() == white.getColor() && copy.getX() == 1 && copy.getY() == 0,
              "Копирование сохранило цвет и позицию", errors);
        check(ChessPiece::getWhiteCount() == 2 && ChessPiece::getBlackCount() == 1,
              "Копирование увеличило нужный счетчик", errors);
        check(copy.moveTo(2, 0) && white.getX() == 1,
              "Изменение копии не изменило оригинал", errors);

        black = white;
        check(black.getColor() == Color::White && black.getX() == 1 && black.getY() == 0,
              "Присваивание скопировало цвет и позицию", errors);
        check(ChessPiece::getWhiteCount() == 3 && ChessPiece::getBlackCount() == 0,
              "Смена цвета при присваивании обновила счетчики", errors);
        check(black.moveTo(0, 0) && white.getX() == 1,
              "После присваивания объекты независимы", errors);
        const TestPiece& samePiece = white;
        white = samePiece;
        check(white.getX() == 1 && ChessPiece::getWhiteCount() == 3,
              "Самоприсваивание не изменило данные и счетчики", errors);

        const int wrongPositions[][2] = {{-1, 0}, {8, 0}, {0, -1}, {0, 8}};
        for (const auto& position : wrongPositions)
        {
            check(rejectsPiece(Color::White, position[0], position[1]),
                  "Неправильная позиция отклонена, счетчики сохранены", errors);
            check(!white.moveTo(position[0], position[1]) && white.getX() == 1 && white.getY() == 0,
                  "Ход за границы не изменил позицию", errors);
        }
        check(rejectsPiece(static_cast<Color>(5), 0, 0),
              "Неизвестный цвет отклонен", errors);

        {
            std::unique_ptr<ChessPiece> temporary(new TestPiece(Color::Black, 3, 3));
            check(ChessPiece::getBlackCount() == 1 && temporary->canMoveTo(4, 3),
                  "Динамическая фигура доступна через базовый указатель", errors);
        }
        check(ChessPiece::getBlackCount() == 0,
              "Удаление через базовый указатель вызвало нужный деструктор", errors);
    }

    testSlidingPieces(errors);
    testKnight(errors);
    testQueen(errors);
    testBoard(errors);
    testCopying<Rook>(3, 6, errors);
    testCopying<Bishop>(5, 5, errors);
    testCopying<Knight>(5, 4, errors);
    testCopying<Queen>(6, 3, errors);
    testCopying<King>(4, 4, errors);
    testHierarchyStatistics(errors);
    check(ChessPiece::getWhiteCount() == 0 && ChessPiece::getBlackCount() == 0,
          "После уничтожения фигур счетчики равны нулю", errors);
    if (errors == 0)
    {
        std::cout << "Все проверки выполнены\n";
    }
    else
    {
        std::cout << "Количество ошибок: " << errors << '\n';
    }
    return errors == 0 ? 0 : 1;
}
