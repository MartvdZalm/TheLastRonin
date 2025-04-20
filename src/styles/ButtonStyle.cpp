#include "ButtonStyle.h"

QString ButtonStyle::styleSheet()
{
    return R"(
        QPushButton {
            background-color: #4a90e2;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 5px 20px;
            font-size: 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #005a8e;
        }

        QPushButton:pressed {
            background-color: #003f69;
        }
    )";
}
