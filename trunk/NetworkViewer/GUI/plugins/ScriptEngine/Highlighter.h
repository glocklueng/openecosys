#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

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



#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
     Highlighter(QTextDocument *parent = 0);

protected:
     void highlightBlock(const QString &text);

private:

     //Structure with format matching a pattern
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };

     //All rules
     QVector<HighlightingRule> highlightingRules;
     QTextCharFormat keywordFormat;
     QTextCharFormat commentFormat;
};

#endif

