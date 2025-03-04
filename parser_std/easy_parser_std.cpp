#include "easy_parser_std.h"

namespace EPARSERSTD {

/*
 * No Errors Occurred.
 */
ERRORINFO_ST EasyParserStd::E000 = ErrorInfo(0, "No Error", std::string("No Errors Occurred"));
/*
 * Bracket '(' Does Not Equal To ').
 */
ERRORINFO_ST EasyParserStd::E001 = ErrorInfo(1, std::string("Bracket Error"));
/*
 * Operator Does Not Match Operand. Operator's Count = Operand's Count - 1.
 */
ERRORINFO_ST EasyParserStd::E002 = ErrorInfo(2, std::string("Operator And Operand Error"));
/*
 * a operand like '-2.4354', '14.54', '5654'.., which should be parsed correctly.
 */
ERRORINFO_ST EasyParserStd::E003 = ErrorInfo(3, std::string("Operand Parse Error"));
/*
 * a operator like '+', '-', '*'.., which should be parsed correctly.
 */
ERRORINFO_ST EasyParserStd::E004 = ErrorInfo(4, std::string("Operator Parse Error"));
/*
 * a number should not be parse to Nan.
 */
ERRORINFO_ST EasyParserStd::E005 = ErrorInfo(5, std::string("Nan Error"));
/*
 * a number should not be parse to Infinite.
 */
ERRORINFO_ST EasyParserStd::E006 = ErrorInfo(6, std::string("Infinite Error"));
/*
 * The Dividend Should Not Be Zero.
 */
ERRORINFO_ST EasyParserStd::E007 = ErrorInfo(7, std::string("Division By Zero Error"));
/*
 * Operands now only support: '+', '-'. '*', '/', '^'.
 */
ERRORINFO_ST EasyParserStd::E008 = ErrorInfo(8, std::string("Undefined Operands"));
/*
 * The Operator Should Be Located At An Even Numbered index.
 */
ERRORINFO_ST EasyParserStd::E009 = ErrorInfo(9, std::string("Operator Index Error"));
/*
 * The operand should be in an odd digit index.
 */
ERRORINFO_ST EasyParserStd::E010 = ErrorInfo(10, std::string("Operand Index Error"));
/*
 * The Power function can't handel the base is negative and the index is not a positive integer.
 */
ERRORINFO_ST EasyParserStd::E011 = ErrorInfo(11, std::string("Power Function Error"));
/*
 * The final number of calculation results is not 1.
 */
ERRORINFO_ST EasyParserStd::E012 = ErrorInfo(12, std::string("UnKnow Error"));

EasyParserStd::EasyParserStd() {}

double EasyParserStd::eval(std::string formula , ERRORINFO_ST &info)
{
    // remove blank character.
    formula = EasyParserStd::replace(formula, " ", "");

    double eval_res = Q_QNAN;

    if(check_legal(formula, info)){

        // 1.extract bracket list, like ((2.43+4.45) * 56.6 + (65.354-3.44))^2
        while (EasyParserStd::contains(formula, '(') || (EasyParserStd::contains(formula, ')'))) {
            std::vector<std::string> Bracket = get_bracket_list(formula);

            for (auto it: Bracket) {
                double tmp_res = eval_sample_no_bracket(it, info);

                formula = EasyParserStd::replace(formula, "("+it+")", std::to_string(tmp_res));
            }
        }

        // 2. caluate the last statement
        eval_res = eval_sample_no_bracket(formula, info);
    }

    return eval_res;
}

double EasyParserStd::EasyParserStd::eval_sample_no_bracket(std::string formula, ERRORINFO_ST &info)
{
    double eval_res = Q_QNAN;

    // remove ' '
    formula =  EasyParserStd::replace(formula, " ", "");

    // extract formula item
    std::vector<FMULITEM_ST> FormulaList;

    // QPair(level,index)
    std::vector<std::pair<int, int>> OperatorLevelList;

    // segmente item to operator and operand
    std::string tmpstr;
    for (int i = 0; i < formula.length(); ++i) {
        if(formula.at(i) == '+' || formula.at(i) == '-' ||
            formula.at(i) == '*' || formula.at(i) == '/' ||
            formula.at(i) == '^'){

            // handel the negative number is in the first position
            if((formula.at(0) == '+' || formula.at(0) == '-') && i == 0){
                tmpstr.push_back(formula.at(i));
                continue;
            }

            // handel negative number in operator like 2--3
            if(i > 0 && (
                    formula.at(i-1) == '+' || formula.at(i-1) == '-' ||
                    formula.at(i-1) =='*' || formula.at(i-1) == '/' ||
                    formula.at(i-1) == '^' ||
                    formula.at(i-1) == 'E' || formula.at(i-1) == 'e'))
            {
                tmpstr.push_back(formula.at(i));
                continue;
            }

            // before number
            bool cvtOK = false;
            double res = EasyParserStd::toDouble(tmpstr, &cvtOK);
            if(cvtOK){
                FormulaList.push_back(FormulaItem(FMULITEMTYPE_EM::Operand, FMULITEMTYPE_VALUE(res)));
            }
            else{
                // report error
                info = E003;
                info.detail = EasyParserStd::replace("%1 Parse To Double Failed!", "%1",  tmpstr);

                return eval_res;
            }

            tmpstr.clear();

            // opterator
            FormulaList.push_back(FormulaItem(FMULITEMTYPE_EM::Operator,  FMULITEMTYPE_VALUE(formula.at(i))));

            continue;
        }

        tmpstr.push_back(formula.at(i));

        if(i == formula.length() - 1){
            // before number
            bool cvtOK = false;
            double res = EasyParserStd::toDouble(tmpstr, &cvtOK);
            if(cvtOK){
                FormulaList.push_back(FormulaItem(FMULITEMTYPE_EM::Operand, res));
            }
            else{
                // report error
                info = E003;
                info.detail = EasyParserStd::replace("%1 Parse To Double Failed!", "%1", tmpstr);

                return eval_res;
            }
        }
    }

    // Check the order of operands and operators
    for (int i = 0; i < FormulaList.size(); ++i) {
        if(i % 2 == 0){
            if(FormulaList.at(i).type != FMULITEMTYPE_EM::Operand){
                info = E009;
                info.detail = "The Operator Should Be Located At An Even Numbered index! Index(" + std::to_string(i+1) + "-"
                              + std::string(FormulaList.at(i).value.oper, 1);

                return eval_res;
            }
        }else {
            if(FormulaList.at(i).type != FMULITEMTYPE_EM::Operator){
                info = E010;
                info.detail = "The operand should be in an odd digit index! Index(" + std::to_string(i+1) + "-"
                              + std::string(FormulaList.at(i).value.oper, 1);

                return eval_res;
            }
        }
    }

    // caluate two item
    while (FormulaList.size() != 1) {
        // 1. get operatorLevelist
        OperatorLevelList.clear();
        for (int i = 0; i < FormulaList.size(); i++) {
            if(FormulaList.at(i).type == FMULITEMTYPE_EM::Operator){
                char op = FormulaList.at(i).value.oper;
                switch (op) {
                case '+':
                case '-':
                    OperatorLevelList.push_back(std::pair<int, int>(1, i));
                    break;
                case '*':
                case '/':
                    OperatorLevelList.push_back(std::pair<int, int>(2, i));
                    break;
                case '^':
                    OperatorLevelList.push_back(std::pair<int, int>(3, i));
                    break;

                default:
                    // report error
                    info = E008;
                    info.detail = EasyParserStd::replace("Operands Now Only Support: '+', '-'. '*', '/', '^'. But You Give:", "%1",   std::string(1, op));

                    return eval_res;
                }
            }
        }
        std::sort(OperatorLevelList.begin(),OperatorLevelList.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.first > b.first; });

        // 2. caluate
        if(FormulaList.size() <= OperatorLevelList.front().second){
            // report error
            info = E002;
            info.detail = "Operator(%1) Does Not Match Operand(%2). Operator's Count = Operand's Count - 1" + std::to_string(FormulaList.size() - OperatorLevelList.size()) + std::to_string(OperatorLevelList.size());

            return eval_res;
        }

        if(OperatorLevelList.front().second < 1){
            // report error
            info = E002;
            info.detail = "Operator(%1) Does Not Match Operand(%2). Operator's Count = Operand's Count - 1" + std::to_string(FormulaList.size() - OperatorLevelList.size()) + std::to_string(OperatorLevelList.size());

            return eval_res;
        }

        double value = eval_two_number_by_operator(FormulaList.at(OperatorLevelList.front().second - 1),
                                                   FormulaList.at(OperatorLevelList.front().second + 1),
                                                   FormulaList.at(OperatorLevelList.front().second), info);

        // 3.check info
        if(info.code != 0){
            return eval_res;
        }

        FormulaList.erase(FormulaList.begin() + OperatorLevelList.front().second - 1);
        FormulaList.erase(FormulaList.begin() + OperatorLevelList.front().second - 1);
        FormulaList.erase(FormulaList.begin() + OperatorLevelList.front().second - 1);

        FormulaList.insert(FormulaList.begin() + OperatorLevelList.front().second - 1, FormulaItem(FMULITEMTYPE_EM::Operand, value));
    }

