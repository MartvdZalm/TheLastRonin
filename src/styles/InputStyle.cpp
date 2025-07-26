#include "InputStyle.h"

QString InputStyle::primary()
{
    return R"(
        QLineEdit {
            border: 1px solid #444;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            background-color: #1e1e1e;
            color: #f0f0f0;
        }

        QLineEdit:focus {
            border: 1px solid #3b82f6;
            background-color: #2a2a2a;
            outline: none;
        }
    )";
}
