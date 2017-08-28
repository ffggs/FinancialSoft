#pragma execution_character_set("utf-8")

#include "FinancialSoft.h"
#include "ui_FinancialSoft.h"
#include "Comm/CommFunc.h"
#include "TableOperation/QTableOperation.h"
#include "DBOperation/QDBOperation.h"

class FinancialSoftPri{
	friend class FinancialSoft;
	Ui::FinancialSoftClass ui;	
};
FinancialSoft::FinancialSoft(QWidget *parent)
	: QMainWindow(parent),m(new FinancialSoftPri())
{
	m->ui.setupUi(this);
	initPage1();
	initPage2();
	initSignalSlots();
	toPage1();
}

void FinancialSoft::initSignalSlots()
{
	connect(m->ui.pushButton_add_page, &QPushButton::clicked, this, &FinancialSoft::slotBtnToAddPageClicked);
	connect(m->ui.pushButton_add_info, &QPushButton::clicked, this, &FinancialSoft::slotBtnAddToDbCLicked);
	connect(m->ui.pushButton_save, &QPushButton::clicked, this, &FinancialSoft::slotBtnSaveChgToDbClicked);
	connect(m->ui.pushButton_return, &QPushButton::clicked, this, &FinancialSoft::slotBtnGiveUpAndReturnClicked);
}

void FinancialSoft::toPage1()
{
	m->ui.stackedWidget->setCurrentWidget(m->ui.page1);
	m->ui.lineEdit_subject1_query->setFocus();
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
	m->ui.dateEdit_change->setDate(QDate::fromString(info.strDate, Qt::ISODate));
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
		info.strDate = m->ui.dateEdit_change->date().toString("yyyy-MM-dd");
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

void FinancialSoft::initPage1()
{
	m->ui.dateEdit_query->setDate(QDate::currentDate());
	m->ui.dateEdit_query->setDisplayFormat("yyyy-MM-dd");

	QTableOperation::instance()->setTableWgt(m->ui.tableWidget);
}
void FinancialSoft::initPage2()
{
	//禁止修改凭证号
	m->ui.lineEdit_voucher_change->setDisabled(true);

	m->ui.dateEdit_change->setDisplayFormat("yyyy-MM-dd");
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

		}
		else{
			NSComm::popTipMsg("数据添加失败");
		}
	}
}
void FinancialSoft::slotBtnSaveChgToDbClicked()
{

}