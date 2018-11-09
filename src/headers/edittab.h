#ifndef EDITTAB_H
#define EDITTAB_H

#include <QtWidgets>

class QModelIndex;
class QPushButton;
class QString;

/**
* This class generates a tab for leftWidget which allows to edit a topic
*/
class QEditTab : public QWidget
{
//use of signals/slots
Q_OBJECT
//members
	QString m_currentTopic;
	bool m_isDataChanged;
	QPushButton* m_saveBtt;
	QPushButton* m_deleteTopicBtt;
	
//functions
public:
	QEditTab(QWidget *parent=0);
	const bool isTopicSaved();
	
signals:
	void createTopic();
	void saveTopic();
	void deleteTopic();

public slots:
	void setCurrentTopics(QStringList selection);
	void activateSaveBtt(const QModelIndex & index);
	void disableSaveBtt();
	void leaveEditTab(int newTab);

};

#endif


