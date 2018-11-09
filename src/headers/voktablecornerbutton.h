#ifndef VOKTABLECORNERBUTTON_H
#define VOKTABLECORNERBUTTON_H


#include <QAbstractButton>

class QIcon;
class QRect;

class QvokTableCornerButton : public QAbstractButton
{
Q_OBJECT

//members
	QRect m_rectangle;
	QIcon m_icon;
		
//functions
public:
	QvokTableCornerButton( QRect rect, QIcon icon=QIcon(), QWidget *parent=0 );

protected:
	void paintEvent(QPaintEvent*);
};

#endif
