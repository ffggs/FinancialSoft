#include "FinancialSoft.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>


void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & msg/*QtMsgType type, const char *msg*/)
{
	QString txtMessage;

	switch (type)
	{
	case QtDebugMsg:    //������Ϣ��ʾ
		txtMessage = QString("Debug: %1").arg(msg);
		break;

	case QtWarningMsg:    //һ���warning��ʾ
		txtMessage = QString("Warning: %1").arg(msg);
		break;

	case QtCriticalMsg:    //���ش�����ʾ
		txtMessage = QString("Critical: %1").arg(msg);
		break;

	case QtInfoMsg:    //���ش�����ʾ
		txtMessage = QString("Info: %1").arg(msg);
		break;

	case QtFatalMsg:    //����������ʾ
		txtMessage = QString("Fatal: %1").arg(msg);
		abort();
	}

	//������������Ϣ��ָ���ļ�
	QFile outputFile("log.txt");
	outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream textStream(&outputFile);
	textStream << txtMessage << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	qInstallMessageHandler(customMessageHandler);

    FinancialSoft w;
    w.show();
    return a.exec();
}
