#include "AppStyle.h"

QString AppStyle::styleSheet()
{
    return R"(
        QScrollBar:vertical {
            background: #1e1e1e;
            width: 14px;
            margin: 0px;
            border: none;
            border-radius: 7px;
        }
        
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #4a4a4a, stop:1 #5a5a5a);
            min-height: 30px;
            border-radius: 7px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #6a6a6a, stop:1 #7a7a7a);
        }
        
        QScrollBar::handle:vertical:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3a3a3a, stop:1 #4a4a4a);
        }
        
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            border: none;
            background: none;
            height: 0px;
        }
        
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }
        
        QScrollBar:horizontal {
            background: #1e1e1e;
            height: 14px;
            margin: 0px;
            border: none;
            border-radius: 7px;
        }
        
        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4a4a4a, stop:1 #5a5a5a);
            min-width: 30px;
            border-radius: 7px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #6a6a6a, stop:1 #7a7a7a);
        }
        
        QScrollBar::handle:horizontal:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a3a3a, stop:1 #4a4a4a);
        }
        
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            border: none;
            background: none;
            width: 0px;
        }
        
        QScrollBar::add-page:horizontal,
        QScrollBar::sub-page:horizontal {
            background: none;
        }
        
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0px;
        }
        
        QScrollBar::handle:vertical {
            background: rgba(255, 255, 255, 0.3);
            border-radius: 4px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: rgba(255, 255, 255, 0.5);
        }
        
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
            border: none;
        }
    )";
}
