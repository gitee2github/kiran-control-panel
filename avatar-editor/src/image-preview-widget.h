//
// Created by lxh on 2020/10/23.
//

#ifndef KIRAN_AVATAR_EDITOR_IMAGE_PREVIEW_WIDGET_H
#define KIRAN_AVATAR_EDITOR_IMAGE_PREVIEW_WIDGET_H

#include <QWidget>
#include <QPixmap>

class ImagePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImagePreviewWidget (QWidget *parent = nullptr);
    ~ImagePreviewWidget ();

    void setSrcImagePath (const QString &imagePath);
    QString srcImagePath ();

    void setDstImagePath (const QString &imagePath);
    QString dstImagePath ();

    bool saveAvatar ();

private:
    void loadSrcImage ();
    //图片位置校验
    bool imagePositionVerification ();
    //图片位置不合法，调整位置
    void imageAutomaticPosition ();
    //图片放大
    void pixmapEnlarge ();
    //图片缩小
    void pixmapReduce ();

    QPixmap generateCircleRectPixmap ();
    QPixmap generateCircleAvatar (const QPixmap &pixmap);

private Q_SLOTS:
    void pixmapCentering ();

protected:
    virtual void paintEvent (QPaintEvent *event) override;
    virtual void resizeEvent (QResizeEvent *event) override;
    virtual void mousePressEvent (QMouseEvent *event) override;
    virtual void mouseReleaseEvent (QMouseEvent *event) override;
    virtual void mouseMoveEvent (QMouseEvent *event) override;
    virtual void wheelEvent (QWheelEvent *event) override;

private:
    //原图像路径
    QString m_srcImagePath;
    //目标图像路径
    QString m_dstImagePath;
    //原始图片
    QPixmap m_originalPixmap;
    //当前拉伸的图片
    QPixmap m_scaledPixmap;
    //当前拉伸的比例
    double m_pixmapScaleFactor = 1.0;
    //图片的左顶点
    QPoint m_pixmapLefttop;
    bool m_btnIsPressed;
    QPoint m_btnPressedPoint;
};

#endif //KIRAN_AVATAR_EDITOR_IMAGE_PREVIEW_WIDGET_H
