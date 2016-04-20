#ifndef BRICK_H
#define BRICK_H

#include <vector>

/*
Для обеспечения возможности удобной отвязки от ядра Qt введём синонимы для тревиальных структур,
описывающих точку и размер элемента. Это обеспечит при необходимости, удобную переадресацию
сборщиха от реализации на основе Qt к независимому коду.
*/
#include <QPoint>
#include <QSize>
typedef QPointF Point;
typedef QSize Size;

class Brick
{
public:
    //Положение звёзд будем описыват точками в локальной системе кординат, связанной с фишкой.
    typedef std::vector<Point> Stars;
private:
    //Каждая фишка описывается собственными размерами, положением и расположением звёзд
    Point p;
    Size s;
    Stars star;
public:
    Brick();
    Brick(const Size&, const Point&, const Stars& star);
    ~Brick();

    //Далее функции доступа
    const Size& size() const { return s; }
    const Point& pos() const { return p; }
    Point& rpos() { return p; }
    const Stars& stars() const { return star; }

    //И функции движения
    void move(const Point&);
    void moveLeft();
    void moveRight();
    void moveBottom();
    void moveTop();
};

#endif // BRICK_H
