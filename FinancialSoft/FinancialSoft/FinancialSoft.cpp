#pragma execution_character_set("utf-8")

#include "FinancialSoft.h"
#include "ui_FinancialSoft.h"
#include "Comm/CommFunc.h"
#include "TableOperation/QTableOperation.h"
#include "DBOperation/QDBOperation.h"
#include "Comm/CommHeader.h"

class FinancialSoftPri{
	friend class FinancialSoft;
	Ui::FinancialSoftClass ui;	
	QLabel* pLabel_borrow;
	QLabel* pLabel_loan;
	QLabel* pLabel_surplus;

	int iEditRow;
	st_data_info infoForEdit;
};
FinancialSoft::FinancialSoft(QWidget *parent)
	: QMainWindow(parent),m(new FinancialSoftPri())
{
	m->ui.setupUi(this);
	initPage1();
	initPage2();
	initStatusBar();
	initSignalSlots();
	toPage1();

	slotBtnQueryClicked();
}

void FinancialSoft::initSignalSlots()
{
	connect(m->ui.pushButton_add_page, &QPushButton::clicked, this, &FinancialSoft::slotBtnToAddPageClicked);
	connect(m->ui.pushButton_add_info, &QPushButton::clicked, this, &FinancialSoft::slotBtnAddToDbCLicked);
	connect(m->ui.pushButton_save, &QPushButton::clicked, this, &FinancialSoft::slotBtnSaveChgToDbClicked);
	connect(m->ui.pushButton_return, &QPushButton::clicked, this, &FinancialSoft::slotBtnGiveUpAndReturnClicked);
	connect(m->ui.pushButton_query, &QPushButton::clicked, this, &FinancialSoft::slotBtnQueryClicked);

	connect(QTableOperation::instance(),&QTableOperation::sgTotalBorrowMoneyChanged,this,&FinancialSoft::slotBorrowMoneyChanged);
	connect(QTableOperation::instance(), &QTableOperation::sgTotalLoanMoneyChanged, this, &FinancialSoft::slotLoanMoneyChanged);

	connect(QTableOperation::instance(), &QTableOperation::sgTotalBorrowMoneyplus, this, &FinancialSoft::slotBorrowMoneyPlus);
	connect(QTableOperation::instance(), &QTableOperation::sgTotalLoanMoneyplus, this, &FinancialSoft::slotLoanMoneyPlus);

	connect(QTableOperation::instance(), &QTableOperation::sgEditRowInfo, this, &FinancialSoft::slotEditDataInfo);

	connect(m->ui.pushButton_delete, &QPushButton::clicked, this, &FinancialSoft::slotBtnDeleteClicked);
}

void FinancialSoft::toPage1()
{
	m->ui.stackedWidget->setCurrentWidget(m->ui.page1);
	m->ui.lineEdit_subject1_query->setFocus();
	m->ui.dateTimeEdit_query_end->setDateTime(QDateTime::currentDateTime());
}
void FinancialSoft::toPage2(bool bAdd)
{
	m->ui.stackedWidget->setCurrentWidget(m->ui.page2);
	if (bAdd)
	{
		m->ui.pushButton_add_info->show();
		m->ui.pushButton_save->hide();
	}
	else{
		m->ui.pushButton_add_info->hide();
		m->ui.pushButton_save->show();
	}
	m->ui.lineEdit_subject1_change->setFocus();
}

void FinancialSoft::setDataInfoToUI(const st_data_info& info)
{
	m->ui.dateEdit_change->setDateTime(QDateTime::fromString(info.strDate, DATE_TIME_FORMAT));
	m->ui.lineEdit_subject1_change->setText(info.strSubject1);
	m->ui.lineEdit_subject2_change->setText(info.strSubject2);
	m->ui.lineEdit_voucher_change->setText(info.strVoucher);
	m->ui.lineEdit_digest_change->setText(info.strdigest);
	m->ui.lineEdit_remark_change->setText(info.strRemark);
	m->ui.lineEdit_borrow_change->setText(QString::number(info.iMoneyBorrow));
	m->ui.lineEdit_loan_change->setText(QString::number(info.iMoneyLoan));
}

