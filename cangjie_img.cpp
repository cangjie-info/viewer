#include "cangjie_img.h"

void CangjieImg::rotateAndCrop(const QImage& startImg, const BoundingBox box, QImage& endImg)
{
    QTransform tf;
    tf.rotate(box.getRotation());
    QRect selection = box;
    endImg = (startImg.transformed(tf)) . copy(selection);
}
