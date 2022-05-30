#ifndef SET_H
#define SET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class set;
}

class set : public QWidget
{
    Q_OBJECT

public:
    explicit set(QWidget *parent = nullptr);
    ~set();

    //鼠标事件，按下、释放、移动事件
    void mousePressEvent(QMouseEvent *eve);
    void mouseReleaseEvent(QMouseEvent* eve);
    void mouseMoveEvent(QMouseEvent* eve);
    bool mousePress;    //鼠标是否点击
    QPoint movePoint;  //鼠标移动的距离
    int GetStatus();
    void SetStatus(int temp);

signals:
    void siganlMessageToSend(int type,QString signate);

private slots:
    void on_pbtPasswod_clicked();

    void on_pbtNickName_clicked();

    void on_pbtSignate_clicked();

    void on_pbtClear_clicked();

    void on_pbtReturn_clicked();

private:
    Ui::set *ui;
    int status ;//窗口是否打开，在关闭主页面时，自动关闭全部窗口
};

#endif // SET_H
