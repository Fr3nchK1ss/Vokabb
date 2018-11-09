#include <QtGui>

#include "voktabbar.h"

/**********************************************************
* Replace the default QTabBar to set a clean tab size
**********************************************************/

QvokTabBar::QvokTabBar(QWidget* parent):QTabBar(parent)
{	
	setUsesScrollButtons(false);
}


QSize QvokTabBar::tabSizeHint ( int /*index*/ ) const
{
	float tabWidth = 260 / count();//linked to the size of leftWidget()

	QSize tabSize(tabWidth, height());
	return tabSize;
}
