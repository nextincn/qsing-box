#ifndef ANSI_COLOR_TEXT_H
#define ANSI_COLOR_TEXT_H

#include <QPlainTextEdit>

class AnsiColorText
{
public:
    static void appendAnsiColorText(QPlainTextEdit *textEdit,
                                    const QString &text);
};

#endif // ANSI_COLOR_TEXT_H
