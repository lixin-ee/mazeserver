#include "mazeserver.h"
#include "ui_mazeserver.h"
#include<QDateTime>
#include<QHeaderView>
#define port 25565
#define tsize 4*1024
mazeserver::mazeserver(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mazeserver)
{
    ui->setupUi(this);
    server=new QTcpServer(this);
    server->listen(QHostAddress::Any,port);
    QObject::connect(server,SIGNAL(newConnection()),this,SLOT(onnewconnect()));
    file.setFileName("mazeplayer.dat");
    file1.setFileName("rank.dat");
    file2.setFileName("rizhi.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
    file.open(QIODevice::WriteOnly);
    file.close();
    }
    else
    {   player tempplayer;
        while(file.read((char*)&tempplayer,sizeof(player)))
        {
         client.append(new player(tempplayer));
        }
        file.close();
    }
    if(!file1.open(QIODevice::ReadOnly))
    {
    file1.open(QIODevice::WriteOnly);
    file1.close();
    }
    else file1.close();
    if(!file2.open(QIODevice::ReadOnly))
    {
    file2.open(QIODevice::WriteOnly);
    file2.close();
    }
    else file2.close();
    table=new QTableView(this);
    itemmodel=new QStandardItemModel(10,10);
    table->setModel(itemmodel);
    table->resize(1021,551);
    ui->plainTextEdit->setReadOnly(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->hide();
}
void mazeserver::onnewconnect()
{
    socket.append(server->nextPendingConnection());
    clientpace.append(0);
    connect(socket[socket.size()-1], SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}
void mazeserver::onSocketReadyRead()
{
   QTcpSocket* tempsocket=qobject_cast<QTcpSocket*>(sender());
   QByteArray tempbyte=tempsocket->readAll();
   int tempcount=0;
   for (auto& x:socket)
   {
       if(tempsocket==x)
       {
           break;
       }
       tempcount++;
   }
       switch (clientpace[tempcount])
       {
      case 0:
       {
           QString tempstr=tempbyte;
           if(tempstr.startsWith("<")&&tempstr.endsWith(">"))
           {
               QStringList list=tempstr.split(QRegExp("[<>]"));
               QString tempname=list[1];
               QString info;
               int count1=0;
               for (auto& x:client)
               {
                   if(strcmp(tempname.toUtf8().data(),x->name)==0)
                   {
                       info="注册失败";
                       tempsocket->write(info.toUtf8());
                       qDebug()<<clientpace.size()<<socket.size();
                       break;
                   }
                   count1++;
               }
               qDebug()<<"utf"<<tempname.toUtf8().data();
               if(count1==client.size())
               {
                   info="注册成功";
                   tempsocket->write(info.toUtf8());
                   client.append(new player(tempname));
                   file.open(QIODevice::Append);
                   player tempplayer=player(tempname);
                   qDebug()<<tempplayer.name;
                   qDebug()<<client[client.size()-1]->name;
                   file.write((char*)&tempplayer,sizeof(player));
                   file.close();
                   qDebug()<<clientpace.size()<<socket.size();
                   QDateTime current_date_time =QDateTime::currentDateTime();
                   QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd")+'\n';
                   QString rz=tempname+"注册成功    "+current_date;
                   file2.open(QIODevice::Append);
                   file2.write(rz.toUtf8());
                   file2.close();
                   on_pushButton_3_clicked();
               }
           qDebug()<<clientpace.size()<<socket.size();

           }
           if(tempstr.startsWith("修改昵称#"))
           {
               QStringList list=tempstr.split('#');
               QString tempname=list[1];
               QString tempname1=list[2];
               qDebug()<<"xiugai "<<list.size()<<list[1]<<list[2];
               QString info;
               int count1=0;
               for (auto& x:client)
               {
                   if(strcmp(tempname1.toUtf8().data(),x->name)==0&&strcmp(tempname.toUtf8().data(),x->name)!=0)
                   {
                       info="修改失败";
                       tempsocket->write(info.toUtf8());
                       break;
                   }
                   count1++;
               }
               if(count1==client.size())
               {
                   info="修改成功";
                   tempsocket->write(info.toUtf8());
                   int count2=0;
                   for (auto x:client)
                   {
                    if(strcmp(x->name,tempname.toUtf8().data())==0)
                    {    std::string str1=tempname1.toStdString();
                         qstrcpy(x->name,str1.c_str());
                         file.open(QIODevice::ReadWrite);
                         file.seek(count2*sizeof(player));
                         file.write((char*)x,sizeof(player));
                         file.close();
                         QDateTime current_date_time =QDateTime::currentDateTime();
                         QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                         QString rz=tempname+"修改昵称为"+tempname1+"   "+current_date+'\n';
                         file2.open(QIODevice::Append);
                         file2.write(rz.toUtf8());
                         file2.close();
                         on_pushButton_3_clicked();
                    }
                    count2++;
                   }
               }

           }
           if(tempstr=="发送结构体")
           {
              clientpace[tempcount]=1;
              QString tempstr3="准备接收";
              tempsocket->write(tempstr3.toUtf8());
           }
           if(tempstr=="结束")
           {   qDebug()<<tempstr;
               clientpace.erase(clientpace.begin()+tempcount);
               socket.erase(socket.begin()+tempcount);
               tempsocket->close();
               tempsocket->deleteLater();
           }
           break;
       }
        case 1:
       {
           player* tempplayer=new player;
           memcpy(tempplayer,tempbyte.data(),sizeof(struct player));
           int count3=0;
           for (auto& x:client)
           {
               if(strcmp(tempplayer->name,x->name)==0)
               {
                  delete x;
                  x=tempplayer;
                  file.open(QIODevice::ReadWrite);
                  file.seek(count3*sizeof(player));
                  file.write((char*)tempplayer,sizeof(player));
                  file.close();
                  QDateTime current_date_time =QDateTime::currentDateTime();
                  QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                  QString rz=QString (x->name)+"请求排行榜   "+current_date+'\n';
                  file2.open(QIODevice::Append);
                  file2.write(rz.toUtf8());
                  file2.close();
                  on_pushButton_3_clicked();
                  break;
               }
               count3++;
           }
           QVector<player*> temparr;
           for(auto& x:client)
           {
               temparr.append(x);
           }
           file1.open(QIODevice::WriteOnly);
           for (int i = 1; i < temparr.size(); i++)
               {
                   for (int j = i - 1; j >= 0; j--)
                   {
                       if (temparr[j]->entertain_egg < temparr[j + 1]->entertain_egg)
                       {
                           player* temp;
                           temp = temparr[j];
                           temparr[j] = temparr[j + 1];
                           temparr[j + 1] = temp;
                       }
                       else break;
                   }
               }
           for (auto& x:temparr)
           {
               file1.write((char*)x,sizeof(player));
           }
           for (int i = 1; i < temparr.size(); i++)
               {
                   for (int j = i - 1; j >= 0; j--)
                   {
                       if (temparr[j]->devil_rate < temparr[j + 1]->devil_rate)
                       {
                           player* temp;
                           temp = temparr[j];
                           temparr[j] = temparr[j + 1];
                           temparr[j + 1] = temp;
                       }
                       else break;
                   }
               }
           for (auto& x:temparr)
           {
               file1.write((char*)x,sizeof(player));
           }
           for (int i = 1; i < temparr.size(); i++)
               {
                   for (int j = i - 1; j >= 0; j--)
                   {
                       if ((temparr[j]->classic_number+temparr[j]->devil_number+temparr[j]->entertain_number)
                               < (temparr[j+1]->classic_number+temparr[j+1]->devil_number+temparr[j+1]->entertain_number))
                       {
                           player* temp;
                           temp = temparr[j];
                           temparr[j] = temparr[j + 1];
                           temparr[j + 1] = temp;
                       }
                       else break;
                   }
               }
           for (auto& x:temparr)
           {
               file1.write((char*)x,sizeof(player));
           }
           file1.close();
           QFileInfo info;
           info.setFile(file1);
           filename=info.fileName();
           filesize=info.size();
           //发送包头
           QString tempstr2=filename+"#"+QString::number(filesize,10);
           tempsocket->write(tempstr2.toUtf8());
           clientpace[tempcount]=2;
           break;
       }
       case 2:
       {
           QString tempstr1=tempbyte;
           if(tempstr1=="接收包头成功")
           {
               sendsize=0;
             while(sendsize<filesize)
             {
             file1.open(QIODevice::ReadOnly);
             tempsocket->write(file1.read(tsize));
             sendsize+=tsize;
             } //发送文件
             clientpace[tempcount]=3;
           }

           break;
       }
       case 3:
       {
           QString tempstr=tempbyte;
           qDebug()<<tempstr;
           if(tempstr=="文件接收完毕")
           {
               file1.close();
             clientpace.erase(clientpace.begin()+tempcount);
             socket.erase(socket.begin()+tempcount);
              tempsocket->close();
              tempsocket->deleteLater();
           }
           break;
       }
       }
}
mazeserver::~mazeserver()
{
    delete ui;
    for (auto x:client)
    {
    delete x;
    }
}


void mazeserver::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    table->hide();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->show();

    for (auto& x:client)
    {
     ui->plainTextEdit->appendPlainText("昵称:"+ QString(x->name)+"   魔鬼场次:"+QString::number( x->devil_number)+"   魔鬼胜率:"+QString::number( x->devil_rate)
                                        + "   魔鬼胜场:"+QString::number(x->devil_vic)
     +"   经典场次:"+QString::number(x->classic_number)+"   经典胜率:"+QString::number(x->classic_rate)+"   经典胜场:"+QString::number(x->classic_vic)
     +"   娱乐场次:"+QString::number(x->entertain_number)+"   娱乐彩蛋数:"+QString::number(x->entertain_egg)+'\n');
    }
}

void mazeserver::on_pushButton_2_clicked()
{
    ui->plainTextEdit->hide();
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    table->show();
    itemmodel->setColumnCount(3);
    itemmodel->setHeaderData(0,Qt::Horizontal,"娱乐彩蛋榜");
    itemmodel->setHeaderData(1,Qt::Horizontal,"魔鬼胜率榜");
    itemmodel->setHeaderData(2,Qt::Horizontal,"总场数榜");
    itemmodel->setRowCount(client.size());
    QVector<player*> temparr;
    for(auto& x:client)
    {
        temparr.append(x);
    }
    file1.open(QIODevice::WriteOnly);
    for (int i = 1; i < temparr.size(); i++)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if (temparr[j]->entertain_egg < temparr[j + 1]->entertain_egg)
                {
                    player* temp;
                    temp = temparr[j];
                    temparr[j] = temparr[j + 1];
                    temparr[j + 1] = temp;
                }
                else break;
            }
        }
    for (auto& x:temparr)
    {
        file1.write((char*)x,sizeof(player));
    }
    for (int i = 1; i < temparr.size(); i++)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if (temparr[j]->devil_rate < temparr[j + 1]->devil_rate)
                {
                    player* temp;
                    temp = temparr[j];
                    temparr[j] = temparr[j + 1];
                    temparr[j + 1] = temp;
                }
                else break;
            }
        }
    for (auto& x:temparr)
    {
        file1.write((char*)x,sizeof(player));
    }
    for (int i = 1; i < temparr.size(); i++)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if ((temparr[j]->classic_number+temparr[j]->devil_number+temparr[j]->entertain_number)
                        < (temparr[j+1]->classic_number+temparr[j+1]->devil_number+temparr[j+1]->entertain_number))
                {
                    player* temp;
                    temp = temparr[j];
                    temparr[j] = temparr[j + 1];
                    temparr[j + 1] = temp;
                }
                else break;
            }
        }
    for (auto& x:temparr)
    {
        file1.write((char*)x,sizeof(player));
    }
    file1.close();
