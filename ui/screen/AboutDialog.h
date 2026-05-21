#pragma once
#include <QDialog>
#include <QPixmap>
#include "ui/sprite/PixelFont.h"

class AboutDialog : public QDialog {
    Q_OBJECT
public:
    explicit AboutDialog(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    PixelFont m_font;
    QPixmap m_icon;
    QPixmap m_bg;
};