#ifndef MMYSQL_H
#define MMYSQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

class dataBase : public QObject
{
    Q_OBJECT
public:
    explicit dataBase(QObject *parent = nullptr);

    bool init(const QString databasetype,
              const QString databaseName,
              const QString hostName,
              const QString userName,
              const QString passWord);
    bool query(QSqlQuery *queryResult,const QString query);

signals:

public slots:

private:
    QSqlDatabase  database;
    bool          database_state;
    QString       database_type;
    QString       database_name;
    QString       database_hostName;
    QString       database_userName;
    QString       database_passWord;


};

#endif // MMYSQL_H
