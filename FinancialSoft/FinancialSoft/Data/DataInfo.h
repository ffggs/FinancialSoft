#pragma once
#include <QString>
#include <QDateTime>
#include "../Comm/CommHeader.h"
struct st_data_info
{
	QString strDate;      //����
	QString strSubject1;  //һ����Ŀ
	QString strSubject2;  //������Ŀ
	QString strdigest;    //ժҪ
	int     iMoneyBorrow; //�跽���
	int     iMoneyLoan;   //�������
	QString strRemark;    //˵��
	QString	strVoucher;	  //ƾ֤��
	int		iId;		  //��ID�������Ψһ��ʶ
	//����ƾ֤�ź������Ե�ǰ����ʱ�����
	void initDateAndVoucher(){
		QDateTime dt = QDateTime::currentDateTime();
		strDate = dt.toString(DATE_TIME_FORMAT);
		strVoucher = dt.toString("yyyyMMddhhmmss");
	}

	st_data_info() :iMoneyBorrow(0), iMoneyLoan(0), iId(0){

	}
};

