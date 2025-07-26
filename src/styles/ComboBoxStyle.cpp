#include "ComboBoxStyle.h"

QString ComboBoxStyle::primary()
{
    return R"(
        QComboBox {
            background-color: #2e2e2e;
            color: white;
            border: 1px solid #444;
            border-radius: 5px;
            padding: 5px 10px;
            min-width: 150px;
        }
        QComboBox:hover {
            border: 1px solid #666;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 25px;
            border-left: 1px solid #444;
        }
        QComboBox::down-arrow {
            width: 12px;
            height: 12px;
        }
        QComboBox QAbstractItemView {
            background-color: #1e1e1e;
            color: white;
            border: 1px solid #444;
            selection-background-color: #3e3e3e;
        }
    )";
}
