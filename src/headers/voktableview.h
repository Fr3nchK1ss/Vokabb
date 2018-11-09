#ifndef VOKTABLEVIEW_H
#define VOKTABLEVIEW_H

#include <QTableView>

class QHeaderView;
class QString;
class QStringList;
class QEditItemModel;
class QCopyItemModel;
class QCopyProxyModel;
class QKeyEvent;
class QRegExp;
class QSearchItemModel;
class QTestRandProxyModel;
class QTestSwapProxyModel;
class QTestProxyModel;
class QTestItemModel;

/**
 * This class is a viewer for the list of words of a topic. But as Qt merges View and
 * Controler in their model/view architecture, this class plays the role of controler
 * too. In particular models are private members of the view.
 * The slots defined in voktableview_pslots.cpp are triggered by user input on an OTHER
 * widget than the view. Typically a tab widget.
 */
class QvokTableView : public QTableView
{
	Q_OBJECT

	//header settings
	QHeaderView *m_hHeader, *m_vHeader;
	short m_currentTabIndex;
	QStringList m_selection;

	//models management (standarditemmodels)
	QEditItemModel *m_editItemModel;
	QCopyItemModel *m_copyItemModel;
	QSearchItemModel *m_searchItemModel;
	QTestItemModel *m_testItemModel;

	//models management (proxymodels)
	QTestProxyModel *m_testProxyModel;
	QTestSwapProxyModel *m_testSwapProxyModel;
	QTestRandProxyModel *m_testRandProxyModel;
	QCopyProxyModel *m_copyProxyModel;



//functions
public:
	QvokTableView( int blankCount, bool displayOnlyColumn2, QString currentVocName, bool, QWidget* parent );
    bool m_showExampleColumn;

private:
	void restoreItemModels();
	bool openTopicFile(QString fileName);

protected:
	void keyPressEvent(QKeyEvent *event);

signals:
	void stopExam();
	void topicSaved();

public slots:
	void setModelHeaders(QString vocName);
	void changeDataView(int newTab);
	void setCurrentTopics(QStringList selection);
	void saveTopic();
	void toggleColDisplay();
	void setBlankRows(int blankRowsCount);
	void startStopTest(short, short, bool);
	void searchSelection( QRegExp searchRegExp );
	void searchVoc( QRegExp searchRegExp );

};

#endif
