#ifndef DATABASE_H
#define DATABASE_H

#include <mainwindow.h>
#include "ui_mainwindow.h"

void MainWindow::searchEmployee()
{

db.setDatabaseName("coins.sqlite");
db.open();
db.transaction();
    QSqlQuery query4;
    //query4.exec("SELECT * FROM users WHERE name = " "'" + userID.toLatin1() + "'" " AND addr = "+coin.toLatin1());
    while (query4.next()) {
     //   yeardb = query.value(0).toInt();
        qDebug() << "coin " << query4.value(0).toString();
       // return yeardb.toLatin1();
    }
db.commit();
db.close();

}



void MainWindow::createTable(QString DBname)
{
    db.setDatabaseName(DBname.toLatin1());

    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    QString query;
  //  query.append("CREATE TABLE IF NOT EXISTS coins("
    //             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
     //            "origid VARCHAR(50),""addr VARCHAR(50));");

    QSqlQuery create;

    create.prepare(query);

    if (create.exec())
    {
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }

    query.clear();
    db.close();

}



#endif // DATABASE_H
