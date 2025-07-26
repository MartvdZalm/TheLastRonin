#include "ButtonStyle.h"

QString ButtonStyle::primary()
{
    return R"(
        QPushButton {
            background-color: #4a90e2;
            color: white;
            border: none;
            border-radius: 4px;
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

QString ButtonStyle::remove()
{
    return R"(
        QPushButton {
            background-color: #ff090e;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 5px 20px;
            font-size: 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #cc0000;
        }

        QPushButton:pressed {
            background-color: #990000;
        }
    )";
}
