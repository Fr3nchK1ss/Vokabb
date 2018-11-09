#include <QtGui>
#include <QApplication>

#include "mdl_testmodel.h"

#define ANSWER_COL 1

#define NO_TEST 0
#define LANG_1 1
#define LANG_2 2
#define MIX 3

/******************************************************
 * QTestItemModel implementation
 ******************************************************/

QTestItemModel::QTestItemModel(int rows, int columns, QObject * parent) : QStandardItemModel(rows, columns, parent)
{
	//insert a finish row
	insertRow( 0 );
	const QImage finishImage(":/finish.png");
	m_finishBrush = QBrush(finishImage);

	setData(index(0,0), m_finishBrush, Qt::BackgroundRole);
	setData( index(0, ANSWER_COL), m_finishBrush, Qt::BackgroundRole);
	setData(index(0,2), m_finishBrush, Qt::BackgroundRole);

	m_rowCount = 0;//number of rows actually filled in the models by data
	m_vocName = "";
}


/***********************
 * public
 ***********************/
/**
 * Allows to set the text of the headerView
 * @arg test: the type of test being performed, NO_TEST by default
 */
void QTestItemModel::setHeader(QString vocName, short examType)
{
	m_vocName = vocName;
	QStringList tokens = (vocName).split("__");
	QString lang1 = tokens.value(0);
	QString lang2 = tokens.value(1);
	QString ans = tr("your answer");
	QStringList s;

	switch ( examType )
	{	
		case NO_TEST:
			s<<lang1<<lang2;
			break;
		case LANG_1:
			s<<lang1<<ans<<lang2;
			break;
		case LANG_2:
			s<<lang2<<ans<<lang1;
			break;
		case MIX:
			{
			QString mix = lang1.append(" / ").append(lang2);
			s<<mix<<ans<<mix;
			}
			break;
		default:
			break;//never reached
	}

	setHorizontalHeaderLabels(s);
}


/**
 * update the model
 */
void QTestItemModel::setCouple(QString word1, QString word2)
{	
	insertRow( m_rowCount );

	setData(index(m_rowCount, 0), word1);
	setData(index(m_rowCount, 0), QColor( 255, 255, 230 ), Qt::BackgroundRole);

	setData(index(m_rowCount, 1), word2);
	setData(index(m_rowCount, 1), QColor( 255, 255, 230 ), Qt::BackgroundRole);

	++m_rowCount;

}


/**
 * Clean completely the view and reset the header
 */
void QTestItemModel::restoreModel()
{
	if ( columnCount() == 3 )
	{
		removeColumn( ANSWER_COL );
		this->setHeader(m_vocName, NO_TEST);
	}

	removeRows(0, m_rowCount );
	m_rowCount = 0;
}


/******************************************************
 * Exercise management
 ******************************************************/
/**
 * Add a ANSWER column for the test. If the column is already
 * there, remove it first to clean previous answers.
 */
void QTestItemModel::prepareTest( short examType )
{
	if (columnCount() == 3)
	{
		removeColumn( ANSWER_COL );
		insertColumn ( ANSWER_COL );
		setData( index(m_rowCount, ANSWER_COL), m_finishBrush, Qt::BackgroundRole);
	}
	else
	{
		insertColumn ( ANSWER_COL );
		setData( index(m_rowCount, ANSWER_COL), m_finishBrush, Qt::BackgroundRole);
	}

	this->setHeader( m_vocName, examType );
}


/**
 * correct() is only called during an exercise, when columnCount == 3
 * correct() looks if the data in ANSWER_COL corresponds to a data in col 0 or col 1.
 * If data in col 0 or col 1 has elements separated by ';', a substring match is 
 * considered correct.
 * It is usefull for synonymous.
 * For example the answer 'say' for the correct data 'say; tell' is considered correct.
 */
void QTestItemModel::correct(short testCount)
{
	if ( columnCount() == 3 )//should always be true
	{
	 QString answer, correct1, correct2;
	 short success = 0;
	 QModelIndex currentIndex;

	 for (int i=0; i < m_rowCount; ++i)
	 {
		currentIndex = index(i, 2);
			correct1 = data(currentIndex).toString();
		currentIndex = index(i, 0);
			correct2 = data(currentIndex).toString();
		currentIndex = index(i, ANSWER_COL );
			answer = data(currentIndex).toString();
		
		bool match1 = false;
		QStringList tokens = correct1.split(" ; ");
		for ( short t=0; t < tokens.size(); ++t )
		{
			if ( answer.compare( tokens.value( t ), Qt::CaseInsensitive) == 0 )
				match1 = true;
		}
		bool match2 = false;
		tokens = correct2.split(" ; ");
		for ( short t=0; t < tokens.size(); ++t )
		{
			if ( answer.compare( tokens.value( t ), Qt::CaseInsensitive) == 0 )
				match2 = true;
		}

		if ( match1 || match2 )
		{
			++success;
			setData(currentIndex, QColor( 240, 255, 240 ), Qt::BackgroundRole);
		}
		else	
			setData(currentIndex, QColor( 255, 240, 240 ), Qt::BackgroundRole);
	 }

	 QString msg = QString::number( success );
	 msg.append(tr(" / ")).append( QString::number( testCount ) );
	 QFont totalFont = QApplication::font();
	 totalFont.setBold(true);

	 setData( index(m_rowCount, ANSWER_COL), msg);
	 item( m_rowCount, ANSWER_COL )->setFont( totalFont );
	 item( m_rowCount, ANSWER_COL )->setTextAlignment( Qt::AlignCenter );
	}
}

