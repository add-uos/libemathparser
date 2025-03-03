#include "easy_parser_qt.h"

/*
 * No Errors Occurred.
 */
ERRORINFO_ST EasyParserQt::E000 = ErrorInfo(0, "No Error", "No Errors Occurred");
/*
 * Bracket '(' Does Not Equal To ').
 */
ERRORINFO_ST EasyParserQt::E001 = ErrorInfo(1, "Bracket Error");
/*
 * Operator Does Not Match Operand. Operator's Count = Operand's Count - 1.
 */
ERRORINFO_ST EasyParserQt::E002 = ErrorInfo(2, "Operator And Operand Error");
/*
 * a operand like '-2.4354', '14.54', '5654'.., which should be parsed correctly.
 */
ERRORINFO_ST EasyParserQt::E003 = ErrorInfo(3, "Operand Parse Error");
/*
 * a operator like '+', '-', '*'.., which should be parsed correctly.
 */
ERRORINFO_ST EasyParserQt::E004 = ErrorInfo(4, "Operator Parse Error");
/*
 * a number should not be parse to Nan.
 */
ERRORINFO_ST EasyParserQt::E005 = ErrorInfo(5, "Nan Error");
/*
 * a number should not be parse to Infinite.
 */
ERRORINFO_ST EasyParserQt::E006 = ErrorInfo(6, "Infinite Error");
/*
 * The Dividend Should Not Be Zero.
 */
ERRORINFO_ST EasyParserQt::E007 = ErrorInfo(7, "Division By Zero Error");
/*
 * Operands now only support: '+', '-'. '*', '/', '^'.
 */
ERRORINFO_ST EasyParserQt::E008 = ErrorInfo(8, "Undefined Operands");
/*
 * The Operator Should Be Located At An Even Numbered index.
 */
ERRORINFO_ST EasyParserQt::E009 = ErrorInfo(9, "Operator Index Error");
/*
 * The operand should be in an odd digit index.
 */
ERRORINFO_ST EasyParserQt::E010 = ErrorInfo(10, "Operand Index Error");
/*
 * The Power function can't handel the base is negative and the index is not a positive integer.
 */
ERRORINFO_ST EasyParserQt::E011 = ErrorInfo(11, "Power Function Error");
/*
 * The final number of calculation results is not 1.
 */
ERRORINFO_ST EasyParserQt::E012 = ErrorInfo(12, "UnKnow Error");

EasyParserQt::EasyParserQt() {}

double EasyParserQt::eval(QString formula , ERRORINFO_ST &info)
{
    // remove blank character.
    formula.replace(" ", "");

    double eval_res = Q_QNAN;

    if(check_legal(formula, info)){

        // 1.extract bracket list, like ((2.43+4.45) * 56.6 + (65.354-3.44))^2
        while (formula.contains("(") || formula.contains(")")) {
            QStringList Bracket = get_bracket_list(formula);

            foreach (auto it, Bracket) {
                double tmp_res = eval_sample_no_bracket(it, info);

                formula.replace(tr("(%1)").arg(it), QString::number(tmp_res, 'g', PRESCION));
            }
        }

        // 2. caluate the last statement
        eval_res = eval_sample_no_bracket(formula, info);
    }

    return eval_res;
}

