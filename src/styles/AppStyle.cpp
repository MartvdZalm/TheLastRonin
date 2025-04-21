#include "AppStyle.h"
#include "ButtonStyle.h"
#include "InputStyle.h"

QString AppStyle::styleSheet()
{
    QString base = R"(
        QWidget {
            background-color: #252525;
            color: #e0e0e0;
            font-family: 'Segoe UI', Roboto, sans-serif;
            font-size: 14px;
        }

        QScrollBar:vertical {
            background: #2c2c2c;
            width: 12px;
            margin: 16px 0 16px 0;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical {
            background: #555;
            min-height: 20px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical:hover {
            background: #777;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            background: none;
            height: 0px;
        }
    )";

    return base + InputStyle::styleSheet() + ButtonStyle::styleSheet();
}
