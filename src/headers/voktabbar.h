#ifndef VOKTABBAR_H
#define VOKTABBAR_H

#include <QTabBar>

class QvokTabBar : public QTabBar
{
//use of signals/slots
Q_OBJECT

//functions
public:
	QvokTabBar(QWidget* parent = 0);
	QSize tabSizeHint ( int index ) const;
};

#endif
