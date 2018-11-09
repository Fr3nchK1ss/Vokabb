#ifndef VOCABULARYDIALOG_H
#define VOCABULARYDIALOG_H

#include <QDialog>

class QString;
class QGridLayout;
class QVBoxLayout;
class QVocabularyLine;

class QVocabularyDialog : public QDialog
{
//use of signals/slots
Q_OBJECT
//members
	QList<QVocabularyLine*> m_vocabularyLineList;
	QVBoxLayout* m_vLayout;
	short m_idCount;

//functions
public:
	QVocabularyDialog( QStringList dirNames, QWidget* parent=0 );
	
private slots:
	void buildLine( QString vocName );
	void deleteLine( short line_id );
	void newVocDialog();

};


#endif
