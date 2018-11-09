#include <QtGui>
#include <QtWidgets>

#include "vocabularywidget.h"
#include "vocabularydialog.h"


QVocabularyWidget::QVocabularyWidget( QString vocName, QWidget *parent ) : QWidget(parent)
{
	m_vocName = vocName;
	m_vocComboBox = new QComboBox(this);

	m_dialogBtt = new QToolButton(this);
	m_dialogBtt->setIconSize( QSize( 10, 10 ) );
	m_dialogBtt->setIcon( style()->standardIcon( QStyle::SP_FileDialogDetailedView ) );
	m_dialogBtt->setContentsMargins( 0, 0 , 0, 0);

	QHBoxLayout *hLayout = new QHBoxLayout(this);
	hLayout->addWidget(m_vocComboBox);
	hLayout->addWidget(m_dialogBtt);
	hLayout->setContentsMargins( 0, 0 , 0, 0);

	this->buildVocList( vocName );

	connect( m_dialogBtt, SIGNAL( clicked() ), this, SLOT( displayVocDialog() ));
	connect( m_vocComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( newSelection(int) ));
}


/**
 * build the list containing vocabularies names
 * @arg vocName : the name to show selected in the comboBox, m_vocName by default
 */
void QVocabularyWidget::buildVocList( QString vocName )
{
	QDir currentDir;
	m_dirNamesList = QStringList();
	m_dirNamesList = currentDir.entryList( QStringList("*__*"), QDir::Dirs, QDir::Name);
	QStringList vocNames = m_dirNamesList;
	vocNames.replaceInStrings( "__", " <> " );

	//fill the combobox display role
	m_vocComboBox->clear();
	m_vocComboBox->addItems( vocNames );

	//fill the user role
	bool isCurrent = false;
	for (short i=0; i < m_vocComboBox->count(); ++i)
	{
		m_vocComboBox->setItemData( i, m_dirNamesList.at(i) );
		if ( m_dirNamesList.at(i) == vocName )
		{
			isCurrent = true;
			m_vocComboBox->setCurrentIndex( i );
			this->newSelection(i);
		}
	}
	
	//select a list item by default if "if ( m_dirNamesList.at(i) == vocName )" failed
	if ( !isCurrent && m_vocComboBox->count() != 0 )
	{
		m_vocComboBox->setCurrentIndex( 0 );
		this->newSelection(0);
	}
}



/**************************************
 * private SLOTS
 *************************************/
/**
 * Display a big dialog box to manage the vocabularies directories
 */
void QVocabularyWidget::displayVocDialog()
{
	QVocabularyDialog *vocabularyDialog= new QVocabularyDialog( m_dirNamesList, this );
	vocabularyDialog->exec();
	this->buildVocList( m_vocName );
	/*
	 * the buildVocList is a re-build in fact. m_vocName can have been deleted, so it is 
	 * just a hint for the "if ( m_dirNamesList.at(i) == vocName )" of buildVocList().
	 * In case the vocabulary m_vocName has been deleted, the 
	 */
}


/**
 * transmit signals to all necessary widgets when a new vocabulary is selected
 * in m_vocComboBox.
 */
void QVocabularyWidget::newSelection( int index )
{
	m_vocName = m_vocComboBox->itemData( index ).toString();
	emit vocChanged( m_vocName );
}

