#pragma execution_character_set("utf-8")
#include "QTableOperation.h"


QTableOperation* QTableOperation::instance()
{
	static QTableOperation* pTable = new QTableOperation();
	return pTable;
}
QTableOperation::QTableOperation() :m_pTable(nullptr)
{
}


QTableOperation::~QTableOperation()
{
}
void QTableOperation::setTableWgt(QTableWidget* pTable)
{
	m_pTable = pTable;

	_initTableWidget(m_pTable);
}

void QTableOperation::_initTableWidget(QTableWidget* pTable)
{
	pTable->clearContents();
	pTable->setRowCount(0);

	QStringList lstColums;
	lstColums << "日期" << "一级科目" << "二级科目" << "摘要" << "借方金额(元)" << "贷方金额(元)" << "余额(元)" << "备注" << "凭证号" << "操作" << "ID";
	pTable->setColumnCount(lstColums.size());
	pTable->setHorizontalHeaderLabels(lstColums);
	
	pTable->hideColumn(10);
	pTable->setColumnWidth(0, 60);
	pTable->setColumnWidth(1, 60);
	pTable->setColumnWidth(2, 60);
	pTable->setColumnWidth(3, 200);
	pTable->setColumnWidth(4, 90);
	pTable->setColumnWidth(5, 90);
	pTable->setColumnWidth(6, 60);
	pTable->setColumnWidth(7, 160);
	pTable->setColumnWidth(8, 100);
	pTable->setColumnWidth(9, 100);
}