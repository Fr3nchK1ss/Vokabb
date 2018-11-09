#include <QtGui>
#include <QtWidgets>

#include "translatedialog.h"


/*************************************************
 * A dialog which displays a combobox to choose
 * the application's style.
 *************************************************/
/**
 * Constructor
 */
QTranslateDialog::QTranslateDialog( QWidget* parent ) : QDialog( parent )
{
	setAttribute(Qt::WA_DeleteOnClose);
	setMinimumWidth( 200 );

	QStringList languages;
	languages << "English" << "Francais" << "Svenska";
	m_styleComboBox = new QComboBox( this );
	m_styleComboBox->addItems( languages );

	QLabel* label = new QLabel( this );
	label->setText("The changes won't take effect\n before the application is restarted.");

	QPushButton* okBtt = new QPushButton(tr("OK"));
	okBtt->setIcon(style()->standardIcon( QStyle::SP_DialogOkButton ));

	QGridLayout *gLayout = new QGridLayout(this);
	gLayout->setContentsMargins( 10, 10, 10, 10);
	gLayout->addWidget( m_styleComboBox, 0, 0, 1, 3 );
	gLayout->addWidget( label, 1, 0, 1, 3);
	gLayout->setRowMinimumHeight( 2, 20);
	gLayout->addWidget( okBtt, 2, 1 );

	connect(m_styleComboBox, SIGNAL( activated(const QString &) ), this, SIGNAL( translate(const QString &) ));
	connect(okBtt, SIGNAL( clicked() ), this, SLOT( accept() ));
}

