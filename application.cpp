#include "application.h"
#include "gui.h"
#include <QEventLoop>
#include <QTimer>

Application::Application(int argc, char **argv)
    : QApplication(argc, argv)
    , g(new GUI)
{
    connect(static_cast<Application*>(qApp)->g, &GUI::closed, this, &Application::quit);
}

Application::~Application()
{
    delete g;
}

void Application::showBarley()
{
    g->show();
    g->setWindowTitle("barley-brick | Breadth-first-search");
}

void Application::showDatum(const Datum& d, bool withTimeout)
{
    if (static_cast<Application*>(qApp)->g->isHidden()) {
        return;
    }

    static_cast<Application*>(qApp)->g->setDatum(d);
    QEventLoop loop;

    QTimer* autoclick(new QTimer(&loop));
    connect(static_cast<Application*>(qApp)->g, &GUI::keyPressed, &loop, &QEventLoop::quit);
    connect(autoclick, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(static_cast<Application*>(qApp)->g, &GUI::closed, &loop, &QEventLoop::quit);

    if (withTimeout) {
        autoclick->start(150);
    }
    loop.exec();
}
