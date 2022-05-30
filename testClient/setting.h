#ifndef SETSIGNAL_H
#define SETSIGNAL_H

#include <QWidget>

namespace Ui {
class setSignal;
}

class setSignal : public QWidget
{
    Q_OBJECT

public:
    explicit setSignal(QWidget *parent = nullptr);
    ~setSignal();

private slots:
    void on_pbtClear_clicked();

    void on_pbtReturn_clicked();

    void on_pbtOk_clicked();
signals:
    void signalToSend(int type,QString signate);
private:
    Ui::setSignal *ui;
};

#endif // SETSIGNAL_H
