#ifndef STYLEDIALOG_H
#define STYLEDIALOG_H

#include <QDialog>

class QString;
class QComboBox;

class QStyleDialog : public QDialog
{
//use of signals/slots
Q_OBJECT
//members
	QComboBox *m_styleComboBox;

//functions
public:
	QStyleDialog( QWidget* parent=0 );
	
private slots:
	void changeStyle(const QString &styleName);

};


#endif
