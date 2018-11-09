
#include <QtWidgets>

#include "copytab.h"

#define EDITTAB 0
#define COPYTAB 1
#define EXAMTAB 2

/**
 * @brief: Constructor of QCopyTab
 */
QCopyTab::QCopyTab(int blankCount, bool displayOnlyColumn2, QWidget *parent) : QWidget(parent)
{
	m_displayOnlyColumn2 = displayOnlyColumn2;

	QLabel* copy = new QLabel(tr("Number of lines :"), this);
	QGroupBox *viewGroupBox = new QGroupBox(tr("View"));

	m_linesSpinBox = new QSpinBox(this);
	m_linesSpinBox->setMaximumWidth(50);
	m_linesSpinBox->setRange(0, 99);
	m_linesSpinBox->setValue(blankCount);

	m_oneColBtt = new QPushButton(this);
	m_oneColBtt->setFlat(true);
	m_oneColBtt->setIconSize( QSize( 32, 32) );
	m_oneColBtt->setIcon( QIcon(":/onecol.icon") );

	m_colLabel = new QLabel( this );

	m_twoColBtt = new QPushButton(this);
	m_twoColBtt->setFlat(true);
	m_twoColBtt->setIconSize( QSize( 32, 32) );
	m_twoColBtt->setIcon( QIcon(":/twocol.icon") );


	//setup view
	QVBoxLayout* vCopyLayout = new QVBoxLayout;
	vCopyLayout->setContentsMargins(20, 40, 20, 40);
		QHBoxLayout* hLayout1 = new QHBoxLayout();
		hLayout1->addWidget(copy);
		hLayout1->addWidget(m_linesSpinBox);
	vCopyLayout->addLayout( hLayout1 );
	vCopyLayout->addStretch();
		QHBoxLayout* hLayout2 = new QHBoxLayout();
		hLayout2->addWidget(m_oneColBtt);
		hLayout2->addWidget( m_colLabel );
		hLayout2->addWidget(m_twoColBtt);
		viewGroupBox->setLayout(hLayout2);
	vCopyLayout->addWidget( viewGroupBox );
	setLayout(vCopyLayout);

	//inner connections
	connect(m_linesSpinBox, SIGNAL( valueChanged(int) ), this, SIGNAL( launchCopy(int) ));

	if ( displayOnlyColumn2 )
	{
		m_colLabel->setText( "<=            " );
		connect(m_twoColBtt, SIGNAL( clicked() ), this, SLOT( displayTwoCol() ));
	}
	else
	{
		m_colLabel->setText( "            =>" );
		connect(m_oneColBtt, SIGNAL(clicked()), this, SLOT( displayOneCol() ));
	}
}


/********************************
 * accessors
 ********************************/
const int QCopyTab::getBlankRowsCount()
{
	return m_linesSpinBox->value();
}
bool QCopyTab::getDisplayOnlyColumn2()
{
	return m_displayOnlyColumn2;
}


/*****************************************
 * private SLOTS
 *****************************************/
void QCopyTab::displayOneCol()
{
	disconnect(m_oneColBtt, SIGNAL(clicked()), this, SLOT( displayOneCol() ));
	connect(m_twoColBtt, SIGNAL( clicked() ), this, SLOT( displayTwoCol() ));
	m_colLabel->setText( "<=            " );
	m_displayOnlyColumn2 = true;
	emit colCountChanged();
}

void QCopyTab::displayTwoCol()
{
	disconnect(m_twoColBtt, SIGNAL( clicked() ), this, SLOT( displayTwoCol() ));
	connect(m_oneColBtt, SIGNAL(clicked()), this, SLOT( displayOneCol() ));
	m_colLabel->setText( "            =>" );
	m_displayOnlyColumn2 = false;
	emit colCountChanged();
}

