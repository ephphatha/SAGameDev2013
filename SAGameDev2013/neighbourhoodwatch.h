#ifndef NEIGHBOURHOODWATCH_H
#define NEIGHBOURHOODWATCH_H

#include <QtWidgets/QMainWindow>
#include "ui_neighbourhoodwatch.h"

class NeighbourhoodWatch : public QMainWindow
{
    Q_OBJECT

public:
    NeighbourhoodWatch(QWidget *parent = 0);
    ~NeighbourhoodWatch();

private:
    Ui::NeighbourhoodWatchClass ui;
};

#endif // NEIGHBOURHOODWATCH_H
