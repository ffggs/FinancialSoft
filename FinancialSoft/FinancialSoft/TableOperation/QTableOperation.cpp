#pragma execution_character_set("utf-8")
#include "QTableOperation.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>
#include "../Comm/CommHeader.h"



QTableOperation* QTableOperation::instance()
{
	static QTableOperation* pTable = new QTableOperation();
	return pTable;
}
QTableOperation::QTableOperation() :QObject(nullptr), m_pTable(nullptr)
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
	_emptyTableWidget(pTable);

	QStringList lstColums;
	lstColums << "日期" << "一级科目" << "二级科目" << "摘要" << "借方金额(元)" << "贷方金额(元)" << "备注" << "凭证号" << "操作" << "ID";
	pTable->setColumnCount(lstColums.size());
	pTable->setHorizontalHeaderLabels(lstColums);
	
	pTable->hideColumn(9);
	pTable->setColumnWidth(0, 80);
	pTable->setColumnWidth(1, 60);
	pTable->setColumnWidth(2, 60);
	pTable->setColumnWidth(3, 200);
	pTable->setColumnWidth(4, 90);
	pTable->setColumnWidth(5, 90);
	pTable->setColumnWidth(6, 160);
	pTable->setColumnWidth(7, 110);
	pTable->setColumnWidth(8, 100);
}

void QTableOperation::addOneRow(const st_data_info& info)
{
	int iRowCount = m_pTable->rowCount();
	m_pTable->insertRow(iRowCount);
	updateRowInfo(iRowCount,info);
}
void QTableOperation::updateRowInfo(int iRow, const st_data_info& info)
{
	QTableWidgetItem* pItemDate = new QTableWidgetItem(info.strDate);
	pItemDate->setCheckState(Qt::Unchecked);
	QTableWidgetItem* pItemSubject1 = new QTableWidgetItem(info.strSubject1);
	QTableWidgetItem* pItemSubject2 = new QTableWidgetItem(info.strSubject2);
	QTableWidgetItem* pItemDigest = new QTableWidgetItem(info.strdigest);
	QTableWidgetItem* pItemMoneyBorrow = new QTableWidgetItem(QString::number(info.iMoneyBorrow));
	QTableWidgetItem* pItemMoneyLoad = new QTableWidgetItem(QString::number(info.iMoneyLoan));
	QTableWidgetItem* pItemRemark = new QTableWidgetItem(info.strRemark);
	QTableWidgetItem* pItemVoucher = new QTableWidgetItem(info.strVoucher);
	pItemVoucher->setFlags(pItemVoucher->flags() &(~Qt::ItemIsEnabled));
	QTableWidgetItem* pItemEditor = new QTableWidgetItem();
	QPushButton* pBtnEdit = new QPushButton("编辑");
	pBtnEdit->setStyleSheet("border:none;color:blue");
	pBtnEdit->setCursor(Qt::PointingHandCursor);

	connect(pBtnEdit, &QPushButton::clicked, [this, pItemDate](){
		int iRow = m_pTable->row(pItemDate);
		slotEditRow(iRow);
	});

	QPushButton* pBtnDel = new QPushButton("删除");
	pBtnDel->setStyleSheet("border:none;color:blue");
	pBtnDel->setCursor(Qt::PointingHandCursor);
	connect(pBtnDel, &QPushButton::clicked, this,[this,pItemDate](){
		int iRow = m_pTable->row(pItemDate);
		slotDeleteRow(iRow);
	});

	QWidget* pWgt = new QWidget();
	QHBoxLayout* pLay = new QHBoxLayout(pWgt);
	pLay->addWidget(pBtnEdit);
	pLay->addWidget(pBtnDel);
	pLay->addStretch();
	pLay->setContentsMargins(10, 0, 0, 0);
	pLay->setSpacing(8);
	m_pTable->setCellWidget(iRow, 8,pWgt);


	pItemEditor->setFlags(pItemEditor->flags() &(~Qt::ItemIsEditable));
	QTableWidgetItem* pItemID = new QTableWidgetItem(QString::number(info.iId));

	m_pTable->setItem(iRow, ROW_INDEX_DATE, pItemDate);			
	m_pTable->setItem(iRow, ROW_INDEX_SUBJECT1, pItemSubject1);		
	m_pTable->setItem(iRow, ROW_INDEX_SUBJECT2, pItemSubject2);		
	m_pTable->setItem(iRow, ROW_INDEX_DIGEST, pItemDigest);		
	m_pTable->setItem(iRow, ROW_INDEX_BORROW, pItemMoneyBorrow);	
	m_pTable->setItem(iRow, ROW_INDEX_LOAN, pItemMoneyLoad);		
	m_pTable->setItem(iRow, ROW_INDEX_REMARK, pItemRemark);		
	m_pTable->setItem(iRow, ROW_INDEX_VOUCHER, pItemVoucher);		
	m_pTable->setItem(iRow, ROW_INDEX_EDITOR, pItemEditor);		
	m_pTable->setItem(iRow, ROW_INDEX_ID, pItemID);			
}

