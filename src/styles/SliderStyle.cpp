#include "SliderStyle.h"

#include "../constants/Colors.h"

QString SliderStyle::primary()
{
    return QString(R"(
        QSlider {
            background-color: transparent;
        }

        QSlider::groove:horizontal {
            background: #383838;
            height: 4px;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            background: %1;
            width: 10px;
            height: 10px;
            margin: -3px 0;
            border-radius: 5px;
        }
        QSlider::sub-page:horizontal {
            background: %2;
            border-radius: 2px;
        }
    )")
        .arg(Colors::LightBlue)
        .arg(Colors::LightBlue);
}
