#ifndef VOCABULARYLINE_H
#define VOCABULARYLINE_H

#include <QtWidgets>

class QLabel;
class QToolButton;
class QRadioButton;
class QString;
class QVocabularyDialog;
class QButtonGroup;

/**
 * The QVocabularyLine class is responsible for the vocabularies objects.
 * It creates and delete vocabularies directories on disk.
 * It presents the vocabularies as a vocabularyLine Widget to be used by 
 * the vocabularyDialog class.
 */
class QVocabularyLine : public QWidget
{
Q_OBJECT
//members
	QLabel *m_label1, *m_label2;
	QToolButton* m_deleteBtt;
	QToolButton* m_editBtt;
	short m_id;
	QString m_vocName;

public:
	QVocabularyLine( short id, QString vocName, QVocabularyDialog* parent=0 );
	void setId( short id );

signals:
	void deleteLine( short id );

private slots:
	void editVoc();
	void renameVoc( QString vocName );
	void deleteVoc();
};

#endif
