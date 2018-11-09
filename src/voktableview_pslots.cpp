#include <QtWidgets>

#include "voktableview.h"
#include "mdl_copymodel.h"
#include "mdl_editmodel.h"
#include "mdl_searchmodel.h"
#include "mdl_testmodel.h"


#define EDITTAB 0
#define COPYTAB 1
#define EXAMTAB 2
#define SEARCHTAB 3

#define CORRECT_COL 2
#define NO_HIDDEN_COL -1

#define LANG_1 1
#define LANG_2 2
#define MIX 3
/*
 * public SLOTS of voktableview
 * The slots defined in voktableview_pslots.cpp are triggered by user input on an OTHER
 * widget than the view. Typically a tab widget.
 */

/*************************************************
 * changes from vocabularyDialog
 ************************************************/
void QvokTableView::setModelHeaders(QString vocName)
{
	m_editItemModel->setHeader(vocName);
	m_copyItemModel->setHeader(vocName);
	m_testItemModel->setHeader(vocName);
	m_searchItemModel->setHeader(vocName);
}



/*************************************************
 * changes from topicListWidget
 ************************************************/

/**
 * Update the views after a change in the topic selection by the user.
 * @arg selection : is a list of topic filename [file1.vok, file2.vok,...]
 */
void QvokTableView::setCurrentTopics(QStringList selection)
{
	//remember selection for EDITTAB
	m_selection = selection;

	this->restoreItemModels();
	for (int i=0; i < selection.size(); i++)
		this->openTopicFile( selection.at(i) );

	if ( m_currentTabIndex == EXAMTAB)
		emit stopExam();

	this->changeDataView(m_currentTabIndex);
}


/*************************************************
 * changes from vokTabWidget
 ************************************************/
/**
 * Change the way the view behaves, according to the selected TAB.
 * @arg newTab : the number of the tab clicked by the user
 */
void QvokTableView::changeDataView(int newTab)
{	
	int oldTab = m_currentTabIndex;
	m_currentTabIndex = newTab;

	switch (newTab)
	{
		case EDITTAB:
			/*
			 * The edit tab can allow only one topic to be displayed at a time.
			 * Otherwise it would be impossible to save a topic correctly. There
			 * are two cases to distinguish:
			 * 1) the user comes from another tab : his selection is kept but the code
			 * 	below displays only the last topic.
			 * 2) the user is in EDITTAB mode and do a multiple selection.
			 * 	setCurrentTopics() slot takes care that only the last topic is
			 * 	displayed at a time.
			 * Multiple selection is NOT DISABLED in topiclistwidget in order to allow
			 * multiple deletions at the same time. Otherwise it would have 
			 * been a nice solution.
			 */
			if (oldTab == EXAMTAB)
				emit stopExam();

			if ( !m_selection.isEmpty() )
			{
				this->restoreItemModels();
				this->openTopicFile( m_selection.last() );
				setEditTriggers(QAbstractItemView::AllEditTriggers);
			}
			else
				setEditTriggers(QAbstractItemView::NoEditTriggers);

			setModel(m_editItemModel);
			//m_hHeader->setClickable(true);
			break;

		case COPYTAB:
			/*
			 * Coming from EDITTAB, display all the selection instead of 
			 * only the last topic (in case the selection has more than one topic!)
			 */
			if (oldTab == EXAMTAB)
				emit stopExam();

			if (oldTab == EDITTAB && m_selection.size() > 1)
			{
				setCurrentTopics(m_selection);
				return;
			}

			setEditTriggers(QAbstractItemView::AllEditTriggers);
			setModel(m_copyProxyModel);
			break;

		case EXAMTAB:
			/*
			 * If the user leaves EXAMTAB during a test:
			 * - stopExam() signal will be emitted by others 'cases'
			 * - but m_testItemModel is not reseted and previous answers are not cleaned.
			 * - next time the user comes back to this tab, m_testItemModel will be reseted.
			 * - at the first call of launchTest() then previous answers will be cleaned.
			 *
			 * Coming from EDITTAB, display all the selection instead of 
			 * only the last topic.
			 */
			if (oldTab != EXAMTAB )
			{//so changeDataView has not been called by this->setCurrentTopics()
				setCurrentTopics(m_selection);
				return;
			}

			setEditTriggers(QAbstractItemView::NoEditTriggers);
			setModel(m_testItemModel);
			break;
		
		case SEARCHTAB:
			/*
			 * Coming from EDITTAB, display all the selection instead of 
			 * only the last topic.
			 */
			if (oldTab == EXAMTAB)
				emit stopExam();

			if (oldTab == EDITTAB && m_selection.size() > 1)
			{
				setCurrentTopics(m_selection);
				return;
			}

			setEditTriggers(QAbstractItemView::AllEditTriggers);
			setModel(m_searchItemModel);
			break;
		default:
			break;//never reached
	}

	scrollToTop();
}



