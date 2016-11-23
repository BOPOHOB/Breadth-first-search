#include "application.h"
#include "kernel/datum.h"

#include <QLabel>
#include <QMovie>
#include <QDir>

#include <future>
#include <random>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    QEventLoop loop;

    auto s(std::async(std::launch::async, [&loop]{ Datum::Solve solve(Datum::solve()); if (loop.isRunning()) { loop.quit(); } return std::move(solve); }));

    QLabel splash;
    splash.setMovie(new QMovie(([](){
        static const QString basePath(":/splash/busy/");
        const QStringList files(QDir(basePath).entryList(QStringList() << "*.gif"));

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> d(0,files.size() - 1);
        const QString& result(files.at(d(gen)));
        return basePath + result;
    })()));
    splash.movie()->start();
    splash.show();
    splash.setWindowTitle("computing. . .");

    if (s.wait_until(std::chrono::system_clock::now()) != std::future_status::ready) {
        loop.exec();
    }
    splash.hide();
    app.showBarley();

    Datum::Solve solve(s.get());

    Datum d;
    while (!solve.empty()) {
        Application::showDatum(d);
        d = d.realize(solve.top());
        solve.pop();
    }
    Application::showDatum(d, false);

    app.quit();
    return 0;
}
