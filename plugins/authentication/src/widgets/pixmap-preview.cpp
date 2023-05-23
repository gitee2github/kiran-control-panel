#include "pixmap-preview.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QResizeEvent>

PixmapPreview::PixmapPreview(QWidget* parent)
    : QWidget(parent)
{
}

PixmapPreview::~PixmapPreview()
{
}

void PixmapPreview::setPixmap(const QString& path)
{
    if (!m_pixmap.load(path) && !path.isEmpty())
    {
        qWarning() << path << "pixmap load failed" << path;
    }
    adjustPixmap(size());
    update();
}

void PixmapPreview::setPixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
    adjustPixmap(size());
    update();
}

void PixmapPreview::adjustPixmap(QSize size)
{
    if (m_pixmap.isNull())
    {
        return;
    }

    m_scaledPixmap = m_pixmap.scaled(size, Qt::KeepAspectRatioByExpanding);
}

void PixmapPreview::resizeEvent(QResizeEvent* event)
{
    adjustPixmap(event->size());
    QWidget::resizeEvent(event);
}

void PixmapPreview::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int radius = qMin(size().width(), size().height()) / 2;
    QRect roundedRect(0, 0, radius * 2, radius * 2);
    roundedRect.moveCenter(rect().center());

    QPainterPath painterPath;
    painterPath.addRoundedRect(roundedRect.x(), roundedRect.y(),
                               roundedRect.width(), roundedRect.height(),
                               radius, radius);

    QBrush backgroundBrush(QColor("#393939"));
    painter.fillPath(painterPath,backgroundBrush);

    if (!m_scaledPixmap.isNull())
    {
        auto pixmapRect = m_scaledPixmap.rect();
        pixmapRect.moveCenter(rect().center());
        painter.drawPixmap(pixmapRect, m_scaledPixmap);
    }

    QWidget::paintEvent(event);
}
