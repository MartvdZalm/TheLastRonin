#include "ButtonStyle.h"

QString ButtonStyle::styleSheet()
{
    return R"(
        QPushButton {
            background-color: #0078d4;  /* Nice blue color */
            color: white;
            border: none;
            border-radius: 8px;
            padding: 5px 20px;
            font-size: 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #005a8e;  /* Darker blue when hovered */
        }

        QPushButton:pressed {
            background-color: #003f69;  /* Even darker blue when pressed */
        }
    )";
}
