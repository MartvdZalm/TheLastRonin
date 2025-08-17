#include "ButtonStyle.h"

#include "../constants/Colors.h"

QString ButtonStyle::primary()
{
    return QString(R"(
        QPushButton {
            background-color: %1;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 5px 20px;
            font-size: 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: %2;
        }

        QPushButton:pressed {
            background-color: %3;
        }
    )")
        .arg(Colors::LightBlue)
        .arg(Colors::MediumBlue)
        .arg(Colors::DarkBlue);
}

QString ButtonStyle::remove()
{
    return QString(R"(
        QPushButton {
            background-color: %1;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 5px 20px;
            font-size: 16px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: %2;
        }

        QPushButton:pressed {
            background-color: %3;
        }
    )")
        .arg(Colors::LightRed)
        .arg(Colors::MediumRed)
        .arg(Colors::DarkRed);
}
