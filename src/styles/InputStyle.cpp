#include "InputStyle.h"

QString InputStyle::styleSheet()
{
    return R"(
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 14px;
            background-color: #fdfdfd;
        }

        QLineEdit:focus {
            border: 1px solid #0078d4;
            background-color: #ffffff;
            outline: none;
        }
    )";
}
