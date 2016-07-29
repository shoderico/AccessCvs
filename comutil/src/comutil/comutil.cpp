#include "comutil.h"

#include <QPixmap>
#include <QtWin>

#include <QSvgRenderer>
#include <QPainter>


#include <olectl.h>
#include <ocidl.h>

#include "pch.hpp"

IPictureDisp *ComUtil::loadPicture(const QString &imagePath)
{
    QPixmap pixmap( imagePath );
    return loadPicture(pixmap);
}

IPictureDisp *ComUtil::loadPicture(const QPixmap &pixmap)
{
    HBITMAP hbm = QtWin::toHBITMAP( pixmap, QtWin::HBitmapAlpha );

    IPictureDisp  *pd;
    PICTDESC pdesc;
    memset( &pdesc, 0, sizeof(pdesc) );
    pdesc.cbSizeofstruct = sizeof(pdesc);
    pdesc.picType = PICTYPE_BITMAP;

    pdesc.bmp.hbitmap = hbm;

    pdesc.bmp.hpal = NULL;
    HRESULT hr = OleCreatePictureIndirect( &pdesc, IID_IPicture, TRUE, (void**)&pd );
    //::DeleteObject(hbm);  // no need to delete object

    if ( FAILED(hr) )
        pd = NULL;

    return pd;
}

IPictureDisp *ComUtil::loadPictureFromSvg(const QString &imagePath, const QSize &size)
{
    QPixmap pixmap = renderSvg(imagePath, size);
    return loadPicture( pixmap );
}

QPixmap ComUtil::renderSvg(const QString &imagePath, const QSize &size)
{
    QSvgRenderer renderer( imagePath );
    QPixmap pixmap( size );
    pixmap.fill( Qt::transparent );
    QPainter painter( &pixmap );
    renderer.render( &painter );
    return pixmap;
}
