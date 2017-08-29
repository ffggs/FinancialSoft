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
	type_bank = 1,        // ���п���
	type_creditcard = 2,  //���ÿ���
	type_cash = 3        //�ֽ���
};

class QDBOperationPri;

typedef std::function<bool(const st_data_info&)> CBQuery;
class QDBOperation 
{

public:
	static QDBOperation* instance();

	bool queryDataByID(int iId,st_data_info& data);

	bool updateDataByID(int iId, st_data_info& data);
	//���һ������
	bool addOneData(st_data_info& data);

	bool deleteRow(int iId);
	bool deleteRows(QList<int> lstIDs);

	void closeDb();

	//��ʼ���Ƿ�ɹ�
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