/*************************************************
 * changes from EDIT TAB
 ************************************************/
/**
 * Save modifications to the current topic.
 */
void QvokTableView::saveTopic()
{
	if ( m_editItemModel->saveTopic(m_selection.last()) )
		emit topicSaved();
	setCurrentTopics( QStringList( m_selection.last() ) );
}



/*************************************************
 * changes from COPY TAB
 ************************************************/
/**
 * Toggle the display from one to two column using a proxy
 */
void QvokTableView::toggleColDisplay()
{
	m_copyProxyModel->toggleColDisplay();
}


/**
 * Set the number of blank rows for the copy exercise.
 */
void QvokTableView::setBlankRows(int blankRowsCount)
{
	m_copyItemModel->setBlankRows(blankRowsCount);
}


/*************************************************
 * changes from EXAM TAB
 ************************************************/
/**
 * Called from examTab when the user launches a test.
 * Modify the data model through proxy model according to the user parameters.
 * @arg testCount : the number of lines for the test
 * @arg boxChecked : the number of the box checked in examtab
 * @arg isTest : indicates if the user has pushed the 'launch test' button or not
 */
void QvokTableView::startStopTest( short testCount, short examType, bool isTest )
{
	if (isTest)
	{
		setEditTriggers(QAbstractItemView::AllEditTriggers);
		/*
		 * All triggers are allowed, but the flags() fonctions 
		 * of testProxyModel does a second selection.
		 */

		//clean possible previous answers
		m_testItemModel->prepareTest( examType );

		switch ( examType )
		{
			case LANG_1:
				m_testProxyModel->shuffle( m_testItemModel, testCount );
				break;
			case LANG_2:
				m_testSwapProxyModel->swapColumns( m_testItemModel );
				m_testProxyModel->shuffle( m_testSwapProxyModel, testCount );
				break;
			case MIX:
				m_testRandProxyModel->randColumns( m_testItemModel);
				m_testProxyModel->shuffle( m_testRandProxyModel, testCount );
				break;
			default:
				break;//never reached
		}

		m_testProxyModel->hideColumn( CORRECT_COL );
		setModel(m_testProxyModel);
	}
	else
	{
		setEditTriggers(QAbstractItemView::NoEditTriggers);
		m_testProxyModel->hideColumn( NO_HIDDEN_COL );
		m_testItemModel->correct(testCount);
	}
}


/*************************************************
* changes from SEARCH TAB
 ************************************************/
/**
 * small research among the selected topics.
 * If we have done a searchVoc() before, or even a searchSelection,
 * the model built out of the selected topics is messed up.
 * So we rebuild it by a setCurrentTopics().
 */
void QvokTableView::searchSelection( QRegExp searchRegExp )
{
	setCurrentTopics(m_selection);
	m_searchItemModel->searchSelection( searchRegExp );
}


/**
 * Big research processed by searchModel
 */
void QvokTableView::searchVoc( QRegExp searchRegExp)
{
	m_searchItemModel->searchVoc( searchRegExp );
}

