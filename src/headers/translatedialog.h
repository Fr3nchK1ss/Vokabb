#ifndef TRANSLATEDIALOG_H
#define TRANSLATEDIALOG_H

#include <QDialog>

class QString;
class QComboBox;

class QTranslateDialog : public QDialog
{
//use of signals/slots
Q_OBJECT
//members
	QComboBox *m_styleComboBox;

//functions
public:
	QTranslateDialog( QWidget* parent=0 );

signals:
	void translate(const QString &);

};


#endif
