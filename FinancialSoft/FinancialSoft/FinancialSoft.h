#pragma once
#include "DBOperation/QDBOperation.h"

#include <QtWidgets/QMainWindow>
#include <QSharedPointer>
#include "Data/DataInfo.h"
#include "Data/QueryInfo.h"

class FinancialSoftPri;
class FinancialSoft : public QMainWindow
{
    Q_OBJECT

public:
    FinancialSoft(QWidget *parent = Q_NULLPTR);

private:
	void initPage1();
	void initPage2();
	void initStatusBar();
	void initSignalSlots();

	void toPage1();
	void toPage2(bool bAdd);

	void setDataInfoToUI(const st_data_info& info);
	QString getDataInfoFromUI(st_data_info& info);

	void getQueryInfoFromUI(st_query_info& queryInfo);

	void updateSurplusMoney();
private slots:
	void slotBtnToAddPageClicked();
	void slotBtnGiveUpAndReturnClicked();
	void slotBtnAddToDbCLicked();
	void slotBtnSaveChgToDbClicked();

	void slotBtnDeleteClicked();

	void slotBtnQueryClicked();

	void slotEditDataInfo(int iRow,const st_data_info& info);

	void slotBorrowMoneyChanged(int iTotal);
	void slotLoanMoneyChanged(int iTotal);

	void slotBorrowMoneyMinus(int iNum);
	void slotBorrowMoneyPlus(int iNum);

	void slotLoanMoneyMinus(int iNum);
	void slotLoanMoneyPlus(int iNum);
private:
	QSharedPointer<FinancialSoftPri> m;
};
