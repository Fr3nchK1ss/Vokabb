#include <QtGui>

#include "vokmainwindow.h"
#include "vocabularydialog.h"
#include "voktableview.h"
#include "vocabularywidget.h"
#include "styledialog.h"
#include "translatedialog.h"
#include "topiclistwidget.h"
#include "voktabwidget.h"
#include "edittab.h"
#include "examtab.h"
#include "copytab.h"
#include "searchtab.h"

/**
* Constructor
*/
QvokMainWindow::QvokMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
   setWindowTitle("Vokabb");
   qApp->setWindowIcon(QIcon(":/vokabb.icon"));

   //retrieve settings
   int testCount, blankCount;
   short boxChecked;
   bool displayOnlyColumn2, showExample;
   QString currentVocName, translation;
   this->readSettings(translation, testCount, blankCount, boxChecked, displayOnlyColumn2, showExample, currentVocName);

   m_translation = translation;
   //initialize members widgets, considering the settings if necessary
   if(showExample) {
      m_vokTableView = new QvokTableView( blankCount, displayOnlyColumn2, currentVocName, true, this);
   } else {
      m_vokTableView = new QvokTableView( blankCount, displayOnlyColumn2, currentVocName, false, this);
   }
   m_editTab = new QEditTab();
   m_examTab = new QExamTab(currentVocName, testCount, boxChecked);
   m_copyTab = new QCopyTab(blankCount, displayOnlyColumn2);
   m_searchTab = new QSearchTab();

   //initialize left member widgets
   m_leftWidget = new QWidget();
   m_leftWidget->setMaximumWidth(280);
   m_vocabularyWidget = new QVocabularyWidget( currentVocName );
   m_topicListWidget = new QTopicListWidget( currentVocName, m_leftWidget);
   m_tabWidget = new QvokTabWidget(m_leftWidget);


   this->setupMenus();
   this->setupViews();
   this->setupConnections();


}


/**
 * Build the menu bar. Link the menu buttons to their corresponding actions.
 */
void QvokMainWindow::setupMenus()
{
	//Settings menu
	//m_fileMenu = menuBar()->addMenu(tr("Files"));

	//Options menu
	m_styleAct = new QAction(tr("Styles"), this);
	menuBar()->addAction(m_styleAct);	

	//Options menu
	
	m_translateAct = new QAction(tr("Translation"), this);
	menuBar()->addAction(m_translateAct);

	//About menu
	m_creditsMenu = menuBar()->addMenu(tr("Credits"));
	m_aboutVokabbAct = new QAction(tr("About Vokabb"), this);
	m_aboutQtAct = new QAction(tr("About Qt"), this);
	
	m_creditsMenu->addAction(m_aboutVokabbAct);
	m_creditsMenu->addAction(m_aboutQtAct);

/*
	QHBoxLayout *hl = new QHBoxLayout();
	hl->setContentsMargins (2, 2, 2 ,2 );
		hl->addStretch();
		QComboBox *cb = new QComboBox();
		cb->setFrame(true);
		cb->setMinimumWidth(200);
		cb->setMinimumHeight( 20 );
	hl->addWidget( cb );
	menuBar()->setLayout (hl);
*/

	//menu connections
	connect(m_styleAct, SIGNAL(triggered() ), this, SLOT( chooseStyle() ));
	connect(m_translateAct, SIGNAL(triggered() ), this, SLOT( translate() ));
	connect(m_aboutVokabbAct, SIGNAL(triggered() ), this, SLOT( aboutVokabb() ));
	connect(m_aboutQtAct, SIGNAL(triggered() ), this, SLOT( aboutQt() ));
}


/**
 * Links all the components of the mainWindow.
 * Organizes the layout for those components.
 */
