#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FinancialSoft.h"

class FinancialSoft : public QMainWindow
{
    Q_OBJECT

public:
    FinancialSoft(QWidget *parent = Q_NULLPTR);

private:
    Ui::FinancialSoftClass ui;
};
