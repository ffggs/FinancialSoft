#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QSharedPointer>
#include <functional>
#include <QSqlQuery>
#include <QList>
#include "../Data/DataInfo.h"
#include "../Data/QueryInfo.h"

enum class em_money_type{
	type_bank = 1,        // 银行卡类
	type_creditcard = 2,  //信用卡类
	type_cash = 3        //现金类
};

class QDBOperationPri;

typedef std::function<bool(const st_data_info&)> CBQuery;
class QDBOperation 
{

public:
	static QDBOperation* instance();

	bool queryDataByID(int iId,st_data_info& data);

	bool updateDataByID(int iId, st_data_info& data);
	//添加一项数据
	bool addOneData(st_data_info& data);

	bool deleteRow(int iId);
	bool deleteRows(QList<int> lstIDs);

	void closeDb();

	//初始化是否成功
	bool isInitOk();

	bool queryInfo(const st_query_info& queryInfo, CBQuery cb);

private:
	bool init();

	int getDbMaxID();

	bool getDBOneInfo(const QSqlQuery& sqlQuery,st_data_info& info);

public:
	QDBOperation();
	~QDBOperation();

private:
	
	QSharedPointer<QDBOperationPri> m;
};
