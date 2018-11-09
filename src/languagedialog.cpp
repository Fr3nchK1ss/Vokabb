#include <QtGui>
#include <QtWidgets>

#include "languagedialog.h"

QLanguageDialog::QLanguageDialog( QWidget* parent, QString vocName ):QDialog(parent)
{
	setWindowTitle("Languages settings");
	
	QStringList tokens = vocName.split("__");
	m_lineEdit1 = new QLineEdit( tokens.value(0) );
	m_lineEdit2 = new QLineEdit( tokens.value(1) );
	
	QLabel* label = new QLabel(tr("Enter the two languages names for the new vocabulary."));
	QPushButton* saveBtt = new QPushButton(tr("Save"));
	QPushButton* cancelBtt = new QPushButton(tr("Cancel"));

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(label);
	vLayout->addSpacing(20);
		QGridLayout* gLayout = new QGridLayout();
		//gLayout->addWidget( new QLabel(tr("First language")), 0, 0);
		//gLayout->addWidget( new QLabel(tr("Second language")), 0, 2);
		gLayout->addWidget(m_lineEdit1, 1, 0);
		gLayout->addWidget( new QLabel("<>"), 1, 1);
		gLayout->addWidget(m_lineEdit2, 1, 2);
	vLayout->addLayout(gLayout);
	vLayout->addSpacing(20);
		QHBoxLayout* hLayout = new QHBoxLayout();
		hLayout->addStretch();
		hLayout->addWidget(saveBtt);
		hLayout->addWidget(cancelBtt);
	vLayout->addLayout(hLayout);
	
	this->setLayout(vLayout);
	
	//inner connections
	connect(saveBtt, SIGNAL(clicked()), this, SLOT(saveVoc()) );
	connect(cancelBtt, SIGNAL(clicked()), this, SLOT(reject()) );
}


/***********************************
 * PRIVATE SLOTS
 **********************************/
/**
 * saveVoc() sends to a vocabulary dialog a new vocabulary name, which is a string 
 * of type lang1__lang2.
 * The function checks if the vocabulary already exists on the disk.But it does not 
 * create the vocabulary directory itself on the disk because this dialog has a 
 * generic purpose. Creation of the directory is delegated to vocabularyLine class.
 */
void QLanguageDialog::saveVoc()
{
	QString lang1 = m_lineEdit1->text();
	QString lang2 = m_lineEdit2->text();
	
	if ( ! (lang1.isEmpty() || lang2.isEmpty()) )
	{
		QString vocabularyName = lang1.append("__").append(lang2);
		if ( QDir::current().exists(vocabularyName) )
			QMessageBox::critical(this,
			tr("File error"),
			tr("the vocabulary ").append(vocabularyName).append(tr(" already exists.")));
		else
		{
			emit newName(vocabularyName);
			accept();
		}
	}
	//else do nothing : the user must use the cancel button
}
