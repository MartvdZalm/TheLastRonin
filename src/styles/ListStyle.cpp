#include "ListStyle.h"

QString ListStyle::primary()
{
    return R"(
        QListWidget {
            background: transparent;
            outline: none;
        }

        QListWidget::item {
            padding: 12px;
            border: none;
        }

        QListWidget::item:selected {
            background-color: #4a90e2;
            color: white;
        }

        QListWidget::item:hover {
            background-color: #005a8e;
        }
    )";
}
