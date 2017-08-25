#pragma once
#include <QTableWidget>
#include "./DBOperation/QDBOperation.h"
class QTableOperation
{
public:
	static QTableOperation* instance();

	void setTableWgt(QTableWidget* pTable);

	//添加一行
	void addOneRow(const st_data_info& info);
	//更新一行
	void updateRowInfo(int iRow, const st_data_info& info);
	//查询信息
	void queryInfo(const QString& strStartDate,const st_data_info& info);

signals:
	//改变某行的信息
	void sgChangeInfo(int iRow,st_data_info& info);

private:
	QTableOperation();
	~QTableOperation();

	void _initTableWidget(QTableWidget* pTable);
private:
	QTableWidget* m_pTable;
};