file1.open(QIODevice::ReadOnly);
player tempplay;
for (int i=0;i<client.size();i++)
{
  file1.read((char*)&tempplay,sizeof(player));
  itemmodel->setItem(i,0,new QStandardItem("昵称: "+QString (tempplay.name)+'\n'+"娱乐场数: "
  +QString::number( tempplay.entertain_number,10)+'\n'+"娱乐彩蛋数: "+QString::number( tempplay.entertain_egg,10)));
  itemmodel->item(i,0)->setTextAlignment(Qt::AlignCenter);
}
for (int i=0;i<client.size();i++)
{
  file1.read((char*)&tempplay,sizeof(player));
  itemmodel->setItem(i,1,new QStandardItem("昵称: "+QString(tempplay.name)+'\n'+"魔鬼场数: "
  +QString::number( tempplay.devil_number,10)+'\n'+"魔鬼胜率: "+QString::number( tempplay.devil_rate)));
  itemmodel->item(i, 1)->setTextAlignment(Qt::AlignCenter);
}
for (int i=0;i<client.size();i++)
{
  file1.read((char*)&tempplay,sizeof(player));
  itemmodel->setItem(i,2,new QStandardItem("昵称: "+QString(tempplay.name)+'\n'+"总场数: "+
  QString::number( tempplay.classic_number+tempplay.devil_number+tempplay.entertain_number,10)));
  itemmodel->item(i, 2)->setTextAlignment(Qt::AlignCenter);
}
file1.close();
table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void mazeserver::on_pushButton_3_clicked()
{   table->hide();
    ui->plainTextEdit->clear();
    ui->plainTextEdit->show();
  ui->pushButton->setEnabled(true);
  ui->pushButton_2->setEnabled(true);
  ui->pushButton_3->setEnabled(false);
  file2.open(QIODevice::ReadOnly);
  ui->plainTextEdit->appendPlainText(file2.readAll());
  file2.close();
}
