#include "datum.h"
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>

const Size Datum::field = Size(5,3);

Datum::Datum() :
    std::vector<Brick>({
        Brick(Size(1, 1), Point(1, 0), Brick::Stars{}),
        Brick(Size(1, 2), Point(2, 0), Brick::Stars{Point(0,0)}),
        Brick(Size(2, 1), Point(3, 0), Brick::Stars{Point(0,0), Point(1,0)}),
        Brick(Size(1, 2), Point(0, 1), Brick::Stars{Point(0,1)}),
        Brick(Size(1, 1), Point(1, 1), Brick::Stars{}),
        Brick(Size(2, 1), Point(3, 1), Brick::Stars{}),
        Brick(Size(1, 1), Point(1, 2), Brick::Stars{Point(0,0)}),
        Brick(Size(1, 1), Point(2, 2), Brick::Stars{}),
        Brick(Size(1, 1), Point(3, 2), Brick::Stars{})
              })
{
    empty[0] = Point(0, 0);
    empty[1] = Point(4, 2);
}

Datum::~Datum()
{

}


quint64 Datum::hash() const
{//Вычислять хэш будетм как XOR всех данных
    return static_cast<quint64>(empty[0].x()) ^ (static_cast<quint64>(empty[0].y()) << 2) ^ (static_cast<quint64>(empty[1].x()) << 4) ^ (static_cast<quint64>(empty[1].y()) << 8)
            ^ (static_cast<quint64>(at(0).pos().x()) << 10) ^ (static_cast<quint64>(at(1).pos().x()) << 12) ^ (static_cast<quint64>(at(2).pos().x()) << 14) ^ (static_cast<quint64>(at(3).pos().x()) << 16)
            ^ (static_cast<quint64>(at(4).pos().x()) << 18) ^ (static_cast<quint64>(at(5).pos().x()) << 20) ^ (static_cast<quint64>(at(6).pos().x()) << 22) ^ (static_cast<quint64>(at(7).pos().x()) << 24)
            ^ (static_cast<quint64>(at(8).pos().x()) << 26) ^ (static_cast<quint64>(at(0).pos().y()) << 28) ^ (static_cast<quint64>(at(1).pos().y()) << 30) ^ (static_cast<quint64>(at(2).pos().y()) << 32)
            ^ (static_cast<quint64>(at(3).pos().y()) << 38) ^ (static_cast<quint64>(at(4).pos().y()) << 40) ^ (static_cast<quint64>(at(5).pos().y()) << 42) ^ (static_cast<quint64>(at(6).pos().y()) << 44)
            ^ (static_cast<quint64>(at(7).pos().y()) << 34) ^ (static_cast<quint64>(at(8).pos().y()) << 36);
}

bool Datum::operator==(const Datum& op) const
{//Одинаковыми считаем положения, в которых все положения элементов совпадают (как по текущему так и по исходному положению)
    return empty[0] == op.empty[0] && empty[1] == op.empty[1] && at(0).pos() == op.at(0).pos() && op.at(1).pos() == at(1).pos()
             && op.at(2).pos() == at(2).pos() && op.at(3).pos() == at(3).pos() && op.at(4).pos() == at(4).pos() && op.at(5).pos() == at(5).pos()
             && op.at(6).pos() == at(6).pos() && op.at(7).pos() == at(7).pos() && op.at(8).pos() == at(8).pos();
}

bool Datum::operator<(const Datum& a) const
{
    return this->hash() < a.hash();
}

int Datum::result() const
{
    int val(0);
    for (const Brick& i: *this) {
        for (const Point& p: i.stars()) {
            if (p.y() + i.pos().y() == 1) {
                ++val;
            }
        }
    }
    return val;
}

std::stack<Datum::Step> Datum::mobilities() const
{
    //Функция вычисляет возможные подвижности, доступные для данного поля
    const Point& a(empty[0]);
    const Point& b(empty[1]);
    std::stack<Step> rez;
    //Для случаев, когда пустые поля находятся на смежных клетках (может быть сдвинута сдвоеная ячейка
    if (a.x() == b.x() && std::abs(a.y() - b.y()) == 1) {
        Point p(a.x() - 1, std::min(a.y(), b.y()));
        auto predicate([&p](const Brick& b)->bool {
            return b.pos() == p && b.size().height() == 2;
        });
        //Проверка на движение вправо сдвоеного вертикального блока
        if (p.x() >= 0) {
            const const_iterator brit(std::find_if(this->begin(), this->end(), predicate));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Right));
            }
        }
        p.rx() += 2;
        //Проверка на движение влево
        if (p.x() < field.width()) {
            const const_iterator brit(std::find_if(this->begin(), this->end(), predicate));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Left));
            }
        }
    }
    //Проверка на движение гарризонтального блока
    if (a.y() == b.y() && std::abs(a.x() - b.x()) == 1) {
        Point p(std::min(a.x(), b.x()), a.y() - 1);
        auto predicate([&p](const Brick& b)->bool {
            return b.pos() == p && b.size().width() == 2;
        });
        //На движение вниз
        if (p.y() >= 0) {
            const const_iterator brit(std::find_if(this->begin(), this->end(), predicate));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Bottom));
            }
        }
        p.ry() += 2;
        //На движение вверх
        if (p.y() < field.height()){
            const const_iterator brit(std::find_if(this->begin(), this->end(), predicate));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Top));
            }
        }
    }
    //Обнаружение подвижностей, связанных со смещением единственного пустого поля
    for (const Point& p : empty) {
        if (p.y() > 0) {
            //Вниз
            const const_iterator brit(std::find_if(this->begin(), this->end(), [&p](const Brick& b)->bool {
                return b.pos().y() == p.y() - b.size().height() && b.pos().x() == p.x() && b.size().width() == 1;
            }));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Bottom));
            }
        }
        if (p.y() < field.height() - 1) {
            //Вверх
            const const_iterator brit(std::find_if(this->begin(), this->end(), [&p](const Brick& b)->bool {
                return b.pos().y() == p.y() + 1 && b.pos().x() == p.x() && b.size().width() == 1;
            }));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Top));
            }
        }
        if (p.x() > 0) {
            //Вправо
            const const_iterator brit(std::find_if(this->begin(), this->end(), [&p](const Brick& b)->bool {
                return b.pos().x() == p.x() - b.size().width() && b.pos().y() == p.y() && b.size().height() == 1;
            }));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Right));
            }
        }
        if (p.x() < field.width() - 1) {
            //Влево
            const const_iterator brit(std::find_if(this->begin(), this->end(), [&p](const Brick& b)->bool {
                return b.pos().x() == p.x() + 1 && b.pos().y() == p.y() && b.size().height() == 1;
            }));
            if (brit != this->end()) {
                rez.push(Step(brit - this->begin(), Left));
            }
        }
    }
    return rez;
}

