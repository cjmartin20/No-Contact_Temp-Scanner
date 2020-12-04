#ifndef TEMPWINDOW_H
#define TEMPWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TempWindow; }
QT_END_NAMESPACE

class TempWindow : public QMainWindow
{
    Q_OBJECT

public:
    TempWindow(QWidget *parent = nullptr);
    ~TempWindow();

private:
    Ui::TempWindow *ui;
};
#endif // TEMPWINDOW_H
