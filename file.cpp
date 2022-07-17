#include "file.h"
#include"ui_file.h"
#include<QDir>
#include<QStandardItem>
#include<QIcon>
#include<database.h>
#include<QMessageBox>
file::file(QWidget *parent)
    : QWidget(parent),ui(new Ui::FileWeiget)
{
    ui->setupUi(this);
this->setFixedSize(450,630);
    QDir dir(QApplication::applicationDirPath()+"/Save");
    //指定AllDirs不对目录名应用过滤器
    dir.setFilter(QDir::AllDirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QStringList filter;
    filter<<"*.db";
    dir.setNameFilters(filter);
    model = new QStandardItemModel;
    QStandardItem *tree;
    qsizetype count=(QApplication::applicationDirPath()+"/Save/").size();
    ui->topicView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(auto info:dir.entryInfoList())
    {
        if(info.fileName() == "." || info.fileName() == "..")
        {
            continue;
        }
        if(info.isDir())
        {
            tree=new QStandardItem(QIcon(":/Image/path.png"),info.fileName());
            tree->setData(info.absoluteFilePath().mid(count),Qt::UserRole+1);
            AddFile(tree,info.absoluteFilePath());
        }
        else
        {
            tree=new QStandardItem(QIcon(":/Image/file.png"),info.baseName());
            tree->setData(info.absoluteFilePath().mid(count),Qt::UserRole+1);
            if(info.baseName()=="!!!ErrorTopic")
            {
                tree->setForeground(QBrush(QColor(255, 0, 0)));
            }

            if(info.baseName()=="!!!RemoveDuplicates")
            {
                tree->setForeground(QBrush(QColor(0, 0, 255)));
            }
            if(info.baseName()=="!!!default")
            {
                tree->setForeground(QBrush(QColor(0, 255,0)));
            }
        }
        model->appendRow(tree);
    }

    ui->topicView->setHeaderHidden(true);
    ui->topicView->setModel(model);
}

file::~file()
{
    model->clear();
    delete model;
    delete ui;
}

void file::AddFile(QStandardItem *item, QString path)
{
    qsizetype count=(QApplication::applicationDirPath()+"/Save/").size();
    QStandardItem *treeSon;
    QDir dir(path);
    //指定AllDirs不对目录名应用过滤器
    dir.setFilter(QDir::AllDirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QStringList filter;
    filter<<"*.db";
    dir.setNameFilters(filter);
    for(auto info:dir.entryInfoList())
    {
        if(info.fileName() == "." || info.fileName() == "..")
        {
            continue;
        }

        if(info.isDir())
        {
            treeSon=new QStandardItem(QIcon(":/Image/path.png"),info.fileName());
            treeSon->setData(info.absoluteFilePath().mid(count),Qt::UserRole+1);

            AddFile(treeSon,info.absoluteFilePath());
        }
        else
        {
            treeSon=new QStandardItem(QIcon(":/Image/file.png"),info.baseName());
            treeSon->setData(info.absoluteFilePath().mid(count),Qt::UserRole+1);
        }
        item->appendRow(treeSon);
    }
}

void file::closeEvent(QCloseEvent *event)
{
    emit parentDo();
}

void file::on_topicView_doubleClicked(const QModelIndex &index)
{
    QString data=index.data(Qt::UserRole+1).toString();
    QFileInfo fileInfo(QApplication::applicationDirPath()+"/Save/"+data);
    if(fileInfo.isFile())
    {
        DataBase* base=DataBase::get(fileInfo.baseName());
        QMessageBox::information(nullptr,"题目名称",base->selectInfo(1));
    }
}


void file::on_pushButton_2_pressed()
{
    close();
}