QString FinancialSoft::getDataInfoFromUI(st_data_info& info)
{
	QString qstrErr;
	do 
	{
		info.strDate = m->ui.dateEdit_change->dateTime().toString(DATE_TIME_FORMAT);
		info.strSubject1 = m->ui.lineEdit_subject1_change->text();
		if (info.strSubject1.isEmpty())
		{
			qstrErr = "一级科目不能为空，请填写后再继续";
			break;
		}
		info.strSubject2 = m->ui.lineEdit_subject2_change->text();
		if (info.strSubject2.isEmpty())
		{
			qstrErr = "二级科目不能为空，请填写后再继续";
			break;
		}
		info.strVoucher = m->ui.lineEdit_voucher_change->text();
		info.strdigest = m->ui.lineEdit_digest_change->text();
		info.strRemark = m->ui.lineEdit_remark_change->text();
		info.iMoneyBorrow = m->ui.lineEdit_borrow_change->text().toInt();
		info.iMoneyLoan = m->ui.lineEdit_loan_change->text().toInt();
	} while (false);
	return qstrErr;
}

void FinancialSoft::getQueryInfoFromUI(st_query_info& queryInfo)
{
	queryInfo.strDateStart = m->ui.dateTimeEdit_query_start->dateTime().toString(DATE_TIME_FORMAT);
	queryInfo.strDateEnd = m->ui.dateTimeEdit_query_end->dateTime().toString(DATE_TIME_FORMAT);
	queryInfo.strSubject1 = m->ui.lineEdit_subject1_query->text();
	queryInfo.strSubject2 = m->ui.lineEdit_subject2_query->text();
	queryInfo.strVoucher = m->ui.lineEdit_voucher_query->text();
	queryInfo.strdigest = m->ui.lineEdit_digest_query->text();
	queryInfo.strMoneyBorrow = m->ui.lineEdit_borrow_query->text();
	queryInfo.strMoneyLoan = m->ui.lineEdit_loan_query->text();
	queryInfo.strRemark = m->ui.lineEdit_remark_query->text();
}

void FinancialSoft::initPage1()
{
	QDateTime dtStart = QDateTime::currentDateTime();
	dtStart.setTime(QTime(0, 0, 0));
	m->ui.dateTimeEdit_query_start->setDateTime(dtStart);
	m->ui.dateTimeEdit_query_start->setDisplayFormat(DATE_TIME_FORMAT);
	m->ui.dateTimeEdit_query_end->setDateTime(QDateTime::currentDateTime());
	m->ui.dateTimeEdit_query_end->setDisplayFormat(DATE_TIME_FORMAT);

	QTableOperation::instance()->setTableWgt(m->ui.tableWidget);
}
void FinancialSoft::initPage2()
{
	//禁止修改凭证号
	m->ui.lineEdit_voucher_change->setDisabled(true);

	m->ui.dateEdit_change->setDisplayFormat(DATE_TIME_FORMAT);
}

void FinancialSoft::initStatusBar()
{
	m->ui.statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
	QLabel* pLabelBorrow = new QLabel("借方总金额(元):");
	m->pLabel_borrow = new QLabel("0");
	m->pLabel_borrow->setFixedWidth(60);
	QLabel* pLabelLoan = new QLabel("贷方总金额(元):");
	m->pLabel_loan = new QLabel("0");
	m->pLabel_loan->setFixedWidth(60);

	QLabel* pLabelSurplus = new QLabel("余额(元):");
	m->pLabel_surplus = new QLabel("0");
	m->pLabel_surplus->setFixedWidth(60);

	m->ui.statusBar->addPermanentWidget(pLabelBorrow);
	m->ui.statusBar->addPermanentWidget(m->pLabel_borrow);
	m->ui.statusBar->addPermanentWidget(pLabelLoan);
	m->ui.statusBar->addPermanentWidget(m->pLabel_loan);

	m->ui.statusBar->addPermanentWidget(pLabelSurplus);
	m->ui.statusBar->addPermanentWidget(m->pLabel_surplus);
}

void FinancialSoft::slotBtnToAddPageClicked()
{
	st_data_info dataInfo;
	dataInfo.initDateAndVoucher();
	
	setDataInfoToUI(dataInfo);
	toPage2(true);
}
void FinancialSoft::slotBtnGiveUpAndReturnClicked()
{
	toPage1();
}