    if(FormulaList.size() != 1){
        // report error
        info = E012;
        info.detail = "The final number of calculation results is not 1.";

        return eval_res;
    }

    eval_res = FormulaList.front().value.operand;

    return eval_res;
}

bool EasyParserStd::check_legal(std::string formula, ERRORINFO_ST &info)
{
    bool is_legal = true;

    //before handel
    formula = EasyParserStd::replace(formula, " ", "");

    // bracket check
    if(EasyParserStd::count(formula, "(") !=EasyParserStd::count(formula, ")")) {
        // report error
        info = E001;
        info.detail = "Bracket '(' Does Not Equal To ')";

        is_legal = false;
    }

    formula = EasyParserStd::replace(formula, "(", "");
    formula = EasyParserStd::replace(formula, ")", "");
    formula = EasyParserStd::replace(formula, "++", "+");
    formula = EasyParserStd::replace(formula, "--", "-(0-1)*");
    formula = EasyParserStd::replace(formula, "+-", "+(0-1)*");
    formula = EasyParserStd::replace(formula, "*-", "*(0-1)*");
    formula = EasyParserStd::replace(formula, "/-", "/(0-1)*");
    formula = EasyParserStd::replace(formula, "^-", "^");

    if(formula.at(0) == '-')    formula = formula.substr(1, formula.size() - 1);

    //operator check
    std::vector<int> oper_list;
    std::vector<std::string> operand_list;

    std::string tmpstr;
    for (int i = 0; i < formula.size(); ++i) {
        if(formula.at(i) == '+' || formula.at(i) == '-' ||
            formula.at(i) == '*' || formula.at(i) == '/' ||
            formula.at(i) == '^'){
            oper_list.push_back(i);

            operand_list.push_back(tmpstr);
            tmpstr.clear();
        }
        else {
            tmpstr.push_back(formula.at(i));

            if(i == formula.length() - 1){
                operand_list.push_back(tmpstr);
            }
        }
    }

    if(oper_list.size() != operand_list.size() - 1){
        // report error
        info = E002;
        info.detail = "Operator("+ std::to_string(oper_list.size())+ ")Does Not Match Operand(" +  std::to_string(operand_list.size()) +") Operator's Count = Operand's Count - 1";

        is_legal = false;
    }

    return is_legal;
}

