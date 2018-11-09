#include <QtGui>
#include <QtWidgets>

#include "topiclistwidget.h"

#define EDITTAB 0
#define EXAMTAB 1
#define COPYTAB 2

/**
 * The Constructor of the left part of the Vokabb Window.
 */
QTopicListWidget::QTopicListWidget( QString currentVoc, QWidget* parent) : QListWidget(parent)
{
	//disable edit triggers : we gonna code our own trigger
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	this->buildItemList(currentVoc);

	//inner connections
	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(dispatchSelection()));
	connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editTopicName(QListWidgetItem*)));
	connect(this, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(renameTopic(QListWidgetItem *)));
}

/*******************************
 * accessors
 ******************************/
const QString QTopicListWidget::getCurrentVocDir()
{
	return m_currentVocDir.remove(QChar('/'));
}


/*******************************
 *private SLOTS
 *******************************/
/**
 * Emit a signal when the user changes his selection (to other widgets)
 */
void QTopicListWidget::dispatchSelection()
{
	QList<QListWidgetItem* > selectedTopics = selectedItems();

	//create the list of topics
	QStringList topicFileNames;
	QString topicName;
	for (int i=0; i < selectedTopics.size(); i++)
	{
		topicName = m_currentVocDir;
		topicName.append( selectedTopics.at(i)->text() );
		topicFileNames << topicName.append(".vok");
	}
	emit newSelection(topicFileNames);
}


/**
 * allow editing of a topic in order to change its name
 */
void QTopicListWidget::editTopicName(QListWidgetItem* topic)
{
	m_oldName = topic->text();
	editItem(topic);
}
void QTopicListWidget::renameTopic(QListWidgetItem* topic)
{
	QString newName = topic->text();

	QDir::setCurrent(m_currentVocDir);
	QFile topicFile(m_oldName.append(".vok"));
	topicFile.rename(newName.append(".vok"));
	QDir::setCurrent("..");


	sortItems();
	clearSelection();
	topic->setSelected(true);
	/*
	 * reopen the file for VokabbTable
	 * itemSelectionChanged() is connected to dispatchSelection()
	 * dispatchSelection() will emit newSelection(topicFileNames)
	 * The voktableview slot 'setCurrentTopics()' will be called
	 * and the correct file reopened
	 */
	emit itemSelectionChanged();
}


/********************************************
 * public SLOTS
 ********************************************/
/**
 * Build the list of topics items from a vocabulary directory
 */
void QTopicListWidget::buildItemList(QString currentVoc)
{
   m_currentVocDir = currentVoc.append("/");
	clear();

	//Create the list of items out of the .vok topic files
	QDir vocDir(m_currentVocDir);
	QStringList topicNames = vocDir.entryList( QStringList("*.vok"), QDir::Files, QDir::Name );
	topicNames.replaceInStrings( ".vok", "" );
	addItems(topicNames);

	//to edit the name of the items
	for (int i=0; i < count(); i++)
		item(i)->setFlags(item(i)->flags() | Qt::ItemIsEditable);

	sortItems();
	clearSelection();
}


/**
 *@brief: update the ListWidget of the left panel when a new topic is created
 *This slots also creates a new text file for the new topic
 */
void QTopicListWidget::createTopic()
{
	bool ok;
	QString newTopic = QInputDialog::getText(this,
			tr("New topic"),
			tr("Enter the name of the topic."),
			QLineEdit::Normal,
			QString(),
			&ok);

	if (ok && !newTopic.isEmpty() )
	{
		QString fileName = newTopic;
		QFile newTopicFile(fileName.prepend(m_currentVocDir).append(".vok"));

		if ( newTopicFile.exists() )
		{
			QMessageBox::warning(this,
				tr("File creation"),
				tr("This topic already exists."));
		}
		else if (newTopicFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			clearSelection();

			//display the new topic in the ListWidget
			QListWidgetItem* newItem = new QListWidgetItem;
			newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
			newItem->setText(newTopic);
			addItem(newItem);

			sortItems();
			newItem->setSelected(true);
			scrollToItem(newItem);
		}
		else
			QMessageBox::warning(this,
					tr("File creation"),
					tr("Unable to write to disk"));
	}
}


/**
 * Delete the selected topic(s)
 */
void QTopicListWidget::deleteTopic()
{
	QList<QListWidgetItem* > selectedTopics = selectedItems();

	//retrieve the names of the topics-to-delete
	QStringList topicNames;
	for (int i=0; i < selectedTopics.size(); i++)
		topicNames<<selectedTopics.at(i)->text();

	//make a single string with the name of the topics-to-delete for user display
	QString topicNamesString = topicNames.join(", ");

	//Open a messageBox to confirm deletion
	int agree = QMessageBox::question(this,
					  tr("Topic deletion"),
					  (topicNamesString.prepend(tr("Are you sure you want to delete ")).append(" ?")),
					  QMessageBox::Ok | QMessageBox::Cancel,
					  QMessageBox::Cancel);
	if ( agree == QMessageBox::Ok )
	{
		//add the extension ".vok" to all topic names
		topicNames.replaceInStrings(QRegExp("^(.*)"), "\\1.vok");

		//delete the vok files corresponding to the topic
		for (int i=0; i < topicNames.size(); i++)
		{
			QString topicName = m_currentVocDir;
			if ( QFile::remove( topicName.append( topicNames.at(i) ) ) )
			{
				//remove the topic item from the listWidget
				delete takeItem(row(selectedTopics.takeFirst()));

			}
			else
			{
				QMessageBox::warning(this,
					     tr("File error"),
					     tr("the file corresponding to the topic can not be deleted. Verify your access rights on this file."));
			}
		}
	}
}
