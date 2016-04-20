#ifndef DATUM_H
#define DATUM_H

#include "brick.h"
#include "stack"
#include <QDebug>

/*
Основная структура данных - совокупность всех фишек на доске. На базе данной структуры осуществляется
управлением перемещениями на поле, выполнение алгоритмов
*/
class Datum : public std::vector<Brick>
{
public:
    static const Size field;

    Datum();
    ~Datum();

    //Сколько звёзд стоят в один ряд
    int result() const;
    //Оценочная функция (сумма количества перемещений, приведших к текущему перемещению) и количества звёзд на линии
    int valuation() const;

    //Набор возможных перемещений
    enum Mobility {
        Left,
        Right,
        Bottom,
        Top
    };

    //Структура, описывающая один шаг. Содержит порядковый номер смещаемой плитки и направление смещения
    struct Step { Step(int i, Mobility m) : id(i), d(m) {} int id; Mobility d; };
    typedef std::stack<Step> Solve;

    //операторы сравнения для обеспечения хранения в древовидных структурах
    bool operator==(const Datum&) const;
    bool operator<(const Datum&) const;

    //Доступ к пустым позициями
    std::vector<Point> empyFields() const { return std::vector<Point>{ empty[0], empty[1] }; }

    static Solve solve();
    std::stack<Step> mobilities() const;

    Datum realize(const Step&) const;

private:
    Point empty[2];

    //в операторе сравнения структуры сравниваются по хэшу
    quint64 hash() const;

    Solve way;
};

//Доопределение отладочного вывода
inline QDebug operator<<(QDebug out, const Datum::Mobility& m) {
    switch (m) {
    case Datum::Left:
        return out << "left";
    case Datum::Right:
        return out << "Right";
    case Datum::Bottom:
        return out << "Bottom";
    case Datum::Top:
        return out << "Top";
    }
    return out;
}

#endif // DATUM_H