double EasyParserStd::eval_two_number_by_operator(const FormulaItem &num1, const FormulaItem &num2, const FormulaItem &opearator, ERRORINFO_ST &info)
{
    return eval_two_number_by_operator(num1.value.operand,  num2.value.operand, opearator.value.oper, info);
}

double EasyParserStd::eval_two_number_by_operator(const double &num1, const double &num2, const char &op, ERRORINFO_ST &info)
{
    // first check NaN and -NAN
    if(std::abs(num1) == Q_QNAN)     {
        // report error
        info = E005;
        info.detail = EasyParserStd::replace("%1 a number should not be parse to Nan.", "%1",  std::to_string(num1));

        return Q_QNAN;
    }

    if(std::abs(num2) == Q_QNAN)     {
        // report error
        info = E005;
        info.detail = EasyParserStd::replace("%1 a number should not be parse to Nan.", "%1",  std::to_string(num2));

        return Q_QNAN;
    }

    // second check INFITE and -INFITE
    if(std::abs(num1) == Q_INFINITY)     {
        // report error
        info = E006;
        info.detail = EasyParserStd::replace("%1 a number should not be parse to Infinite!", "%1",  std::to_string(num1));

        return Q_INFINITY;
    }

    if(std::abs(num2) == Q_INFINITY)     {
        // report error
        info = E006;
        info.detail = EasyParserStd::replace("%1 a number should not be parse to Infinite!", "%1", std::to_string(num2));

        return Q_INFINITY;
    }

    double res = Q_QNAN;

    switch (op) {
    case '+':
        res = num1 + num2;
        break;
    case '-':
        res = num1 - num2;
        break;
    case '*':
        res = num1 * num2;
        break;
    case '/':
        if(num2 == 0){
            // report error
            info = E007;
            info.detail = "The Dividend Should Not Be Zero";

            return res;
        }
        res = num1 / num2;

        break;
    case '^':
        if(num1 < 0 && fabs((int)num2-num2) < 1E+8){
            // report error
            info = E011;
            info.detail = "The Power function can't handel the base is negative and the index is not a positive integer.";

            return res;
        }
        res = std::pow(num1, num2);
        break;

    default:
        // report error
        info = E008;
        info.detail = EasyParserStd::replace("Operands Now Only Support: '+', '-'. '*', '/', '^'. But You Give: %1", "%1", std::string(1, op));

        break;
    }

    return res;
}

