#include "AppStyle.h"

QString AppStyle::styleSheet()
{
    return R"(
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

        QComboBox::down-arrow {
            image: url(:/Images/ArrowDown);
        }
    )";
}
