#ifndef TOPICLISTWIDGET_H
#define TOPICLISTWIDGET_H

#include <QListWidget>

class QListWidgetItem;
class QString;
class QStringList;

/**
 * QTopicListWidget displaying the current topic
 */
class QTopicListWidget : public QListWidget
{
//use of signals/slots
Q_OBJECT

//members
	QString m_currentVocDir;
	QString m_oldName;
	
//functions
public:
	QTopicListWidget(QString currentVoc, QWidget* parent=0);
	const QString getCurrentVocDir();

public slots:
	void buildItemList(QString currentVoc);
	void createTopic();
	void deleteTopic();
	
signals:
	void newSelection(QStringList selectedTopicsFileNames);

private slots:
	void dispatchSelection();
	void editTopicName(QListWidgetItem* topic);
	void renameTopic(QListWidgetItem* topic);
		
};

#endif
