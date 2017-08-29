#pragma once
#include <QString>

//查询信息，如果为空，则表示该项不查询
struct  st_query_info
{
	QString strDateStart;    //开始日期
	QString strDateEnd;		//结束日期
	QString strSubject1;	//一级科目
	QString strSubject2;	//二级科目
	QString strdigest;		//摘要
	QString strMoneyBorrow; //借方金额
	QString strMoneyLoan;   //贷方金额
	QString	strSurplus;     //余额
	QString strRemark;		//说明
	QString	strVoucher;		//凭证号
};

