#include <QtGui>

#include "edittab.h"

#define EDITTAB 0
#define COPYTAB 1
#define EXAMTAB 2

/**
 * @brief: Constructor of QEditTab
 */
QEditTab::QEditTab( QWidget *parent ) : QWidget(parent)
{
	m_isDataChanged = false;

	//Create a new topic
	QPushButton* newTopicBtt = new QPushButton(tr("Create a new topic").rightJustified(20, ' '));
	newTopicBtt->setIcon(style()->standardIcon(QStyle::SP_FileIcon));

	//Save the changes to the topic
	m_saveBtt = new QPushButton(tr("Save changes").rightJustified(20, ' '));
	m_saveBtt->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	m_saveBtt->setEnabled(false);

	//Delete the topic
	m_deleteTopicBtt = new QPushButton(tr("Delete the topic(s)").rightJustified(20, ' '));
	m_deleteTopicBtt->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
	m_deleteTopicBtt->setEnabled(false);

	/*Layout of the tab*/
	QVBoxLayout* vTopicLayout = new QVBoxLayout;
	vTopicLayout->setContentsMargins(20, 15, 20, 15);
	vTopicLayout->addWidget(newTopicBtt);
	vTopicLayout->addWidget(m_saveBtt);
	vTopicLayout->addWidget(m_deleteTopicBtt);

	setLayout(vTopicLayout);

	//inner connections
	connect(newTopicBtt, SIGNAL(clicked()), this, SIGNAL(createTopic()));
	connect(m_saveBtt, SIGNAL(clicked()), this, SIGNAL(saveTopic()));
	connect(m_deleteTopicBtt, SIGNAL(clicked()), this, SIGNAL(deleteTopic()));
}


/**
 * Kind of accessor.
 * Tell if the last data modifications have been saved.
 * The status of the m_saveBtt reflects the status of the data.
 */
const bool QEditTab::isTopicSaved()
{
	return !m_saveBtt->isEnabled();
}


/**********************************************
* public SLOTS
***********************************************/
/**
 * called by a new user-selection with the topicListWidget
 * Enable or disable buttons according to selection(s).
 * All changes to the current topic are lost without warning.
 */
void QEditTab::setCurrentTopics(QStringList selection)
{
   // If it is modified, and the list is changed, then ask to save.
   if (!isTopicSaved()) {
      emit saveTopic();
   }
	if (selection.size() >= 1)
		m_deleteTopicBtt->setEnabled(true);
	else // size == 0
		m_deleteTopicBtt->setEnabled(false);

	m_saveBtt->setEnabled(false);
}


/**
 * Enable or disable buttons according to user changes.
 */
void QEditTab::activateSaveBtt(const QModelIndex & index)
{
	//if ( index.isValid() )
     m_saveBtt->setEnabled(true);
}
void QEditTab::disableSaveBtt()
{
	m_saveBtt->setEnabled(false);
}


/**
 * If the tab is changed during edit and data not saved,
 * proposes to save data.
 */
void QEditTab::leaveEditTab(int newTab)
{
	if ( newTab != EDITTAB && !isTopicSaved() )
	{
		QMessageBox::StandardButton answer;
		answer = QMessageBox::question ( this,
				tr("Leaving topic edition"),
				tr("Do you want to save changes to the topic ?"),
				QMessageBox::Save | QMessageBox::Discard,
				QMessageBox::Save );
		if ( answer == QMessageBox::Save )
			emit saveTopic();

		this->disableSaveBtt();
	}
}

