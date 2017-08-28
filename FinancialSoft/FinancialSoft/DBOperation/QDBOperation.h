#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QSharedPointer>
#include <functional>
#include <QSqlQuery>

enum class em_money_type{
	type_bank = 1,        // ���п���
	type_creditcard = 2,  //���ÿ���
	type_cash = 3        //�ֽ���
};
struct st_data_info 
{
	QString strDate;      //����
	QString strSubject1;  //һ����Ŀ
	QString strSubject2;  //������Ŀ
	QString strdigest;    //ժҪ
	int     iMoneyBorrow; //�跽���
	int     iMoneyLoan;   //�������
	int		iSurplus;     //���
	QString strRemark;    //˵��
	QString	strVoucher;	  //ƾ֤��
	int		iId;		  //��ID�������Ψһ��ʶ
	//����ƾ֤�ź������Ե�ǰ����ʱ�����
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

	//���һ������
	bool addOneData(st_data_info& data);

	void closeDb();

	//��ʼ���Ƿ�ɹ�
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
