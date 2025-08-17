#include "InputStyle.h"

#include "../constants/Colors.h"

QString InputStyle::primary()
{
    return QString(R"(
        QLineEdit {
            border: 1px solid #444;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            background-color: %1;
            color: #f0f0f0;
        }

        QLineEdit:focus {
            border: 1px solid #3b82f6;
            background-color: %2;
            outline: none;
        }
    )")
        .arg(Colors::DarkerGray)
        .arg(Colors::DarkGray);
}
