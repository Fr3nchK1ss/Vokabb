#include <QtGui>

#include "vocabularyline.h"
#include "vocabularydialog.h"
#include "languagedialog.h"


QVocabularyLine::QVocabularyLine( short id, QString vocName, QVocabularyDialog* parent) : QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	m_id = id;
	m_vocName = vocName;

	/*
	 * Here the directory corresponding to the vocabulary is created, if it does not exist
	 */
	QDir currentDir;
	if ( !currentDir.exists(m_vocName) )
		currentDir.mkdir(m_vocName);

	QStringList tokens = vocName.split("__");
	m_label1 = new QLabel( tokens.value(0), this );
	m_label1->setMargin( 5 );
	m_label1->setMinimumWidth( 150 );
	m_label1->setAlignment(Qt::AlignCenter);
	m_label1->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	m_label2 = new QLabel( tokens.value(1), this );
	m_label2->setMargin( 5 );
	m_label2->setMinimumWidth( 150 );
	m_label2->setAlignment(Qt::AlignCenter);
	m_label2->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	m_editBtt = new QToolButton(this);
	m_editBtt->setIcon( style()->standardIcon(QStyle::SP_FileIcon) );

	m_deleteBtt = new QToolButton(this);
	m_deleteBtt->setIcon( style()->standardIcon(QStyle::SP_TitleBarCloseButton) );

	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->addStretch();
	hLayout->addWidget( m_label1);
	hLayout->addStretch();
	hLayout->addWidget( new QLabel("<>",this));
	hLayout->addStretch();
	hLayout->addWidget( m_label2);
	hLayout->addStretch();
	hLayout->addWidget( m_editBtt);
	hLayout->addWidget( m_deleteBtt);

	connect( m_editBtt, SIGNAL( clicked() ), this, SLOT( editVoc() ));
	connect( m_deleteBtt, SIGNAL( clicked() ), this, SLOT( deleteVoc() ));
}

//accessors
void QVocabularyLine::setId( short id )
{
	m_id = id;
}


/***********************************
 * private SLOTS
 **********************************/
/**
 * Edit the name of the current vocabulary
 * Proposes a dialog to enter a couple language1/language2.
 * renameVoc() transfers the vocChanged() signal to warn other widgets.
 */
void QVocabularyLine::editVoc()
{
	QLanguageDialog* languageDialog = new QLanguageDialog(this, m_vocName);
	connect( languageDialog, SIGNAL( newName(QString) ), this, SLOT( renameVoc(QString) ));
	languageDialog->exec();
	//languageDialog verifies that the directory does not already exist.
}
/** rename after edit */
void QVocabularyLine::renameVoc( QString vocName )
{
	QDir currentDir;
	if ( !currentDir.rename(m_vocName, vocName) )
	{
		QMessageBox::warning(this,
		tr("Directory error"),
		tr("the directory ").append(m_vocName).append(tr(" can not be renamed. Verify your access rights.")));
	}
	else
	{
		QStringList tokens = vocName.split("__");
		m_label1->setText( tokens.value(0) );
		m_label2->setText( tokens.value(1) );
		m_vocName = vocName;
	}
}


/**
 * The deletion of a QVocabularyLine is processed in 2 steps :
 * 1) the deletion of the vocabulary directory, done by the vocabularyLine itself
 * 2) the removal/deletion of the vocabularyLine from its parent widget,done by vocabularyDialog through the deleteLine() signal.
 */
void QVocabularyLine::deleteVoc()
{
	//Open a messageBox to confirm deletion
	QString message;
	message.append(tr("Are you sure you want to delete the vocabulary ").append( m_label1->text() ).append(" / ").append( m_label2->text() ).append(tr(" ?")));
	int agree = QMessageBox::question(this,
					tr("Vocabulary deletion"),
					message,
					QMessageBox::Ok | QMessageBox::Cancel,
					QMessageBox::Cancel);
	if ( agree == QMessageBox::Ok )
	{
		//Look all the files in the vocabulary directory
		bool deletionError = false;
		QDir vocDir(m_vocName);
		QStringList topicNames = vocDir.entryList( QStringList("*"), QDir::Files);
		for (int i=0; i < topicNames.size(); i++)
		{
			if ( ! vocDir.remove( topicNames.at(i) ) )
			{
				QMessageBox::warning(this,
				tr("File error"),
				tr("the file ").append( topicNames.at(i) ).append(tr(" can not be deleted. Verify your access rights.")));
				deletionError = true;
				break;
			}
		}
		
		if ( !deletionError )
		{
			QDir currentDir;
			if ( !currentDir.rmdir(m_vocName) )
			{
				QMessageBox::warning(this,
				tr("Directory error"),
				tr("the directory ").append(m_vocName).append(tr(" can not be deleted. Verify your access rights.")));
			}
			else
				emit deleteLine( m_id );
		}
	}
}
