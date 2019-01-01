#include "dataBase.h"

dataBase::dataBase(QObject *parent) : QObject(parent)
{
    database_state =false;
}
/**
*   @project         xyb_TemplaeLib
*   @brief           摘要
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-20
*/
bool dataBase::init(const QString databasetype,
                    const QString databaseName,
                    const QString hostName,
                    const QString userName,
                    const QString passWord)
{
    QString meg;
    database_state = true;
    if(databasetype.isEmpty())
    {
        meg.append("database type is empty.");
        database_state = false;
    }
    if(databaseName.isEmpty())
    {
        meg.append("database name is empty.");
        database_state = false;
    }
    if(hostName.isEmpty())
    {
        meg.append("host name is empty.");
        database_state = false;
    }
    if(userName.isEmpty())
    {
        meg.append("user name is empty.");
        database_state = false;
    }
    if(passWord.isEmpty())
    {
        meg.append("password is empty.");
        database_state = false;
    }
    if(database_state)
    {
        database_type       = databasetype;
        database_name       = databaseName;
        database_hostName   = hostName;
        database_userName   = userName;
        database_passWord   = passWord;

        database = QSqlDatabase::addDatabase(database_type);
        database.setDatabaseName(database_name);
        database.setHostName(database_hostName);
        database.setUserName(database_userName);
        database.setPassword(database_passWord);
        qDebug()<<"init database succeed.";
    }else {
        qDebug()<<"init database failed."<< meg;
    }
    return database_state;
}
/**
*   @project         xyb_TemplaeLib
*   @brief           摘要
*   @param a         参数1：
*   @param b         参数2：
*   @author          XuYuanbing
*   @data            2018-12-20
*/
bool dataBase::query(QSqlQuery *queryResult,const QString query)
{
    if(query.isEmpty())
    {
        qDebug()<<"query where is empty.";
        return false;
    }
    queryResult->clear();

    if(database.open())
    {
        queryResult->operator=(database.exec(query));
        database.close();
        return true;
    }else {
        qDebug()<<"can not open database.";
        return false;
    }
}
