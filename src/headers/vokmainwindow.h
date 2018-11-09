#ifndef VOKMAINWINDOW_H
#define VOKMAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QString;
class QvokTableView;
class QvokTabWidget;
class QTopicListWidget;
class QEditTab;
class QExamTab;
class QCopyTab;
class QSearchTab;
class QVocabularyDialog;
class QVocabularyWidget;

class QvokMainWindow : public QMainWindow
{
//use of signals/slots
Q_OBJECT

//members
	//Popup menus of the menu bar
	QMenu* m_fileMenu;

	QAction* m_styleAct;

	QAction* m_translateAct;

	QMenu* m_creditsMenu;
	QAction* m_aboutVokabbAct;
	QAction* m_aboutQtAct;

	//Here the usefull widgets of the mainWindow
	QvokTableView *m_vokTableView;
	QWidget* m_leftWidget;
		QVocabularyWidget *m_vocabularyWidget;
		QTopicListWidget *m_topicListWidget;
		QvokTabWidget *m_tabWidget;
			QEditTab *m_editTab;
			QExamTab *m_examTab;
			QCopyTab *m_copyTab;
			QSearchTab *m_searchTab;

	QString m_translation;

//functions
public:
	QvokMainWindow(QWidget *parent=0, Qt::WindowFlags flags=0);

private:
	void setupMenus();
	void setupViews();
	void setupConnections();
	void readSettings(QString &translation, int& testCount, int& blankCount, short& boxChecked, bool& displayOnlyColumn2, bool& showExampleColumn, QString& currentVocName);
	void writeSettings();

protected:
	void closeEvent(QCloseEvent *event);//overloaded

signals:
	void saveTopic();

private slots:
	void aboutVokabb();
	void aboutQt();
	void chooseStyle();
	void translate();
	void setTranslation( const QString &translation );

};

#endif
