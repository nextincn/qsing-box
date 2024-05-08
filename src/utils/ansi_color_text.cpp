#include "ansi_color_text.h"

#include <QRegularExpression>

// Parsing ANSI colors and dispalys
void AnsiColorText::appendAnsiColorText(QPlainTextEdit *textEdit,
                                        const QString &text) {
    QString formattedText;
    QTextCharFormat defaultFormat;
    QColor color;

    QTextCursor cursor(textEdit->document());
    cursor.movePosition(QTextCursor::End);

    QStringList segments = text.split("\033[");
    for (const QString &segment : segments) {
        int colorIndex = segment.indexOf('m');
        if (colorIndex != -1) {
            QString colorCode = segment.left(colorIndex);
            QStringList codes = colorCode.split(';');

            if (codes.size() == 1 && codes[0] == "0") {
                // Reset formatting to default
                cursor.setCharFormat(defaultFormat);
            } else if (codes.size() == 1) {
                switch (codes[0].toInt()) {
                case 31:
                    color = Qt::red;
                    break;
                case 32:
                    color = Qt::green;
                    break;
                case 36:
                    color = Qt::blue;
                    break;
                default:
                    break;
                }
                // Handle ANSI color code
                QTextCharFormat format;
                format.setForeground(color);
                cursor.setCharFormat(format);
            } else if (codes.size() == 3 && codes[0] == "38"
                       && codes[1] == "5") {
                // Handle ANSI color code with 8-bit color mode (38;5;...)
                int colorValue = codes[2].toInt();
                QTextCharFormat format;
                format.setForeground(QColor::fromHsv(colorValue, 255, 210));
                cursor.setCharFormat(format);
            }

            QString textPart = segment.mid(colorIndex + 1);
            cursor.insertText(textPart);
        } else {
            cursor.insertText(segment);
        }
    }

    defaultFormat.setForeground(textEdit->palette().text().color());
    cursor.setCharFormat(defaultFormat);
}
