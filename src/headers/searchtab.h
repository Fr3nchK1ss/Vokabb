#ifndef SEARCHTAB_H
#define SEARCHTAB_H

#include <QtWidgets>

class QLineEdit;
class QString;

/**
* This class generates a tab for leftWidget which allows to edit a topic
*/
class QSearchTab : public QWidget
{
Q_OBJECT

	QLineEdit *m_searchLineEdit;
	
//functions
public:
	QSearchTab(QWidget *parent=0);
	
signals:
	void searchSelection( QRegExp searchRegExp );
	void searchVoc( QRegExp searchRegExp);

public slots:
	void setEditFocus( int newTab );

private slots:
	void selectionSearch();
	void vocSearch();

};

#endif
