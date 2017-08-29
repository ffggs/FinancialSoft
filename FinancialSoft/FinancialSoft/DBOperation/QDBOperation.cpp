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
				" money_borrow NUMERIC, money_loan NUMERIC, remark TEXT, voucher TEXT, id INTEGER PRIMARY KEY)";
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

bool QDBOperation::queryInfo(const st_query_info& queryInfo, CBQuery cb)
{
	bool bRet = false;
	QString strSql = QString("select * from data_info where date>='%1' and date<='%2'").arg(queryInfo.strDateStart).arg(queryInfo.strDateEnd);
	if (!queryInfo.strSubject1.isEmpty()){
		strSql += QString(" and subject1 like '%%1%'").arg(queryInfo.strSubject1);
	}
	if (!queryInfo.strSubject2.isEmpty()){
		strSql += QString(" and subject2 like '%%1%'").arg(queryInfo.strSubject2);
	}
	if (!queryInfo.strVoucher.isEmpty()){
		strSql += QString(" and voucher like '%%1%'").arg(queryInfo.strVoucher);
	}
	if (!queryInfo.strdigest.isEmpty()){
		strSql += QString(" and digest like '%%1%'").arg(queryInfo.strdigest);
	}

	if (!queryInfo.strMoneyBorrow.isEmpty())
	{
		bool bRet = false;
		int iMoney = queryInfo.strMoneyBorrow.toInt(&bRet);
		if (bRet){
			strSql += QString(" and money_borrow='%1'").arg(iMoney);
		}
	}

	if (!queryInfo.strMoneyLoan.isEmpty())
	{
		bool bRet = false;
		int iMoney = queryInfo.strMoneyLoan.toInt(&bRet);
		if (bRet){
			strSql += QString(" and money_loan='%1'").arg(iMoney);
		}
	}
	if (!queryInfo.strRemark.isEmpty()){
		strSql += QString(" and remark like '%%1%'").arg(queryInfo.strRemark);
	}

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
	else{
		qInfo() << "QDBOperation::queryInfo db query failed,sql:" << strSql << ",error:" << sqlQuery.lastError().text();
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
	info.strRemark = sqlQuery.value(6).toString();
	info.strVoucher = sqlQuery.value(7).toString();
	info.iId = sqlQuery.value(8).toInt();

	return true;
}
bool QDBOperation::queryDataByID(int iId, st_data_info& data)
{
	QString strSql = QString("select * from data_info where id=%1").arg(iId);
	QSqlQuery sqlQuery;
	bool bRet = sqlQuery.exec(strSql);
	if (bRet && sqlQuery.next())
	{
		bRet = getDBOneInfo(sqlQuery, data);
	}
	return bRet;
}

bool QDBOperation::updateDataByID(int iId, st_data_info& data)
{
	QMutexLocker locker(&m->mtx_for_db);
	//QString strSql = "insert into data_info values(?,?,?,?,?,?,?,?,?,?)";
	QString strSql = QString("update data_info set date='%1',subject1='%2',subject2='%3',digest='%4',money_borrow=%5,money_loan=%6,remark='%7',voucher='%8' where id=%9")
		.arg(data.strDate).arg(data.strSubject1).arg(data.strSubject2).arg(data.strdigest).arg(data.iMoneyBorrow)
		.arg(data.iMoneyLoan).arg(data.strRemark).arg(data.strVoucher).arg(data.iId);

	QSqlQuery sqlQuery(m->database);
	bool bRet = sqlQuery.exec(strSql);
	if (!bRet)
	{
		qInfo() << "update data to db failed,error:" << sqlQuery.lastError().text() << ",sql:" << strSql;
	}
	return bRet;
}
bool QDBOperation::addOneData(st_data_info& data)
{
	QMutexLocker locker(&m->mtx_for_db);
	data.iId = getDbMaxID();
	//QString strSql = "insert into data_info values(?,?,?,?,?,?,?,?,?,?)";
	QString strSql = QString("insert into data_info values('%1','%2','%3','%4',%5,%6,'%7','%8',%9)")
		.arg(data.strDate).arg(data.strSubject1).arg(data.strSubject2).arg(data.strdigest).arg(data.iMoneyBorrow)
		.arg(data.iMoneyLoan).arg(data.strRemark).arg(data.strVoucher).arg(data.iId);

 	QSqlQuery sqlQuery(m->database);
	bool bRet = sqlQuery.exec(strSql);
	if (bRet)
	{
		m->database.commit();
	}
	else{
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

bool QDBOperation::deleteRow(int iId)
{
	QMutexLocker locker(&m->mtx_for_db);
	QString strSql = QString("delete from data_info where id=%1").arg(iId);
	QSqlQuery sqlQuery;
	
	return sqlQuery.exec(strSql);
}
bool QDBOperation::deleteRows(QList<int> lstRows)
{
	if (lstRows.isEmpty())
	{
		return true;
	}
	QMutexLocker locker(&m->mtx_for_db);
	QString strSql = QString("delete from data_info where ");
	QString qstrIds;
	for (int i = 0; i < lstRows.size(); i++)
	{
		if (qstrIds.isEmpty())
		{
			qstrIds = QString("id=%1").arg(lstRows[i]);
		}
		else{
			qstrIds += QString(" or id=%1").arg(lstRows[i]);
		}
	}
	strSql += qstrIds;

	QSqlQuery sqlQuery;
	return sqlQuery.exec(strSql);
}