void QTableOperation::_emptyTableWidget(QTableWidget* pTable)
{
	pTable->clearContents();
	pTable->setRowCount(0);
}
bool QTableOperation::callback_query_info(const st_data_info& info)
{
	m_iTotalBorrowMoney += info.iMoneyBorrow;
	m_iTotalLoanMoney += info.iMoneyLoan;

	addOneRow(info);
	return true;
}

bool QTableOperation::queryInfo(const st_query_info& info)
{
	m_iTotalBorrowMoney = 0;
	m_iTotalLoanMoney = 0;
	m_iTotalSurplusMoney = 0;
	_emptyTableWidget(m_pTable);
	CBQuery cb = std::bind(&QTableOperation::callback_query_info, this, std::placeholders::_1);
	bool bRet = QDBOperation::instance()->queryInfo(info, cb);

	m_iTotalSurplusMoney = m_iTotalBorrowMoney - m_iTotalLoanMoney;

	emit sgTotalBorrowMoneyChanged(m_iTotalBorrowMoney);
	emit sgTotalLoanMoneyChanged(m_iTotalLoanMoney);

	return bRet;
}

void QTableOperation::slotDeleteRow(int iRow)
{
	QString qstrId = m_pTable->item(iRow, ROW_INDEX_ID)->text();
	int iId = qstrId.toInt();
	if (QDBOperation::instance()->deleteRow(iId))
	{
		m_pTable->removeRow(iRow);
	}
}
void QTableOperation::slotEditRow(int iRow)
{
	int iId = m_pTable->item(iRow, ROW_INDEX_ID)->text().toInt();
	st_data_info info;
	info.strDate = m_pTable->item(iRow, ROW_INDEX_DATE)->text();
	info.strSubject1 = m_pTable->item(iRow, ROW_INDEX_SUBJECT1)->text();
	info.strSubject2 = m_pTable->item(iRow, ROW_INDEX_SUBJECT2)->text();
	info.strdigest = m_pTable->item(iRow, ROW_INDEX_DIGEST)->text();
	info.strVoucher = m_pTable->item(iRow, ROW_INDEX_VOUCHER)->text();
	info.iMoneyBorrow = m_pTable->item(iRow, ROW_INDEX_BORROW)->text().toInt();
	info.iMoneyLoan = m_pTable->item(iRow, ROW_INDEX_LOAN)->text().toInt();
	info.strRemark = m_pTable->item(iRow, ROW_INDEX_REMARK)->text();
	info.iId = iId;

	emit sgEditRowInfo(iRow, info);
// 
// 	if (!QDBOperation::instance()->updateDataByID(iId,info))
// 	{
// 		qInfo() << "QTableOperation::slotSaveRow updateDataByID failed,row:" << iRow << ",id:" << iId;
// 		return;
// 	}
// 
// 	emit sgTotalBorrowMoneyplus(info.iMoneyBorrow - oldInfo.iMoneyBorrow);
// 	emit sgTotalLoanMoneyplus(info.iMoneyLoan - oldInfo.iMoneyLoan);
}