#include "gui.h"

#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <thread>

GUI::GUI(QWidget *parent)
    : QWidget(parent)
    , t(new QTimer(this))
{
    QPalette ppp(this->palette());
    ppp.setColor(QPalette::Background, Qt::white);
    this->setPalette(ppp);
    //Заранее настроим анимацию на 25 кадров в секунду
    t->setInterval(40);
    //и определим поведение по таймеру
    connect(t, &QTimer::timeout, [this](){
        int i(0);
        //Найдём несовпадающую плитку
        while(i < my.size() && my[i].pos() == dest[i].pos()) {
            ++i;
        }
        //Если не нашлась - останавливаем анимацию
        if (i == my.size()) {
            this->t->stop();
            return;
        }
        //иначе сместим её в направлении конечного положения
        while(i < my.size()) {
            if (my[i].pos() != dest[i].pos()) {
                Point& a(my[i].rpos());
                Point& b(dest[i].rpos());
                if ((a - b).manhattanLength() > 0.01) {
                    a -= (a - b) / 2.0;
                } else {
                    a = b;
                }
            }
            ++i;
        }
        //и пререрисуем
        this->repaint();
    });
}

GUI::~GUI()
{

}

void GUI::resizeEvent(QResizeEvent *)
{
    //Когда сцена меняет размер необходимо пересчитать размеры плитки и картинок.
    if (width() / Datum::field.width() > height() / Datum::field.height()) {
        itemSize.setWidth(height() / Datum::field.height());
        p0 = QPoint((width() - itemSize.width() * Datum::field.width()) / 2.0, 0);
    } else {
        itemSize.setWidth(width() / Datum::field.width());
        p0 = QPoint(0, (height() - itemSize.width() * Datum::field.height()) / 2.0);
    }
    itemSize.setHeight(itemSize.width());

    brickSize = QSizeF(itemSize.width() - 2 * margin, itemSize.height() - 2 * margin);

    static const QPixmap img0(":/1028079.jpeg");
    static const QPixmap star0(":/star.png");
    img = img0.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    star = star0.scaled(brickSize.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void GUI::setDatum(const Datum& hyest)
{
    //При получении нового положения, установким его как целевое
    dest = hyest;
    //И запустим анимацию
    t->start();
}

void GUI::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.translate(p0);

    //Отрисовка плиток
    for (const Brick& i : my) {
        //Нарисовали саму плитку
        QRectF r(QRectF(QPointF(i.pos().x() * itemSize.width() + margin,
                                i.pos().y() * itemSize.height() + margin),
                        QSizeF(itemSize.width() * i.size().width() - margin * 2,
                               itemSize.height() * i.size().height() - margin * 2)));
        p.drawPixmap(r, img, r);
        //Нарисовали звёздочки на ней
        for (const Point& j: i.stars()) {
            p.drawPixmap(r.topLeft() + QPointF(j.x() * itemSize.width(), j.y() * itemSize.height()), star);
        }
    }
    if (false) {
        for (const Point& i: my.empyFields()) {
            p.drawEllipse(QRectF(QPointF(i.x() * itemSize.width() + margin,
                                         i.y() * itemSize.height() + margin),
                                 brickSize));
        }
    }
}


void GUI::keyPressEvent(QKeyEvent*)
{
    emit keyPressed();
}

void GUI::closeEvent(QCloseEvent*)
{
    emit closed();
}
