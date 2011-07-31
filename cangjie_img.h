#ifndef CANGJIE_IMG_H
#define CANGJIE_IMG_H

#include <QImage>
#include "bounding_box.h"

namespace CangjieImg
{
    void rotateAndCrop(const QImage& startImg, const BoundingBox box, QImage& endImg);
}


#endif // CANGJIE_IMG_H