//search bracket expression, like  (a+b*c)
std::vector<std::string> EasyParserStd::get_bracket_list(const std::string &match_string, bool save_bracket)
{
    std::vector<std::string>                     listResult;

    std::regex word_regex("\\({1}[^()]*\\){1}");
    auto words_begin =
        std::sregex_iterator(match_string.begin(), match_string.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();

        if(save_bracket){
            listResult.push_back(match_str);
        }
        else {
            listResult.push_back(EasyParserStd::replace(EasyParserStd::replace(match_str, "(", ""), ")", ""));
        }
    }

    return listResult;
}

std::string EasyParserStd::replace(const std::string &inputStr, const std::string replace_before, const std::string replace_after)
{
    std::string outputStr = inputStr;
    std::string::size_type pos = 0;

    while((pos = outputStr.find(replace_before)) != std::string::npos)   //替换所有指定子串
    {
        outputStr.replace(pos, replace_before.length(), replace_after);
    }

    return outputStr;
}

bool EasyParserStd::contains(const std::string &inputStr, const char need_find_char)
{
    bool finded = false;

    auto result = std::find(inputStr.begin(), inputStr.end(), need_find_char);

    if(result != inputStr.end())    finded = true;

    return finded;
}

int EasyParserStd::count(const std::string &inputStr, const std::string need_find_str)
{
    int count = 0;

    std::string::size_type pos = 0;

    while ((pos = inputStr.find(need_find_str, pos)) != std::string::npos) {
        ++count;
        pos += need_find_str.length();
    }

    return count;
}

double EasyParserStd::toDouble(const std::string &inputStr, bool *ok)
{
    double res = 0;
    bool cvt_ok = true;

    try {
        res = std::stod(inputStr);
    } catch (...) {
        cvt_ok = false;
    }

    if(ok != nullptr)
        *ok = cvt_ok;

    return res;
}
}
