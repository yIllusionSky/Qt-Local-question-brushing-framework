#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include<QStandardItem>
QT_BEGIN_NAMESPACE
namespace Ui { class FileWeiget; }
QT_END_NAMESPACE
class file : public QWidget
{
    Q_OBJECT
public:
    explicit file(QWidget *parent = nullptr);
    ~file();
signals:
    void parentDo();

private slots:

    void on_topicView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_pressed();

private:
    Ui::FileWeiget *ui;
    QStandardItemModel *model;
    void AddFile(QStandardItem* item,QString path);


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // FILE_H
