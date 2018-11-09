#include <QtGui>

#include "searchtab.h"

#define EDITTAB 0
#define COPYTAB 1
#define EXAMTAB 2
#define SEARCHTAB 3

/**
 * @brief: Constructor of QSearchTab
 */
QSearchTab::QSearchTab( QWidget *parent ) : QWidget(parent)
{
	m_searchLineEdit = new QLineEdit(this);

	QPushButton* selectionSearchBtt = new QPushButton(tr("Search selected topic(s)"));

	QPushButton* vocSearchBtt = new QPushButton(tr("Search the entire vocabulary"));

	/*Layout of the tab*/
	QVBoxLayout* vTopicLayout = new QVBoxLayout;
	vTopicLayout->setContentsMargins(20, 15, 20, 15);
	vTopicLayout->addWidget( m_searchLineEdit );
	vTopicLayout->addWidget( vocSearchBtt );
	vTopicLayout->addWidget( selectionSearchBtt );
	setLayout(vTopicLayout);
	
	//inner connections
	connect( selectionSearchBtt, SIGNAL(clicked()), this, SLOT( selectionSearch() ));
	connect( vocSearchBtt, SIGNAL(clicked()), this, SLOT( vocSearch() ));
}


/**********************************************
* private SLOTS
***********************************************/
/**
 * emit a signal to search all selected topic for the string searchRegExp
 */
void QSearchTab::selectionSearch()
{
	const QRegExp searchRegExp = QRegExp( m_searchLineEdit->text(), Qt::CaseInsensitive );
	emit searchSelection( searchRegExp );
}


/**
 * emit a signal to search the entire vocabulary for the string searchRegExp
 */
void QSearchTab::vocSearch()
{
	const QRegExp searchRegExp = QRegExp( m_searchLineEdit->text(), Qt::CaseInsensitive );
	emit searchVoc( searchRegExp );
}


/**********************************************
* private SLOTS
***********************************************/
void QSearchTab::setEditFocus( int newTab )
{
	if ( newTab == SEARCHTAB )
		m_searchLineEdit->setFocus();
}

