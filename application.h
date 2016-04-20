#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class GUI;
class Datum;

//Кдасс QApplication используется как агрегатор точек вхождения в глобальные методы. Дополним их множество функцией, приводящей единственный виджет в заданное состояние
class Application : public QApplication
{
    GUI* const g;
public:
    Application(int argc, char** argv);
    ~Application();

    static void showDatum(const Datum&);
};

#endif // APPLICATION_H