void QvokMainWindow::setupViews()
{
	//the window view is divided into two parts
	QSplitter* splitter = new QSplitter();

	//LEFT PART
		//upper part is m_topicListWidget

		//lower part
		m_tabWidget->addTab(m_editTab, tr("&Edit"));
		m_tabWidget->setTabToolTip ( 0, tr("Bring modifications to a topic.") );

		m_tabWidget->addTab(m_copyTab, tr("&Copy"));
		m_tabWidget->setTabToolTip ( 1, tr("Copy lines of vocabulary.") );

		m_tabWidget->addTab(m_examTab, tr("&Test !"));
		m_tabWidget->setTabToolTip ( 2, tr("Test your knowledge.") );

		m_tabWidget->addTab(m_searchTab, tr("&Search"));

		//assembling upper and lower parts
		QVBoxLayout* leftLayout = new QVBoxLayout;
		leftLayout->addWidget(m_vocabularyWidget);
		leftLayout->addWidget(m_topicListWidget);
		leftLayout->addWidget(m_tabWidget);

		m_leftWidget->setLayout(leftLayout);
		splitter->addWidget(m_leftWidget);

	//RIGHT PART
		splitter->addWidget(m_vokTableView);

	setCentralWidget(splitter);
}


/**
 * Connections between the widgets members of vokabbWindow
 */
void QvokMainWindow::setupConnections()
{
	connect(this, SIGNAL( saveTopic() ), m_vokTableView, SLOT( saveTopic() ));

	connect(m_vocabularyWidget, SIGNAL( vocChanged(QString) ), m_topicListWidget, SLOT( buildItemList(QString) ));
	connect(m_vocabularyWidget, SIGNAL( vocChanged(QString) ), m_examTab, SLOT( setLanguages(QString) ));
	connect(m_vocabularyWidget, SIGNAL( vocChanged(QString) ), m_vokTableView, SLOT( setModelHeaders(QString) ));

	//user events on topicListWidget
	connect(m_topicListWidget, SIGNAL(newSelection(QStringList)), m_editTab, SLOT(setCurrentTopics(QStringList)));
	connect(m_topicListWidget, SIGNAL(newSelection(QStringList)), m_vokTableView, SLOT(setCurrentTopics(QStringList)));
	
	//user events on editTab
	connect(m_editTab, SIGNAL(createTopic()), m_topicListWidget, SLOT(createTopic()));
	connect(m_editTab, SIGNAL(saveTopic()), m_vokTableView, SLOT( saveTopic() ));
	connect(m_editTab, SIGNAL(deleteTopic()), m_topicListWidget, SLOT(deleteTopic()));
	
	//user events on examTab
	connect(m_examTab, SIGNAL(startStopTest(short, short, bool)), m_vokTableView, SLOT(startStopTest(short, short, bool)));

	//user events on copyTab
	connect(m_copyTab, SIGNAL( launchCopy(int) ), m_vokTableView, SLOT( setBlankRows(int) ));
	connect(m_copyTab, SIGNAL( colCountChanged() ), m_vokTableView, SLOT( toggleColDisplay() ));

	//user events on searchTab
	connect(m_searchTab, SIGNAL( searchSelection(QRegExp) ), m_vokTableView, SLOT( searchSelection(QRegExp) ));
	connect(m_searchTab, SIGNAL( searchVoc(QRegExp) ), m_vokTableView, SLOT( searchVoc(QRegExp) ));

	//user events on m_tabWidget
	connect(m_tabWidget, SIGNAL(currentChanged(int)), m_vokTableView, SLOT(changeDataView(int)));
	connect(m_tabWidget, SIGNAL(currentChanged(int)), m_editTab, SLOT( leaveEditTab(int) ));
	connect(m_tabWidget, SIGNAL(currentChanged(int)), m_searchTab, SLOT( setEditFocus(int) ));

	//user events on vokTableView
	connect(m_vokTableView, SIGNAL( pressed(const QModelIndex &) ), m_editTab, SLOT( activateSaveBtt(const QModelIndex &) ));
	connect(m_vokTableView, SIGNAL( topicSaved() ), m_editTab, SLOT( disableSaveBtt() ));
	connect(m_vokTableView, SIGNAL( stopExam() ), m_examTab, SLOT( stopExam() ));

}


/**
 * Settings
 * Classical overloaded closeEvent function
 */
