#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QDialog>

class QLineEdit;
class QString;

class QLanguageDialog : public QDialog
{
//use of signals/slots
Q_OBJECT

//members
	QLineEdit* m_lineEdit1, * m_lineEdit2;

//functions
public:
	QLanguageDialog( QWidget* parent=0, QString vocName="" );

signals:
	void newName(QString vocabularyName);

private slots:
	void saveVoc();
};

#endif
