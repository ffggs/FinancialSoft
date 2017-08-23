#include "FinancialSoft.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FinancialSoft w;
    w.show();
    return a.exec();
}