void QvokMainWindow::closeEvent(QCloseEvent *event)
{
	if ( m_editTab->isTopicSaved() == true )
	{
		this->writeSettings();
		event->accept();
	}
	else
	{
		QMessageBox::StandardButton answer;
		answer = QMessageBox::question ( this,
				tr("Application terminating"),
				tr("Do you want to save changes to the topic ?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
				QMessageBox::Save );
		switch (answer)
		{
		 case QMessageBox::Save :
					emit saveTopic();
		 case QMessageBox::Discard :
					this->writeSettings();
					event->accept();
					break;
		 default :
					event->ignore();
		}

	}	
}


/**
 * Settings
 * Classical readSettings function
 */
void QvokMainWindow::readSettings(QString &translation, int& testCount, int& blankCount, short& boxChecked, bool& displayOnlyColumn2, bool& showExampleColumn,  QString& currentVocName)
{
	QSettings settings("./vokabb.ini", QSettings::IniFormat);
	QPoint pos = settings.value("pos", QPoint(0,0)).toPoint();
	QSize size = settings.value("size", QSize(800,600)).toSize();
	translation = settings.value("translation", "English").toString();
	testCount = settings.value("testCount", 12).toInt();
	blankCount = settings.value("blankCount", 5).toInt();
	boxChecked = (short)settings.value("boxChecked", 3).toInt();
	displayOnlyColumn2 = settings.value("displayOnlyColumn2", false).toBool();
	currentVocName = settings.value("vocabulary", "").toString();
	showExampleColumn = settings.value("displayExample", true).toBool();

	resize(size);
	move(pos);
}
/**
 * Settings
 * Classical writeSettings function
 */
 void QvokMainWindow::writeSettings()
{
	QSettings settings( "./vokabb.ini", QSettings::IniFormat );
	settings.setValue( "translation", m_translation );
	settings.setValue( "pos", pos() );
	settings.setValue( "size", size() );
	settings.setValue( "testCount", m_examTab->getTestCount() );
	settings.setValue( "blankCount", m_copyTab->getBlankRowsCount() );
	settings.setValue( "boxChecked", m_examTab->getBoxChecked() );
	settings.setValue( "displayOnlyColumn2", m_copyTab->getDisplayOnlyColumn2() );
	settings.setValue( "vocabulary", m_topicListWidget->getCurrentVocDir() );
}



/************************************************************
* SLOTS
************************************************************/
/**
 * Display informations on Vokabb
 */
void QvokMainWindow::aboutVokabb()
{
	QMessageBox* aboutBox = new QMessageBox(this);
	aboutBox->setWindowTitle( "Vokabb" );
	aboutBox->setIconPixmap( QPixmap(":/vokabb.icon") );
	aboutBox->setText( tr(
			"<p>Vokabb is an open source application under GPL2 license. Please refer to "
			"<a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a> for the license details."
			"<p>Authors : Copyright vokabb development team."
			"<br/>Website: <a href=\"http://www.vokabb.org\">www.vokabb.org</a></p>"
			"<p>Special thanks to all testers.</p>"
			));
	aboutBox->setTextFormat(Qt::RichText);
	aboutBox->exec();
}
/**
 * Display informations on Qt
 */
void QvokMainWindow::aboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

/**
 * Choose the feel and look of the application with a comboBox.
 */
void QvokMainWindow::chooseStyle()
{
	QStyleDialog *styleDialog= new QStyleDialog( this );
	styleDialog->exec();

}

/**
 * Display a comboBox to choose the application languages.
 * There is no dynamic language changes but the language choosen
 * is saved in the settings. The new language will be displayed at 
 * the next restart.
 */
void QvokMainWindow::translate()
{
	QTranslateDialog *translateDialog= new QTranslateDialog( this );
	connect( translateDialog, SIGNAL( translate(const QString &) ), SLOT( setTranslation(const QString &) ));
	translateDialog->exec();
}
void QvokMainWindow::setTranslation( const QString &translation )
{
	m_translation = translation;
}


