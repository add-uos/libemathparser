#ifndef OPERAIONBYMUPARSER_H
#define OPERAIONBYMUPARSER_H
#include <QString>
#include "muParser.h"
#include <QtMath>

class OperaionByMuParser
{
public:
    OperaionByMuParser();

    //计算最终值
    static double ComputeExpressionValue(QString str);
};

#endif // OPERAIONBYMUPARSER_H
