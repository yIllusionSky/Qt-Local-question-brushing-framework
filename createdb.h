#ifndef CREATEDB_H
#define CREATEDB_H

#include <QWidget>

namespace Ui {
class createDB;
}

class createDB : public QWidget
{
    Q_OBJECT

public:
    explicit createDB(QWidget *parent = nullptr);
    ~createDB();
signals:
    void parentDo();

private slots:
    void on_pushButton_3_pressed();

    void on_pushButton_2_pressed();

private:
    Ui::createDB *ui;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CREATEDB_H
