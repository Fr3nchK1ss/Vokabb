#include <QtGui>

#include "examtab.h"

#define EDITTAB 0
#define COPYTAB 1
#define EXAMTAB 2

/**
 * @brief: Constructor of QExamTab
 */
QExamTab::QExamTab(QString vocName, int testCount, int boxChecked, QWidget *parent) : QWidget(parent)
{
	//select the number of words for the test
	m_wordsSpinBox = new QSpinBox();
	m_wordsSpinBox->setRange(1, 99);
	m_wordsSpinBox->setValue( testCount );
	m_wordsSpinBox->setMaximumWidth(50);

	//to select directly all the words of a topic
	m_maxBtt = new QToolButton(this);
	m_maxBtt->setMaximumSize( QSize( 20, 20 ) );
	m_maxBtt->setIcon( QIcon(":/spinmax.icon") );

	//languages of the test
	m_translationGroupBox = new QGroupBox(tr("Translation"));
	m_translationRadio1 = new QRadioButton();//String set by setLanguages()
	m_translationRadio2 = new QRadioButton();//String set by setLanguages()
	m_translationRadio3 = new QRadioButton(tr("Mix"));
	switch(boxChecked)
	{
		case 1:
			m_translationRadio1->setChecked(true);break;
		case 2:
			m_translationRadio2->setChecked(true);break;
		case 3:
			m_translationRadio3->setChecked(true);break;
		default:
			break;//never reached
	}

	//Launch or end the Test
	m_startStopBtt = new QPushButton(tr("Launch the test"));
	m_isTest = false;

	this->setupViews();
	this->setLanguages(vocName);

	//inner connections
	connect(m_maxBtt, SIGNAL( clicked() ), this, SLOT( setMaxValue() ));
	connect(m_startStopBtt, SIGNAL( clicked() ), this, SLOT( toggleBtt() ));
}


void QExamTab::setupViews()
{
	QLabel* wordsLabel = new QLabel(tr("Maximum of lines :"));
	
	/*Define the layout*/
	QVBoxLayout* vExamLayout = new QVBoxLayout;
		QHBoxLayout* hLayout1 = new QHBoxLayout;
		hLayout1->addWidget(wordsLabel);
		hLayout1->addWidget(m_wordsSpinBox);
		hLayout1->addWidget( m_maxBtt );
	vExamLayout->addLayout(hLayout1);
	vExamLayout->addStretch();
		QVBoxLayout* vGroupBox = new QVBoxLayout;
			QHBoxLayout* hLayout2 = new QHBoxLayout;
			hLayout2->addWidget( m_translationRadio1 );
			hLayout2->addWidget( m_translationRadio2 );
		vGroupBox->addLayout( hLayout2 );
		vGroupBox->addWidget( m_translationRadio3);
		m_translationGroupBox->setLayout(vGroupBox);
	vExamLayout->addWidget(m_translationGroupBox);
	vExamLayout->addStretch();
		QHBoxLayout* hExamLayout2 = new QHBoxLayout;
		hExamLayout2->addStretch();
		hExamLayout2->addWidget(m_startStopBtt);
		hExamLayout2->addStretch();
	vExamLayout->addLayout(hExamLayout2);
	vExamLayout->setContentsMargins( 20, 20, 20, 20);
	setLayout(vExamLayout);
}


/******************************
 * accessors
 ******************************/
const int QExamTab::getTestCount()
{
	return m_wordsSpinBox->value();
}
const short QExamTab::getBoxChecked()
{
	if (m_translationRadio1->isChecked())
		return 1;
	else if (m_translationRadio2->isChecked())
		return 2;
	else
		return 3;
}


/********************************************
 * private SLOTS
 ********************************************/
void QExamTab::setMaxValue()
{
	m_wordsSpinBox->setValue(99);
}



/*********************************************
 * public SLOTS
 *********************************************/
/**
 * If the tab is changed during a test, the test is automatically terminated.
 *
 */
void QExamTab::stopExam()
{
	if( m_isTest )
	{
		m_startStopBtt->setText(tr("Launch the test"));
		m_isTest = false;
		m_translationGroupBox->setDisabled( false );
		m_wordsSpinBox->setDisabled( false );
	}
}


/**
 * 
 */
void QExamTab::toggleBtt()
{
	if(m_isTest)
	{
		m_startStopBtt->setText(tr("Launch the test"));
		m_isTest = false;
		m_translationGroupBox->setDisabled(false);
		m_wordsSpinBox->setDisabled( false );

	}
	else
	{
		m_startStopBtt->setText(tr(" Result "));
		m_isTest = true;
		m_translationGroupBox->setDisabled(true);
		m_wordsSpinBox->setDisabled( true );
	}
	
	emit startStopTest( this->getTestCount(), this->getBoxChecked(), m_isTest);
}


/**
 * 
 */
void QExamTab::setLanguages(QString vocName)
{
	QStringList tokens = (vocName).split("__");
	QString lang1 = tokens.value(0);
	QString lang2 = tokens.value(1);
	
	QString radio1; 
	radio1.append(tr(" to ")).append(lang2);
	m_translationRadio1->setText(radio1);

	QString radio2;
	radio2.append(tr(" to ")).append(lang1);
	m_translationRadio2->setText(radio2);
}
