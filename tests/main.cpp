#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <windows.h>
#include "ChessPiece.h"

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
 * @brief Выводит результат проверки
 *
 * @param condition Результат условия
 * @param message Название проверки
 * @param errors Количество ошибок
 */
void check(bool condition, const std::string& message, int& errors)
{
    std::cout << (condition ? "[OK] " : "[Ошибка] ") << message << '\n';
    if (!condition)
    {
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
 * @brief Проверяет базовый класс, копирование и время жизни фигур
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

    check(ChessPiece::getWhiteCount() == 0 && ChessPiece::getBlackCount() == 0,
          "После уничтожения фигур счетчики равны нулю", errors);
    std::cout << "\nКоличество ошибок: " << errors << '\n';
    return errors == 0 ? 0 : 1;
}
