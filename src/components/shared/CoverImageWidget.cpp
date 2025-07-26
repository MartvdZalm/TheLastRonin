#include "CoverImageWidget.h"

#include <QVBoxLayout>

CoverImageWidget::CoverImageWidget(const QString& imagePath, QWidget* parent) : QWidget(parent)
{
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(300, 300);
    imageLabel->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
}

void CoverImageWidget::setImage(const QString& imagePath)
{
    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}