double EasyParserQt::eval_sample_no_bracket(QString formula, ERRORINFO_ST &info)
{
    double eval_res = Q_QNAN;

    // remove ' '
    formula.replace(" ","");

    // extract formula item
    QList<FMULITEM_ST> FormulaList;

    // QPair(level,index)
    QList<QPair<int, int>> OperatorLevelList;

    // segmente item to operator and operand
    QString tmpstr;
    for (int i = 0; i < formula.length(); ++i) {
        if(formula.at(i) == QChar('+') || formula.at(i) == QChar('-') ||
            formula.at(i) == QChar('*') || formula.at(i) == QChar('/') || formula.at(i) == QChar('^')){

            // handel the negative number is in the first position
            if((formula.at(0) == QChar('+') || formula.at(0) == QChar('-')) && i == 0){
                tmpstr.append(formula.at(i));
                continue;
            }

            // handel negative number in operator like 2--3
            if(i > 0 && (
                          formula.at(i-1) == QChar('+') || formula.at(i-1) == QChar('-') ||
                          formula.at(i-1) == QChar('*') || formula.at(i-1) == QChar('/') ||
                          formula.at(i-1) == QChar('^') || formula.at(i-1) == QChar('E') || formula.at(i-1) == QChar('e')))
            {
                tmpstr.append(formula.at(i));
                continue;
            }

            // before number
            bool cvtOK = false;
            double res = tmpstr.toDouble(&cvtOK);
            if(cvtOK){
                FormulaList.append(FMULITEM_ST(FMULITEMTYPE_EM::Operand, FMULITEMTYPE_VALUE(res)));
            }
            else{
                // report error
                info = E003;
                info.detail = tr("%1 Parse To Double Failed!").arg(tmpstr);

                return eval_res;
            }

            tmpstr.clear();

            // opterator
            FormulaList.append(FMULITEM_ST(FMULITEMTYPE_EM::Operator,  FMULITEMTYPE_VALUE(formula.at(i).toLatin1())));

            continue;
        }

        tmpstr.append(formula.at(i));

        if(i == formula.length() - 1){
            // before number
            bool cvtOK = false;
            double res = tmpstr.toDouble(&cvtOK);
            if(cvtOK){
                FormulaList.append(FormulaItem(FMULITEMTYPE_EM::Operand, res));
            }
            else{
                // report error
                info = E003;
                info.detail = tr("%1 Parse To Double Failed!").arg(tmpstr);

                return eval_res;
            }
        }
    }

    // Check the order of operands and operators
    for (int i = 0; i < FormulaList.size(); ++i) {
        if(i % 2 == 0){
            if(FormulaList.at(i).type != FMULITEMTYPE_EM::Operand){
                info = E009;
                info.detail = tr("The Operator Should Be Located At An Even Numbered index! Index(%1)-%2").arg(i+1)
                                  .arg(FormulaList.at(i).value.oper);

                return eval_res;
            }
        }else {
            if(FormulaList.at(i).type != FMULITEMTYPE_EM::Operator){
                info = E010;
                info.detail = tr("The operand should be in an odd digit index! Index(%1)-%2").arg(i+1)
                                  .arg(FormulaList.at(i).value.oper);

                return eval_res;
            }
        }
    }

    // caluate two item
    while (FormulaList.length() != 1) {
        // 1. get operatorLevelist
        OperatorLevelList.clear();
        for (int i = 0; i < FormulaList.length(); i++) {
            if(FormulaList.at(i).type == FMULITEMTYPE_EM::Operator){
                char op = FormulaList.at(i).value.oper;
                switch (op) {
                case '+':
                case '-':
                    OperatorLevelList.append(QPair<int, int>(1, i));
                    break;
                case '*':
                case '/':
                    OperatorLevelList.append(QPair<int, int>(2, i));
                    break;
                case '^':
                    OperatorLevelList.append(QPair<int, int>(3, i));
                    break;

                default:
                    // report error
                    info = E008;
                    info.detail = tr("Operands Now Only Support: '+', '-'. '*', '/', '^'. But You Give: %1").arg(op);

                    return eval_res;
                }
            }
        }
        std::sort(OperatorLevelList.begin(),OperatorLevelList.end(), [](const QPair<int, int>& a, const QPair<int, int>& b) {
            return a.first > b.first; });

        // 2. caluate
        if(FormulaList.length() <= OperatorLevelList.first().second){
            // report error
            info = E002;
            info.detail = tr("Operator(%1) Does Not Match Operand(%2). Operator's Count = Operand's Count - 1").arg(FormulaList.length() - OperatorLevelList.length()).arg(OperatorLevelList.length());

            return eval_res;
        }

        if(OperatorLevelList.first().second < 1){
            // report error
            info = E002;
            info.detail = tr("Operator(%1) Does Not Match Operand(%2). Operator's Count = Operand's Count - 1").arg(FormulaList.length() - OperatorLevelList.length()).arg(OperatorLevelList.length());

            return eval_res;
        }

        double value = eval_two_number_by_operator(FormulaList.at(OperatorLevelList.first().second - 1),
                                                   FormulaList.at(OperatorLevelList.first().second + 1),
                                                   FormulaList.at(OperatorLevelList.first().second), info);

        // 3.check info
        if(info.code != 0){
            return eval_res;
        }

        FormulaList.removeAt(OperatorLevelList.first().second - 1);
        FormulaList.removeAt(OperatorLevelList.first().second - 1);
        FormulaList.removeAt(OperatorLevelList.first().second - 1);

        FormulaList.insert(OperatorLevelList.first().second - 1, FormulaItem(FMULITEMTYPE_EM::Operand, value));
    }

    if(FormulaList.length() != 1){
        // report error
        info = E012;
        info.detail = tr("The final number of calculation results is not 1.");

        return eval_res;
    }

    eval_res = FormulaList.first().value.operand;

    return eval_res;
}

