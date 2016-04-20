#include "application.h"
#include "kernel/datum.h"

int main(int argc, char *argv[])
{
    //Точка входа в програму
    // просто создаёт приложение,
    Application app(argc, argv);

    // получает решение
    Datum::Solve s(Datum::solve());
    Datum d;
    //Создаёт стек состояний для решения
    while (!s.empty()) {
        Application::showDatum(d);
        d = d.realize(s.top());
        s.pop();
    }
    // и отображает его
    Application::showDatum(d);

    //после чего можно закончить выполнение прогемы
    app.quit();
    return 0;
}
