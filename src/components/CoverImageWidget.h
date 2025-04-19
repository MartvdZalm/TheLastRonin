#ifndef COVERIMAGEWIDGET_H
#define COVERIMAGEWIDGET_H

#include <QWidget>
#include <QLabel>

class CoverImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoverImageWidget(const QString& imagePath, QWidget* parent = nullptr);

private:
    QLabel* imageLabel;
};

#endif // COVERIMAGEWIDGET_H
