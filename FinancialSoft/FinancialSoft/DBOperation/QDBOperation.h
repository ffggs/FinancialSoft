#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QSharedPointer>
#include <functional>
#include <QSqlQuery>

enum class em_money_type{
	type_bank = 1,        // 银行卡类
	type_creditcard = 2,  //信用卡类
	type_cash = 3        //现金类
};
struct st_data_info 
{
	QString strDate;      //日期
	QString strSubject1;  //一级科目
	QString strSubject2;  //二级科目
	QString strdigest;    //摘要
	int     iMoneyBorrow; //借方金额
	int     iMoneyLoan;   //贷方金额
	int		iSurplus;     //余额
	QString strRemark;    //说明
	QString	strVoucher;	  //凭证号
	int		iId;		  //项ID，该项的唯一标识
	//产生凭证号函数，以当前日期时间组合
	void initDateAndVoucher(){
		QDateTime dt = QDateTime::currentDateTime();
		strDate = dt.toString("yyyy-MM-dd");
		strVoucher = dt.toString("yyyyMMddhhmmss");
	}
	
	st_data_info():iMoneyBorrow(0),iMoneyLoan(0),iSurplus(0),iId(0){

	}
};
class QDBOperationPri;

typedef std::function<bool(const st_data_info&)> CBQuery;
class QDBOperation 
{

public:
	static QDBOperation* instance();

	//添加一项数据
	bool addOneData(st_data_info& data);

	void closeDb();

	//初始化是否成功
	bool isInitOk();

	bool queryInfo(const QString& strStartDate, const st_data_info& info,CBQuery cb);

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
