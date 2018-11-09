#ifndef VOCABULARYWIDGET_H
#define VOCABULARYWIDGET_H

class QComboBox;
class QString;
class QToolButton;

#include <QWidget>

class QVocabularyWidget : public QWidget
{
Q_OBJECT
	QString m_vocName;
	QComboBox *m_vocComboBox;
	QToolButton *m_dialogBtt;
	QStringList m_dirNamesList;

public:
	QVocabularyWidget( QString vocName, QWidget *parent = 0 );

signals:
	void vocChanged(QString);

private slots: 
	void displayVocDialog();
	void newSelection( int index );

private:
	void buildVocList( QString vocName="" );

};

#endif
