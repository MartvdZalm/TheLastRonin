#include "ListStyle.h"

#include "../constants/Colors.h"

QString ListStyle::primary()
{
    return QString(R"(
        QListWidget {
            background: transparent;
            outline: none;
        }

        QListWidget::item {
            padding: 12px;
            border: none;
        }

        QListWidget::item:selected {
            background-color: %1;
            color: white;
        }

        QListWidget::item:hover {
            background-color: %2;
        }
    )")
        .arg(Colors::LightBlue)
        .arg(Colors::MediumBlue);
}
