#pragma once
#include <QTableWidget>
#include <QObject>
#include "./DBOperation/QDBOperation.h"
class QTableOperation:public QObject
{
	Q_OBJECT
public:
	static QTableOperation* instance();

	void setTableWgt(QTableWidget* pTable);

	//添加一行
	void addOneRow(const st_data_info& info);
	//更新一行
	void updateRowInfo(int iRow, const st_data_info& info);

	//查询信息
	bool queryInfo(const st_query_info& info);

private:
	bool callback_query_info(const st_data_info& info);

private:
	void slotDeleteRow(int iRow);
	void slotEditRow(int iRow);

signals:
	//改变某行的信息
	void sgEditRowInfo(int iRow,const st_data_info& info);
	void sgTotalBorrowMoneyChanged(int iTotal);
	void sgTotalLoanMoneyChanged(int iTotal);

	void sgTotalBorrowMoneyplus(int iTotal);
	void sgTotalLoanMoneyplus(int iTotal);
private:
	QTableOperation();
	~QTableOperation();

	void _initTableWidget(QTableWidget* pTable);
	void _emptyTableWidget(QTableWidget* pTable);
private:
	QTableWidget* m_pTable;
	int m_iTotalBorrowMoney;
	int m_iTotalLoanMoney;
	int m_iTotalSurplusMoney;
};

