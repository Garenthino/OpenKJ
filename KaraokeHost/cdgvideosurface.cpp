#include "cdgvideosurface.h"
#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QWidget>
#include <QPainter>
#include <QTransform>
#include <QDebug>

/*
 * This was a test to see if QAbstractVideoSurface use would be faster/more efficient than drawing to a glcanvas
 * directly.  Turns out this isn't the case at all.  This file is no longer in use and is only here for possible
 * later use.
 *
*/

CdgVideoSurface::CdgVideoSurface(QWidget *widget, QObject *parent)
{
    Q_UNUSED(parent);
    CdgVideoSurface::widget = widget;
}

QList<QVideoFrame::PixelFormat> CdgVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    qCritical() << "CdgVideoSurface::supportedPixelFormats called";
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool CdgVideoSurface::start(const QVideoSurfaceFormat &format)
{
    qCritical() << "CdgVideoSurface::start (wrong one) called";
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        imageSize = size;
        sourceRect = format.viewport();

        QAbstractVideoSurface::start(format);

        widget->updateGeometry();
        updateVideoRect();

        return true;
    } else {
        return false;
    }
}

bool CdgVideoSurface::start()
{
    QVideoSurfaceFormat format(QSize(300,216),QVideoFrame::Format_RGB24);
    qCritical() << "CdgVideoSurface::start called";
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        imageSize = size;
        sourceRect = format.viewport();

        QAbstractVideoSurface::start(format);

        widget->updateGeometry();
        updateVideoRect();

        return true;
    } else {
        qCritical() << "Error in CdgVideoSurface::start()";
        return false;
    }
}

void CdgVideoSurface::stop()
{
    currentFrame = QVideoFrame();
    targetRect = QRect();

    QAbstractVideoSurface::stop();

    widget->update();
}

bool CdgVideoSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        qCritical() << "CdgVideoSurface::present - Incorrect Format Error";
        qCritical() << "surfaceFormat = " << surfaceFormat().pixelFormat();
        qCritical() << "frameFormat = " << frame.pixelFormat();
        qCritical() << "surfaceSize = " << surfaceFormat().frameSize();
        qCritical() << "frameSize = " << frame.size();
        stop();

        return false;
    } else {
        currentFrame = frame;

        //widget->repaint(targetRect);
        widget->update();
        return true;
    }
}

void CdgVideoSurface::updateVideoRect()
{
    qCritical() << "CdgVideoSurface::updateVideoRect called";
    QSize size = surfaceFormat().sizeHint();
//    size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);
    size.scale(widget->size(), Qt::KeepAspectRatio);

    targetRect = QRect(QPoint(0, 0), size);
    targetRect.moveCenter(widget->rect().center());
}

void CdgVideoSurface::paint(QPainter *painter)
{
    if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform oldTransform = painter->transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter->scale(1, -1);
           painter->translate(0, -widget->height());
        }

        QImage image(
                currentFrame.bits(),
                currentFrame.width(),
                currentFrame.height(),
                currentFrame.bytesPerLine(),
                imageFormat);

        painter->drawImage(targetRect, image, sourceRect);

        painter->setTransform(oldTransform);

        currentFrame.unmap();
    }
}
