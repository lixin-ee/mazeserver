#ifndef MAZESERVER_H
#define MAZESERVER_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QFile>
#include<QVector>
#include<QFileInfo>
#include<QTableView>
#include<QStandardItemModel>
QT_BEGIN_NAMESPACE
namespace Ui { class mazeserver; }
QT_END_NAMESPACE

struct player
{
     char name[20];int classic_number,entertain_number,devil_number,classic_vic,entertain_egg,
    devil_vic;double classic_rate,devil_rate;
    player(QString _name="",int _calssic_number=0,int _entertain_number=0,int _devil_number=0,int _classic_vic=0,
           int _entertain_egg=0,int _devil_vic=0,double _classic_rate=0,double _devil_rate=0)
    {
         qstrcpy(name,_name.toStdString().c_str());
         classic_number=_calssic_number;entertain_number=_entertain_number;devil_number=_devil_number;
         classic_vic=_classic_vic;entertain_egg=_entertain_egg;devil_vic=_devil_vic;classic_rate=_classic_rate;
         devil_rate=_devil_rate;
    }
};
class mazeserver : public QWidget
{
    Q_OBJECT

public:
    mazeserver(QWidget *parent = nullptr);
    ~mazeserver();
public slots:
    void onnewconnect();
    void onSocketReadyRead();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    QVector<player*> client;
    QVector<int> clientpace;
    QTcpServer* server;
    QVector <QTcpSocket*> socket;
    QFile file;
    QFile file1;
    QFile file2;
    QString filename;
    int filesize;
    int sendsize=0;
    Ui::mazeserver *ui;
    QTableView* table;
    QStandardItemModel* itemmodel;
    bool wipe=false;
};
#endif // MAZESERVER_H
