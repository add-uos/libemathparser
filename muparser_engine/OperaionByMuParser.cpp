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
#if defined(_UNICODE)
        m_parser.SetExpr(str.toStdWString());
#else
        m_parser.SetExpr(str.toStdString());
#endif
        return m_parser.Eval();
    }

    catch (mu::Parser::exception_type &e)
    {
        //上报错误
#if defined(_UNICODE)
        std::cout << QString::fromStdWString(e.GetMsg()).toStdString() << std::endl;
#else
        std::cout << QString::fromStdString(e.GetMsg()).toStdString() << std::endl;
#endif
        //返回值
        return Q_QNAN;
    }
}
