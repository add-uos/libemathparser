#ifndef EASY_PARSER_STD_H
#define EASY_PARSER_STD_H

#include <cmath>
#include <string>
#include <vector>
#include <regex>

#define PRESCION 20
#define Q_QNAN NAN
#define Q_INFINITY INFINITY

namespace ZHYSTD {

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
    FormulaItem(FormulaItemType Type, FormulaValue Value) {
        value = Value;
        type = Type;
    }

    FMULITEMTYPE_EM type;
    FMULITEMTYPE_VALUE value;
} FMULITEM_ST;

typedef struct ErrorInfo
{
    ErrorInfo(){
    }

    ErrorInfo(int Code, std::string Info, std::string Detail) {
        code = Code;
        info = Info;
        detail = Detail;
    }

    ErrorInfo(int Code, std::string Info) {
        code = Code;
        info = Info;
    }

    int code = 0;
    std::string info = "Not Init ERROR INFO";
    std::string detail = "Not Init ERROR DETAIL";
}   ERRORINFO_ST;

class EasyParserStd
{

public:
    EasyParserStd();

    static double eval(std::string formula, ERRORINFO_ST &info = E000);

    static double eval_sample_no_bracket(std::string formula, ERRORINFO_ST &info = E000);

    static bool check_legal(std::string formula, ERRORINFO_ST &info = E000);

private:
    //calucate two number
    static double eval_two_number_by_operator(const FormulaItem &num1,const FormulaItem &num2, const FormulaItem &opearator, ERRORINFO_ST &info = E000);
    static double eval_two_number_by_operator(const double&num1, const double &num2, const char &op, ERRORINFO_ST &info = E000);

    //get bracket list
    static std::vector<std::string> get_bracket_list(const std::string &match_string, bool save_bracket = false);

public:
    //replace function
    static std::string replace(const std::string &inputStr, const std::string replace_before, const std::string replace_after);
    static bool contains(const std::string & inputStr, const char need_find_char);
    static int count(const std::string & inputStr, const std::string need_find_str);

    // convert to double
    static double toDouble(const std::string &inputStr, bool *ok = nullptr);

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
}
#endif // EASY_PARSER_STD_H
