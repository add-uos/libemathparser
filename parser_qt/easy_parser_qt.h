#ifndef EASY_PARSER_QT_H
#define EASY_PARSER_QT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtMath>
#include <QVariant>
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QRegularExpression>

#define PRESCION 20

typedef enum FormulaItemType{
    Operator,
    Operand
} FMULITEMTYPE_EM;

typedef union FormulaValue{
    FormulaValue(){
        oper = '\0';
    };
    FormulaValue(double Value){
        operand = Value;
    }

    FormulaValue(char Op){
        oper = Op;
    }
    char oper;
    double operand;
}FMULITEMTYPE_VALUE;

typedef struct FormulaItem
{
    FormulaItem(FMULITEMTYPE_EM Type, FMULITEMTYPE_VALUE Value) {
        type = Type;
        value = Value;
    }

    FMULITEMTYPE_EM type;
    FMULITEMTYPE_VALUE value;
} FMULITEM_ST;

typedef struct ErrorInfo
{
    ErrorInfo(){
    }

    ErrorInfo(int Code, QString Info, QString Detail) {
        code = Code;
        info = Info;
        detail = Detail;
    }

    ErrorInfo(int Code, QString Info) {
        code = Code;
        info = Info;
    }

    int code = 0;
    QString info = "Not Init ERROR INFO";
    QString detail = "Not Init ERROR DETAIL";
}   ERRORINFO_ST;

class EasyParserQt : QObject
{
    Q_OBJECT

public:
    EasyParserQt();

    static double eval(QString formula, ERRORINFO_ST &info = E000);

    static double eval_sample_no_bracket(QString formula, ERRORINFO_ST &info = E000);

    static bool check_legal(QString formula, ERRORINFO_ST &info = E000);

private:

    //calucate two number
    static double eval_two_number_by_operator(const FormulaItem &num1,const FormulaItem &num2, const FormulaItem &opearator, ERRORINFO_ST &info = E000);
    static double eval_two_number_by_operator(const double&num1, const double &num2, const char &op, ERRORINFO_ST &info = E000);

    //get bracket list
    static QStringList get_bracket_list(const QString &match_string, bool save_bracket = false);

public:
    // error code info
    static ERRORINFO_ST E000;
    static ERRORINFO_ST E001;
    static ERRORINFO_ST E002;
    static ERRORINFO_ST E003;
    static ERRORINFO_ST E004;
    static ERRORINFO_ST E005;
    static ERRORINFO_ST E006;
    static ERRORINFO_ST E007;
    static ERRORINFO_ST E008;
    static ERRORINFO_ST E009;
    static ERRORINFO_ST E010;
    static ERRORINFO_ST E011;
    static ERRORINFO_ST E012;
};

#endif // EASY_PARSER_QT_H
