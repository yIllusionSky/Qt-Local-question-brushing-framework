#ifndef SHOWINFORMATION_H
#define SHOWINFORMATION_H

#include <QWidget>
#include<database.h>
QT_BEGIN_NAMESPACE
namespace Ui { class showInformation; }
QT_END_NAMESPACE
class ShowInformation : public QWidget
{
    Q_OBJECT
public:
    explicit ShowInformation(QWidget *parent);
    void setShowInformation(QList<ProblemData>* problem,QList<quint8>* ans,QList<quint32>* id,QList<bool>* theRecord,quint32 theIndex);
    void setShowInformation(QList<ProblemData>* problem,QList<quint32>* id,QList<bool>* theRecord,quint32 theIndex);
    ~ShowInformation();
signals:
    void parentDo(quint32 theIndex,quint8 whatHappen);

private slots:
    void on_pushButton_4_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_5_pressed();
    void chooseId();
private:

    void setMiniInformation();
    bool tensorType;
    Ui::showInformation* ui;
    quint32 theId;
    QList<ProblemData>* tproblem;
    QList<quint8>* tans;
    QList<quint32>* tid;
    QList<bool>* tRecord;
    quint32 tIndex;
    bool canChange;
    quint8 changeType;
    quint8 justAnswer;


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // SHOWINFORMATION_H