bool EasyParserQt::check_legal(QString formula, ERRORINFO_ST &info)
{
    bool is_legal = true;

    //before handel
    formula = formula.trimmed();
    formula.replace(" ", "");

    // bracket check
    if(formula.count("(") != formula.count(")")) {
        // report error
        info = E001;
        info.detail = tr("Bracket '(' Does Not Equal To ')");

        is_legal = false;
    }

    formula.replace("(","").replace(")", "");

    formula.replace("++", "+");
    formula.replace("--", "-(0-1)*");
    formula.replace("+-", "+(0-1)*");
    formula.replace("*-", "*(0-1)*");
    formula.replace("/-", "/(0-1)*");
    formula.replace("^-", "^");
    if(formula.at(0) == '-')    formula.remove(0,1);

    //operator check
    QStringList items = formula.split(QRegExp("[\\+\\-\\*\\^\\/]"), QString::SkipEmptyParts);
    if(formula.count(QRegExp("[\\+\\-\\*\\^\\/]")) != items.length()-1){
        // report error
        info = E002;
        info.detail = tr("Operator(%1) Does Not Match Operand(%2). Operator's Count = Operand's Count - 1").arg(formula.count(QRegExp("[\\+\\-\\*\\^\\/]"))).arg(items.length());

        is_legal = false;
    }

    return is_legal;
}

double EasyParserQt::eval_two_number_by_operator(const FormulaItem &num1, const FormulaItem &num2, const FormulaItem &opearator, ERRORINFO_ST &info)
{
    return eval_two_number_by_operator(num1.value.operand,  num2.value.operand, opearator.value.oper, info);
}

double EasyParserQt::eval_two_number_by_operator(const double &num1, const double &num2, const char &op, ERRORINFO_ST &info)
{
    // first check NaN and -NAN
    if(abs(num1) == Q_QNAN)     {
        // report error
        info = E005;
        info.detail = tr("%1 a number should not be parse to Nan.").arg(QString::number(num1));

        return Q_QNAN;
    }

    if(abs(num2) == Q_QNAN)     {
        // report error
        info = E005;
        info.detail = tr("%1 a number should not be parse to Nan.").arg(QString::number(num2));

        return Q_QNAN;
    }

    // second check INFITE and -INFITE
    if(abs(num1) == Q_INFINITY)     {
        // report error
        info = E006;
        info.detail = tr("%1 a number should not be parse to Infinite!").arg(QString::number(num1));

        return Q_INFINITY;
    }

    if(abs(num2) == Q_INFINITY)     {
        // report error
        info = E006;
        info.detail = tr("%1 a number should not be parse to Infinite!").arg(QString::number(num2));

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
            info.detail = tr("The Dividend Should Not Be Zero");

            return res;
        }
        res = num1 / num2;

        break;
    case '^':
        if(num1 < 0 && fabs((int)num2-num2) < 1E+8){
            // report error
            info = E011;
            info.detail = tr("The Power function can't handel the base is negative and the index is not a positive integer.");

            return res;
        }
        res = qPow(num1, num2);
        break;

    default:
        // report error
        info = E008;
        info.detail = tr("Operands Now Only Support: '+', '-'. '*', '/', '^'. But You Give: %1").arg(op);

        break;
    }

    return res;
}

//search bracket expression, like  (a+b*c)
QStringList EasyParserQt::get_bracket_list(const QString & match_string, bool save_bracket)
{
    QStringList                     listResult;
    QRegularExpression              regExp( QString( "\\({1}[^()]*\\){1}" ) );
    QRegularExpressionMatchIterator itRegMatch = regExp.globalMatch( match_string, 0, QRegularExpression::MatchType::NormalMatch );
    while ( itRegMatch.hasNext() ) {
        QRegularExpressionMatch regMatch = itRegMatch.next();
        if(save_bracket){
            listResult.append( regMatch.captured( 0 ) );
        }
        else {
            listResult.append( regMatch.captured( 0 ).replace("(", "").replace(")", "") );
        }
    }
    return listResult;
}
