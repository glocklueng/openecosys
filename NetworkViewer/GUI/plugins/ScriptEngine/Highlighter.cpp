/**
     Copyright (C) 2009-2011 IntRoLab
     http://introlab.gel.usherbrooke.ca
     Dominic Letourneau, ing. M.Sc.A.
     Dominic.Letourneau@USherbrooke.ca

     This file is part of OpenECoSys/NetworkViewer.
     OpenECoSys/NetworkViewer is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by the Free Software
     Foundation, either version 3 of the License, or (at your option) any later version.
     OpenECoSys/NetworkViewer is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
     or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
     You should have received a copy of the GNU General Public License along with
     OpenECoSys/NetworkViewer. If not, see http://www.gnu.org/licenses/.

 */

#include "Highlighter.h"

//http://code.google.com/p/fw4spl/source/browse/SrcLib/core/fwGuiQt/src/fwGuiQt/highlighter/PythonHighlighter.cpp?spec=svnf51423f270e6db51fefd07f1e0075d73f3ff969a&r=f51423f270e6db51fefd07f1e0075d73f3ff969a

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    //Javascript keywords
    keywordFormat.setForeground(QColor(0, 153, 255));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    keywordPatterns << "\\bbreak\\b";
    keywordPatterns << "\\bcase\\b";
    keywordPatterns << "\\bcatch\\b";
    keywordPatterns << "\\bcontinu\\be";
    keywordPatterns << "\\bdefaul\\bt";
    keywordPatterns << "\\bdelete\\b";
    keywordPatterns << "\\bdo\\b";
    keywordPatterns << "\\belse\\b";
    keywordPatterns << "\\bfinally\\b";
    keywordPatterns << "\\bfor\\b";
    keywordPatterns << "\\bfunction\\b";
    keywordPatterns << "\\bif\\b";
    keywordPatterns << "\\bin\\b";
    keywordPatterns << "\\binstanceof\\b";
    keywordPatterns << "\\bnew\\b";
    keywordPatterns << "\\breturn\\b";
    keywordPatterns << "\\bswitch\\b";
    keywordPatterns << "\\bthis\\b";
    keywordPatterns << "\\bthrow\\b";
    keywordPatterns << "\\btry\\b";
    keywordPatterns << "\\btypeof\\b";
    keywordPatterns << "\\bvar\\b";
    keywordPatterns << "\\bvoid\\b";
    keywordPatterns << "\\bwhile\\b";
    keywordPatterns << "\\bwith\\b";
    keywordPatterns << "\\btrue\\b";
    keywordPatterns << "\\bfalse\\b";
    keywordPatterns << "\\bnull\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //Additional NetworkViewer keywords
    keywordFormat.setForeground(QColor(50, 100, 200));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList localKeywordPatterns;
    localKeywordPatterns
            << "\\baddScriptVariable\\b"
            << "\\baddToPlot\\b"
            << "\\baddSliderControl\\b"
            << "\\bcreatePseudoModule\\b"
            << "\\baddPseudoModule\\b"
            << "\\btime\\b"
            << "\\bcycle\\b"
            << "\\bmodules\\b"
            << "\\bvariables\\b"
            << "\\bprint\\b";

    foreach (const QString &pattern, localKeywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Operators
    QTextCharFormat operatorsFormat;
    operatorsFormat.setForeground(Qt::cyan);
    QStringList operatorsPatterns;
    operatorsPatterns << "="
                    // Comparison
                    << "==" << "!=" << "<" << "<=" << ">" << ">="
                    // Arithmetic
                    << "\\+" << "-" << "\\*" << "/" << "//" << "\\%" << "\\*\\*"
                    // In-place
                    << "\\+=" << "-=" << "\\*=" << "/=" << "\\%="
                    // Bitwise
                    << "\\^" << "\\|" << "\\&" << "\\~" << ">>" << "<<";

    foreach(const QString &pattern, operatorsPatterns)
    {
        rule.pattern = QRegExp( pattern );
        rule.format = operatorsFormat;
        highlightingRules.append(rule);
    }

    // Braces
    QTextCharFormat bracesFormat;
    bracesFormat.setForeground(Qt::darkGray);
    QStringList bracesPatterns;
    bracesPatterns << "\\{" << "\\}" << "\\(" << "\\)" << "\\[" << "\\]";
    foreach(const QString &pattern, bracesPatterns)
    {
        rule.pattern = QRegExp( pattern );
        rule.format = bracesFormat;
        highlightingRules.append(rule);
    }

    // 'class' followed by an identifier
    QTextCharFormat classFormat;
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bclass\\b\\s*(\\w+)");
    rule.format = classFormat;
    highlightingRules.append(rule);


    // Numeric literals
    QTextCharFormat numericFormat;
    QColor col;
    col.setNamedColor("brown");
    numericFormat.setForeground(col);
    rule.format = numericFormat;
    rule.pattern = QRegExp("\\b[+-]?[0-9]+[lL]?\\b");
    highlightingRules.append(rule);
    rule.pattern = QRegExp("\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b");
    highlightingRules.append(rule);
    rule.pattern = QRegExp("\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b");
    highlightingRules.append(rule);

    //Strings
    commentFormat.setForeground(QColor(255, 0, 0));
    rule.pattern = QRegExp("\".*\"");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    //Single line comments
    commentFormat.setForeground(QColor(255, 132, 0));
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

}

//Process every rule and highlight text if matches
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

   //Block terminated
   setCurrentBlockState(0);
}

