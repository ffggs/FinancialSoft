#pragma once
#include <QTableWidget>
#include "./DBOperation/QDBOperation.h"
class QTableOperation
{
public:
	static QTableOperation* instance();

	void setTableWgt(QTableWidget* pTable);

	//���һ��
	void addOneRow(const st_data_info& info);
	//����һ��
	void updateRowInfo(int iRow, const st_data_info& info);
	//��ѯ��Ϣ
	void queryInfo(const QString& strStartDate,const st_data_info& info);

signals:
	//�ı�ĳ�е���Ϣ
	void sgChangeInfo(int iRow,st_data_info& info);

private:
	QTableOperation();
	~QTableOperation();

	void _initTableWidget(QTableWidget* pTable);
private:
	QTableWidget* m_pTable;
};

