#include <QtGui>
#include <QtWidgets>

#include "voktabwidget.h"
#include "voktabbar.h"

/*******************************************************************
* Replace the default QTabWidget to access protected setTab function
********************************************************************/

QvokTabWidget::QvokTabWidget(QWidget* parent) : QTabWidget(parent)
{	
	setMaximumHeight(250);
	QvokTabBar* tabBar = new QvokTabBar(this);
	setTabBar( tabBar );
}
