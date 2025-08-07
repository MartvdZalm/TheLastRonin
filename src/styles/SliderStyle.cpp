#include "SliderStyle.h"

QString SliderStyle::primary()
{
    return R"(
        QSlider {
            background-color: transparent;
        }

        QSlider::groove:horizontal {
            background: #383838;
            height: 4px;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: #4a90e2;
            width: 10px;
            height: 10px;
            margin: -3px 0;
            border-radius: 5px;
        }
        QSlider::sub-page:horizontal {
            background: #4a90e2;
            border-radius: 2px;
        }
    )";
}
