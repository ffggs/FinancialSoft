#pragma once
#include <QString>
#define NAMESPACE_BEGIN(ns) namespace ns{
#define NAMESPACE_END() }
NAMESPACE_BEGIN(NSComm)
void popTipMsg(const QString& strTip);
void popErrorMsg(const QString& strTip);
NAMESPACE_END()

