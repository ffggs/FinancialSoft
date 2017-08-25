#pragma execution_character_set("utf-8")
#include "QDBOperation.h"
#include <QDir>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>  
#include <QSqlQuery>  
#include <QDebug>
#include <QtGlobal>
#include <QMutexLocker>


class QDBOperationPri{
	friend class QDBOperation;

	QString strDbPath;//数据库路径
	QMutex mtx_for_db;//数据库同步锁
	bool bInitSuccess = false;

	QSqlDatabase database;

	//锁设定为运行同一线程重入
	QDBOperationPri() :mtx_for_db(QMutex::Recursive){

	}
};
QDBOperation::QDBOperation()
	:m(new QDBOperationPri)
{
	m->bInitSuccess = init();
}

QDBOperation::~QDBOperation()
{
}
QDBOperation* QDBOperation::instance()
{
	static QDBOperation* pDbOper = new QDBOperation();
	return pDbOper;
}
bool QDBOperation::init()
{
	QMutexLocker(&m->mtx_for_db);
	bool bRet = false;
	do 
	{
		QString strDir = QDir::currentPath() + "\\data\\";
		QDir().mkpath(strDir);
		m->strDbPath = strDir + "dbinfo.db";

		bool bDbFileExists = QFile::exists(m->strDbPath);
		//设置数据库信息
		m->database = QSqlDatabase::addDatabase("QSQLITE");
		m->database.setDatabaseName(m->strDbPath);
		bool bOpen = m->database.open();
		if (!m->database.isOpen()){
			qInfo() << "open sqlite failed,error str:" << m->database.lastError().nativeErrorCode() << ",db path:" << m->strDbPath;
			break;
		}
		//文件不存在，则是首次运行，需要创建表
		if (!bDbFileExists)
		{
			QString strCreateTable = "CREATE TABLE data_info (date TEXT, subject1 TEXT, subject2 TEXT, digest TEXT,"
				" money_borrow NUMERIC, money_loan NUMERIC, surplus NUMERIC, remark TEXT, voucher TEXT, id INTEGER PRIMARY KEY)";
			QSqlQuery query;
			if (!query.exec(strCreateTable)){
				qInfo() << "create tabel query.exec failed";
				break;
			}
		}
		bRet = true;
	} while (false);
	return bRet;
}

bool QDBOperation::isInitOk()
{
	return m->bInitSuccess;
}

bool QDBOperation::queryInfo(const QString& strStartDate, const st_data_info& info,CBQuery cb)
{
	bool bRet = false;
	QString strSql = QString("select * from data_info where date='&1'").arg(strStartDate);
	QSqlQuery sqlQuery;
	bRet = sqlQuery.exec(strSql);
	if (bRet)
	{
		while (sqlQuery.next())
		{
			st_data_info info;
			getDBOneInfo(sqlQuery, info);
			cb(info);
		}
	}
	return bRet;
}
bool QDBOperation::getDBOneInfo(const QSqlQuery& sqlQuery, st_data_info& info)
{
	info.strDate = sqlQuery.value(0).toString();
	info.strSubject1 = sqlQuery.value(1).toString();
	info.strSubject2 = sqlQuery.value(2).toString();
	info.strdigest = sqlQuery.value(3).toString();
	info.iMoneyBorrow = sqlQuery.value(4).toInt();
	info.iMoneyLoan = sqlQuery.value(5).toInt();
	info.iSurplus = sqlQuery.value(6).toInt();
	info.strRemark = sqlQuery.value(7).toString();
	info.strVoucher = sqlQuery.value(8).toString();
	info.iId = sqlQuery.value(9).toInt();
}

bool QDBOperation::addOneData(st_data_info& data)
{
	QMutexLocker locker(&m->mtx_for_db);
	data.iId = getDbMaxID();
	//QString strSql = "insert into data_info values(?,?,?,?,?,?,?,?,?,?)";
	QString strSql = QString("insert into data_info values('%1','%2','%3','%4',%5,%6,%7,'%8','%9',%10)")
		.arg(data.strDate).arg(data.strSubject1).arg(data.strSubject2).arg(data.strdigest).arg(data.iMoneyBorrow)
		.arg(data.iMoneyLoan).arg(data.iSurplus).arg(data.strRemark).arg(data.strVoucher).arg(data.iId);

 	QSqlQuery sqlQuery(m->database);
	bool bRet = sqlQuery.exec(strSql);
	if (!bRet){
		qInfo() << "add data to db failed,error:" << sqlQuery.lastError().text() << ",sql:" << strSql;
	}
	return bRet;
}
int QDBOperation::getDbMaxID()
{
	int iId = 1;
	QMutexLocker locker(&m->mtx_for_db);

	QString strSql = "select id from data_info order by id desc limit 0,1";
	QSqlQuery sqlQuery;
	if (sqlQuery.exec(strSql) && sqlQuery.next())
	{
		iId = sqlQuery.value(0).toInt();
		iId++; //取到ID后，自动加1
	}
	return iId;
}