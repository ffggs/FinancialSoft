#pragma once
#include "DBOperation/QDBOperation.h"

#include <QtWidgets/QMainWindow>
#include <QSharedPointer>

class FinancialSoftPri;
class FinancialSoft : public QMainWindow
{
    Q_OBJECT

public:
    FinancialSoft(QWidget *parent = Q_NULLPTR);

private:
	void initPage1();
	void initPage2();
	void initSignalSlots();

	void toPage1();
	void toPage2(bool bAdd);

	void setDataInfoToUI(const st_data_info& info);
	QString getDataInfoFromUI(st_data_info& info);

private slots:
	void slotBtnToAddPageClicked();
	void slotBtnGiveUpAndReturnClicked();
	void slotBtnAddToDbCLicked();
	void slotBtnSaveChgToDbClicked();
private:
	QSharedPointer<FinancialSoftPri> m;
};
