#pragma execution_character_set("utf-8")
#include "CommFunc.h"
#include <QMessageBox>
NAMESPACE_BEGIN(NSComm)
void popTipMsg(const QString& strTip)
{
	QMessageBox msgBox;
	msgBox.setText(strTip);
	msgBox.exec();
}
void popErrorMsg(const QString& strTip)
{

}
NAMESPACE_END()