void FinancialSoft::slotBtnAddToDbCLicked()
{
	st_data_info info;
	QString qstrErr = getDataInfoFromUI(info);

	if (!qstrErr.isEmpty())
	{
		NSComm::popErrorMsg(qstrErr);
	}
	else{
		bool bRet = QDBOperation::instance()->addOneData(info);
		if (bRet){
			NSComm::popTipMsg("数据添加成功");
			QTableOperation::instance()->addOneRow(info);
			//更新界面上的数据显示
			if (info.iMoneyBorrow){
				slotBorrowMoneyPlus(info.iMoneyBorrow);
			}
			if(info.iMoneyLoan){
				slotLoanMoneyPlus(info.iMoneyLoan);
			}
			toPage1();
		}
		else{
			NSComm::popTipMsg("数据添加失败");
		}
	}
}
void FinancialSoft::slotBtnSaveChgToDbClicked()
{
	st_data_info info;
	getDataInfoFromUI(info);
	info.iId = m->infoForEdit.iId;

	if (QDBOperation::instance()->updateDataByID(info.iId,info)){
		QTableOperation::instance()->updateRowInfo(m->iEditRow, info);

		int iMoneyBorrow = info.iMoneyBorrow - m->infoForEdit.iMoneyBorrow;
		int iMoneyLoan = info.iMoneyLoan - m->infoForEdit.iMoneyLoan;
		if (iMoneyBorrow)
		{
			slotBorrowMoneyPlus(iMoneyBorrow);
		}
		if (iMoneyLoan)
		{
			slotLoanMoneyPlus(iMoneyLoan);
		}
	}

	NSComm::popTipMsg("信息保存成功");
	slotBtnGiveUpAndReturnClicked();
}
void FinancialSoft::slotBtnDeleteClicked()
{
	QList<int> lstRows;
	for (int i = 0; i < m->ui.tableWidget->rowCount();i++)
	{
		QTableWidgetItem* pItem = m->ui.tableWidget->item(i, ROW_INDEX_DATE);
		if (pItem->checkState() == Qt::Checked){
			lstRows.push_back(i);
		}
	}

	if (lstRows.isEmpty()){
		NSComm::popTipMsg("请先选择要删除的项");
		return;
	}

	QList<int> lstIds;
	for (QList<int>::Iterator it = lstRows.begin(); it != lstRows.end();it++)
	{
		QTableWidgetItem* pId = m->ui.tableWidget->item(*it, ROW_INDEX_ID);
		lstIds.push_back(pId->text().toInt());
	}

	bool bRet = QDBOperation::instance()->deleteRows(lstIds);
	if (bRet)
	{
		int iMoneyBorrow = 0, iMoneyLoan = 0;
		for (QList<int>::Iterator it = lstRows.end(); it != lstRows.begin();)
		{
			it--;
			iMoneyBorrow += m->ui.tableWidget->item(*it, ROW_INDEX_BORROW)->text().toInt();
			iMoneyLoan += m->ui.tableWidget->item(*it, ROW_INDEX_LOAN)->text().toInt();
			m->ui.tableWidget->removeRow(*it);
		}
		slotBorrowMoneyMinus(iMoneyBorrow);
		slotLoanMoneyMinus(iMoneyLoan);
	}
	else{
		NSComm::popErrorMsg("删除选中数据失败");
	}

}
void FinancialSoft::slotBtnQueryClicked()
{
	st_query_info queryInfo;
	getQueryInfoFromUI(queryInfo);

	QTableOperation::instance()->queryInfo(queryInfo);
}

void FinancialSoft::slotEditDataInfo(int iRow, const st_data_info& info)
{
	m->iEditRow = iRow;
	m->infoForEdit = info;
	setDataInfoToUI(info);

	toPage2(false);
}

void FinancialSoft::slotBorrowMoneyChanged(int iTotal)
{
	m->pLabel_borrow->setText(QString::number(iTotal));
	updateSurplusMoney();
}
void FinancialSoft::slotLoanMoneyChanged(int iTotal)
{
	m->pLabel_loan->setText(QString::number(iTotal));
	updateSurplusMoney();
}

void FinancialSoft::slotBorrowMoneyMinus(int iNum)
{
	int iValue = m->pLabel_borrow->text().toInt() - iNum;
	m->pLabel_borrow->setText(QString::number(iValue));
	
	updateSurplusMoney();
}
void FinancialSoft::slotBorrowMoneyPlus(int iNum)
{
	int iValue = m->pLabel_borrow->text().toInt() + iNum;
	m->pLabel_borrow->setText(QString::number(iValue));

	updateSurplusMoney();
}
void FinancialSoft::slotLoanMoneyMinus(int iNum)
{
	int iValue = m->pLabel_loan->text().toInt() - iNum;
	m->pLabel_loan->setText(QString::number(iValue));

	updateSurplusMoney();
}
void FinancialSoft::slotLoanMoneyPlus(int iNum)
{
	int iValue = m->pLabel_loan->text().toInt() + iNum;
	m->pLabel_loan->setText(QString::number(iValue));

	updateSurplusMoney();
}
void FinancialSoft::updateSurplusMoney()
{
	int iBorrow = m->pLabel_borrow->text().toInt();
	int iLoan = m->pLabel_loan->text().toInt();
	m->pLabel_surplus->setText(QString::number(iBorrow - iLoan));
}