#include <QtGui>
#include <QtWidgets>

#include "voktablecornerbutton.h"

/**
 * Constructor
 * @arg rect : the rectangle representing the button
 * @arg icon : the icon decoration of the button
 */
QvokTableCornerButton::QvokTableCornerButton( QRect rect, QIcon icon, QWidget *parent) : QAbstractButton(parent)
{
	m_rectangle = rect;
	m_icon = icon;
}



void QvokTableCornerButton::paintEvent(QPaintEvent*)
{
	//QStyleOptionHeader opt;
	QStyleOptionButton opt;//
	QStyle::State state = QStyle::State_None;
	if (isEnabled())
		state |= QStyle::State_Enabled;
	if (isActiveWindow())
		state |= QStyle::State_Active;
	if (isDown())
		state |= QStyle::State_Sunken;
	opt.state = state;
	//opt.orientation = Qt::Horizontal;
	//opt.position = QStyleOptionHeader::OnlyOneSection;
	opt.rect = m_rectangle;
	opt.iconSize = QSize(28,28);//
	opt.icon = m_icon;
	//opt.iconAlignment = Qt::AlignCenter;
	QPainter painter(this);
	style()->drawControl(/*QStyle::CE_Header*/QStyle::CE_PushButton, &opt, &painter, this);
}
