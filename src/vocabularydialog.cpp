#include <QtGui>

#include "vocabularydialog.h"
#include "vocabularyline.h"
#include "languagedialog.h"

/**
 * A nasty bug in Qt deters from removing correctly a widget from a QGridLayout
 * So the layout construction for QVocabularyDialog becomes complex, but it works.
 *
 */
QVocabularyDialog::QVocabularyDialog( QStringList dirNames, QWidget* parent):QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);

	//initialize members
	setWindowTitle("Vocabularies List");
	m_vLayout = new QVBoxLayout();
	m_idCount = 0;

	QPushButton* newVocBtt = new QPushButton(tr("New vocabulary"));
	newVocBtt->setIcon(style()->standardIcon( QStyle::SP_FileDialogNewFolder ));
	QPushButton* closeBtt = new QPushButton(tr("Close"));
	closeBtt->setIcon(style()->standardIcon( QStyle::SP_DialogCloseButton ));

	QLabel *firstlang = new QLabel(tr("First language"));
	firstlang->setMinimumWidth(150);
	firstlang->setAlignment(Qt::AlignCenter);

	QLabel *secondlang = new QLabel(tr("Second language"));
	secondlang->setMinimumWidth(150);
	secondlang->setAlignment(Qt::AlignCenter);

	/********************
	 * LAYOUT
	 *******************/
	QVBoxLayout* mainLayout= new QVBoxLayout(this);
	mainLayout->setContentsMargins( 20, 20, 20, 30);
		QHBoxLayout* hLayout1 = new QHBoxLayout();
		hLayout1->addStretch();
		hLayout1->addWidget( firstlang );
		hLayout1->addStretch();
		hLayout1->addWidget( new QLabel(tr("<>")) );
		hLayout1->addStretch();
		hLayout1->addWidget( secondlang );
		hLayout1->addStretch();
		hLayout1->addSpacing(60);
	mainLayout->addLayout(hLayout1);
	mainLayout->addSpacing(20);
		//Here we gonna use m_vLayout in this for loop
		for (short i=0; i < dirNames.size(); ++i)
		{
			this->buildLine( dirNames.at(i) );
		}
	mainLayout->addLayout(m_vLayout);
	mainLayout->addSpacing(20);
		QHBoxLayout* hLayout2 = new QHBoxLayout();
		hLayout2->addWidget(newVocBtt);
		hLayout2->addStretch();
		hLayout2->addWidget(closeBtt);
	mainLayout->addLayout(hLayout2);

	//inner connections
	connect(newVocBtt, SIGNAL( clicked() ), this, SLOT( newVocDialog() ));
	connect(closeBtt, SIGNAL( clicked() ), this, SLOT( close() ));
}


/*********************************************
 * private SLOTS
 ********************************************/
/**
 * Add a new vocabulary line in the dialog layout. Each line is given an 
 * id which is m_idCount. This id is used in case of line deletion.
 * 1) The line emits a deleteLine() with its id.
 * 2) The vocabularyDialog catches the signal and thanks to the id knows
 * 	which line of m_vocabularyLineList must be deleted.
 */
void QVocabularyDialog::buildLine( QString vocName)
{
	m_vocabularyLineList << new QVocabularyLine( m_idCount, vocName, this);
	++m_idCount;
	m_vLayout->addWidget( m_vocabularyLineList.last());

	connect( m_vocabularyLineList.last(), SIGNAL( deleteLine( short ) ), this, SLOT( deleteLine( short ) ));
}


/**
 * Destroy a line created by buildLine in the layout.
 * @arg line: not the line of the gridLayout!
 */
void QVocabularyDialog::deleteLine( short line_id )
{
	/*
	 * We don't have to disconnect the vocabularyLine, the documentation precises :
	 * "A signal-slot connection is removed when either of the objects involved are destroyed."
	 * The flag Qt::WA_DeleteOnClose is set so we just have to remove the vocabularyLine	
	 * from the layout, from the list, and close it.
	 */
	m_vLayout->removeWidget( m_vocabularyLineList.at( line_id ) );
	m_vocabularyLineList.takeAt( line_id )->close();
	/*
	 * The layout is updated automatically !normally! But it does not work so we force the
	 * layout overriding by setFixedSize() and then resize the window.
	 * setFixedSize is (350, 50) and not less to avoid flickering.
	 */
	setFixedSize( 350, 50 );
	setMinimumSize( sizeHint() );

	/*
	 * reset the line id so they correspond for each vocabularyLine
	 * to their place in vocabularyLineList. See buildLine() comment.
	 */
	for ( m_idCount = 0; m_idCount < m_vocabularyLineList.size(); ++m_idCount)
	{
		m_vocabularyLineList.at(m_idCount)->setId(m_idCount);
	}

	/*
	 * Take cares of the consequences of a deletion
	 */
	if ( m_vocabularyLineList.size() == 0)
	{
		this->newVocDialog();
	}
}


/**
 * Proposes a dialog to enter a couple language1/language2.
 * Calls buildLine if the user gives valid arguments.
 */
void QVocabularyDialog::newVocDialog()
{
	QLanguageDialog* languageDialog = new QLanguageDialog(this);
	connect( languageDialog, SIGNAL( newName(QString) ), this, SLOT ( buildLine(QString) ) );
	languageDialog->exec();
}
