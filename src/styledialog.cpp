#include <QtGui>
#include <QtWidgets>

#include "styledialog.h"


/*************************************************
 * A dialog which displays a combobox to choose
 * the application's style.
 *************************************************/
/**
 * Constructor
 */
QStyleDialog::QStyleDialog( QWidget* parent ) : QDialog( parent )
{
	setAttribute(Qt::WA_DeleteOnClose);
	setMinimumWidth( 200 );

	m_styleComboBox = new QComboBox( this );
	m_styleComboBox->addItems( QStyleFactory::keys() );

	QPushButton* okBtt = new QPushButton(tr("OK"));
	//okBtt->setMaximumWidth( 50 );
	okBtt->setIcon(style()->standardIcon( QStyle::SP_DialogOkButton ));

	QGridLayout *gLayout = new QGridLayout(this);
	gLayout->setContentsMargins( 10, 10, 10, 10);
	gLayout->addWidget( m_styleComboBox, 0, 0, 1, 3 );
	gLayout->setRowMinimumHeight( 1, 20);
	gLayout->addWidget( okBtt, 2, 1 );

	connect(m_styleComboBox, SIGNAL( activated(const QString &) ), this, SLOT( changeStyle(const QString &) ));
	connect(okBtt, SIGNAL( clicked() ), this, SLOT( accept() ));
}


/**********************************
 * private SLOTS
 **********************************/
void QStyleDialog::changeStyle(const QString &styleName)
{/*
	if (styleName == "NorwegianWood") 
	{
		QApplication::setStyle(new NorwegianWoodStyle);
	} 
	else 
	{*/
		QApplication::setStyle( QStyleFactory::create(styleName) );
	//}
 }
