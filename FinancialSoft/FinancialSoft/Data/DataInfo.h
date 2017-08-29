#pragma once
#include <QString>
#include <QDateTime>
#include "../Comm/CommHeader.h"
struct st_data_info
{
	QString strDate;      //日期
	QString strSubject1;  //一级科目
	QString strSubject2;  //二级科目
	QString strdigest;    //摘要
	int     iMoneyBorrow; //借方金额
	int     iMoneyLoan;   //贷方金额
	QString strRemark;    //说明
	QString	strVoucher;	  //凭证号
	int		iId;		  //项ID，该项的唯一标识
	//产生凭证号函数，以当前日期时间组合
	void initDateAndVoucher(){
		QDateTime dt = QDateTime::currentDateTime();
		strDate = dt.toString(DATE_TIME_FORMAT);
		strVoucher = dt.toString("yyyyMMddhhmmss");
	}

	st_data_info() :iMoneyBorrow(0), iMoneyLoan(0), iId(0){

	}
};

