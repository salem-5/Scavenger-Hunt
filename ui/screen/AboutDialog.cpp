#include "AboutDialog.h"
#include "core/Constants.h"
#include <QPainter>

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent) {
    const int ui = Constants::UI_SCALE;
    setFixedSize(176 * ui, 128 * ui);
    setWindowTitle("About Scavenger Hunt");
    m_icon.load(":/assets/icons/icon.png");
    m_bg.load(":/assets/sprites/ui/about.png");
}

void AboutDialog::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    const int ui = 3;
    painter.fillRect(rect(), QColor("#93A9CD"));
    int iconSize = 32 * ui*2;
    int iconX = (width() - iconSize) / 2;
    int iconY = 8 * ui;
    painter.drawPixmap(0, 0, 176*ui, 128*ui, m_bg);
    painter.drawPixmap(iconX, iconY, iconSize, iconSize, m_icon);

    QString titleText = "Scavenger Hunt";
    int bigCharW = 11 * ui;
    int titleW = titleText.length() * bigCharW;
    int titleX = (width() - titleW) / 2;
    int titleY = iconY + iconSize + 20 * ui;

    m_font.drawText(painter, titleText, titleX, titleY, ui, PixelFont::Dark, true);
    QString versionText = QString("Version %1").arg(APP_VERSION);
    int smallCharW = 6 * ui;
    int versionW = versionText.length() * smallCharW;
    int versionX = (width() - versionW) / 2;
    int versionY = titleY + 14 * ui;

    m_font.drawText(painter, versionText, versionX, versionY, ui, PixelFont::Dark, false);
    QDialog::paintEvent(event);
}