#ifndef FORM_H
#define FORM_H

#include <QWidget>
namespace Ui { class Form; }

class form : public QWidget
{
    Q_OBJECT
public:
    explicit form(QWidget *parent = nullptr);
    ~form();

signals:
    void ChangeType(int type);


public slots:
    void eventHappend();

private:
    Ui::Form* ui;
    int chooseType;

    // QWidget interface
private slots:
    void on_pushButton_pressed();
};

#endif // FORM_H
