#ifndef GUI_H
#define GUI_H

#include "kernel/datum.h"
#include <QWidget>

class QTimer;

//Класс GUI предоставляет функциональноссть для отображения текущего решения
class GUI : public QWidget
{
    Q_OBJECT

    //Тут будет лежать текущая позиция, имеющая место в ходе анимации
    Datum my;
    //Это расположение плиток в направлении которого меняется анимация
    Datum dest;
    //Просто таймер, отвечающий за вызов перерисовки вовремя анимации
    QTimer* const t;

    //Расчитываемые в ходе изменения размера окна параметры. Размер одной плитки поля и размер начала рисования поля
    QSizeF itemSize;
    QPointF p0;

    //Отступ между плитками пятнашек
    static const int margin = 5;
    //Размер пятнашки с учётом отступа
    QSizeF brickSize;

    //Картинка фона
    QPixmap img;
    //и звёздочки. так же меняются в ходе изменения размера
    QPixmap star;

    //Подписываемся следить за событиями
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent*);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
public:
    GUI(QWidget* parent = 0);
    ~GUI();

    //Запускает анимацию перестроения состояния из текущего к принимаемому
    void setDatum(const Datum&);

signals:
    //Дополнением стандартное множество сигналов сигналом о закрытии окна и о нажатии любой клавиши
    void keyPressed();
    void closed();
};

#endif // GUI_H
