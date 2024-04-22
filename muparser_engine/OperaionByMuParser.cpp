#include "OperaionByMuParser.h"

OperaionByMuParser::OperaionByMuParser()
{

}

double OperaionByMuParser::ComputeExpressionValue(QString str)
{
    try
    {
        double result = 0;

        mu::Parser m_parser;

        m_parser.SetExpr(str.toStdString());

        return m_parser.Eval();
    }

    catch (mu::Parser::exception_type &e)
    {
        //上报错误
        std::cout << QString::fromStdString(e.GetMsg()).toStdString() << std::endl;

        //返回值
        return Q_QNAN;
    }
}
