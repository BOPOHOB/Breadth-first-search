#include "application.h"
#include "gui.h"
#include <QEventLoop>

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
    , g(new GUI)
{
    g->show();
    g->setWindowTitle("Пятнашки ");
    connect(static_cast<Application*>(qApp)->g, &GUI::closed, this, &Application::quit);
}

Application::~Application()
{
    delete g;
}

void Application::showDatum(const Datum& d)
{
    if (static_cast<Application*>(qApp)->g->isHidden()) {
        return;
    }

    static_cast<Application*>(qApp)->g->setDatum(d);
    QEventLoop loop;
    connect(static_cast<Application*>(qApp)->g, &GUI::keyPressed, &loop, &QEventLoop::quit);
    connect(static_cast<Application*>(qApp)->g, &GUI::closed, &loop, &QEventLoop::quit);

    loop.exec();
}