Datum Datum::realize(const Datum::Step& s) const
{
    //Порождение нового расположения по исходному и шагу
    Datum r(*this);
    r.way.push(s);
    bool changed(false);
    Brick& i(r[s.id]);
    switch (s.d) {
    case Left:
        //Тест на корректность заданного шага. логика такова:
        //Попытаться найти пустое поле, положение которого будет изменено в результате заданного движения.
        //Если такое пустое поле (из двух) не обнаружено, то перемещение не возможно и задано не корректно.
        for(Point& p : r.empty) {
            if (p.y() >= i.pos().y() && p.y() < i.pos().y() + i.size().height() && p.x() == i.pos().x() - 1) {
                changed = true;
                p.rx() += i.size().width();
            }
        }
        //Отладочное завершение программы в случае, если перемешение не кооректно
        assert(changed);
        //Иначе данное перемещение осуществляется
        i.moveLeft();
        break;
    case Right:
        //Далее аналогично описаны пермещения в остальных направленияхю.
        for(Point& p : r.empty) {
            if (p.y() >= i.pos().y() && p.y() < i.pos().y() + i.size().height() && p.x() == i.pos().x() + i.size().width()) {
                changed = true;
                p.rx() -= i.size().width();
            }
        }
        assert(changed);
        i.moveRight();
        break;
    case Bottom:
        for(Point& p : r.empty) {
            if (p.x() >= i.pos().x() && p.x() < i.pos().x() + i.size().width() && p.y() == i.pos().y() + i.size().height()) {
                changed = true;
                p.ry() -= i.size().height();
            }
        }
        assert(changed);
        i.moveBottom();
        break;
    case Top:
        for(Point& p : r.empty) {
            if (p.x() >= i.pos().x() && p.x() < i.pos().x() + i.size().width() && p.y() == i.pos().y() - 1) {
                changed = true;
                p.ry() += i.size().height();
            }
        }
        assert(changed);
        i.moveTop();
        break;
    }
    return r;
}

//Оценка перспективности состояния оценивается как длинна пути, приведшего к данному решению и количеству звёзд на центральной линии
int Datum::valuation() const
{
    return way.size() + result();
}

//непосредственно решение существует в двух реализациях. Изначально была реализована упрощенная и наиболее эфективная версия, в данной версии
//текущее множество возможных продолжений сортируется по значению оценочной функции, к дальнейшему рассмотрению принимается наиболее перспективное продолжение
Datum::Solve Datum::solve()
{
    //"первый" спискок состояний. Хранит все возможные продолжения развития алгоритм
    std::multimap<int, Datum> first;
    //Положим в него начальное состояние. Оценочная функция для него равна 0 (не сделано ни одного шага и ни одна звезда не расположена на главной диагонали
    first.insert(std::pair<int, Datum>(0, Datum()));

    //"Второй" списк содержит рассмотренные варианты. В начале он пуст.
    std::set<Datum> second;

    //Теперь пока не исследованы все возможные продоложения
    while (!first.empty()
           //И не найдено решение
         && first.begin()->second.result() < 5) {
        //Возьмём наиболее перспективное решение
        const Datum& cur(first.begin()->second);

        //если оно ещё не рассмотрено (за счет того, что используются система хранения set, поиск происходит со сложностью log(N))
        if (second.find(cur) == second.end()) {
            //Найдём все его возможные подвижности
            std::stack<Step> steps(cur.mobilities());
            //Последовательно рассмотрим каждое
            while (!steps.empty()) {
                const Step& s(steps.top());
                //Построем данное продолжение
                const Datum next(cur.realize(s));
                //Если оно ещё не рассмотрено
                if (second.find(next) == second.end()) {
                    //примим его к рассмотрению
                    first.insert(std::pair<int, Datum>(next.valuation(), next));
                }
                steps.pop();
            }
            //Кладём рассмотреное состояние во второй список
            second.insert(cur);
        }
        //и убираем из первого
        first.erase(first.begin());
    }

    //Алгоритм построен так, что стэк перемещений копится в обратном порядке, по этому перевернём его.
    Datum::Solve s;
    while (!first.begin()->second.way.empty()) {
        s.push(first.begin()->second.way.top());
        first.begin()->second.way.pop();
    }
    //Теперь решение готово.
    return s;
}
