#include "comutil.h"

#include <QPixmap>
#include <QtWin>

#include <olectl.h>

IPictureDisp *ComUtil::loadPicture(const QString &imagePath)
{
    QPixmap pixmap( imagePath );
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
