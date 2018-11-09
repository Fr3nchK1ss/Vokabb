#include <QtGui>
#include <QApplication>

#include "mdl_searchmodel.h"

/******************************************************
 * QSearchItemModel implementation
 ******************************************************/

QSearchItemModel::QSearchItemModel(int rows, int columns, QObject * parent) : QStandardItemModel(rows, columns, parent)
{
	m_rowCount = 0;//number of rows actually filled in the models by data
	m_vocName = "";
}


/************************************
 * private functions
 ************************************/
/**
 *
 */
void QSearchItemModel::setSearchCouple(QString word1, QString word2,  QString topicName, QRegExp searchRegExp)
{
	insertRow( m_rowCount );

	QModelIndex currentIndex = index( m_rowCount, 0 );
	QFont foundFont = QApplication::font();
	foundFont.setBold(true);

	setData( currentIndex, word1 );
	setData( currentIndex, tr("In ").append(topicName), Qt::ToolTipRole );
	if ( word1.contains( searchRegExp ) )
		item( m_rowCount, 0 )->setFont(foundFont);

	currentIndex = index( m_rowCount, 1 );
	setData( currentIndex, word2 );
	setData( currentIndex, tr("In ").append(topicName), Qt::ToolTipRole );
	if ( word2.contains( searchRegExp ) )
		item( m_rowCount, 1 )->setFont(foundFont);

	++m_rowCount;
}


/************************************
 * public functions
 ************************************/
/**
 * Reimplements flags() so that the user can not modify the leading copy line.
 */
Qt::ItemFlags QSearchItemModel::flags(const QModelIndex &/*index*/) const
{
	return Qt::ItemIsEnabled;
	
}


/**
 * Allows to set the text of the headerView
 */
void QSearchItemModel::setHeader(QString vocName)
{
	QStringList tokens = (vocName).split("__");
	QString lang1 = tokens.value(0);
	QString lang2 = tokens.value(1);

	//set the header data
	setHeaderData(0, Qt::Horizontal, lang1);
	setHeaderData(1, Qt::Horizontal, lang2);

	m_vocName = vocName;
}


/**
 * restore model with high violence
 */
void QSearchItemModel::restoreModel()
{
	//Clear the previous search, including the status row if exists.
	removeRows(0, rowCount() );
	m_rowCount = 0;
}


/**
 * update the model
 */
void QSearchItemModel::setCouple(QString word1, QString word2)
{
	insertRow( m_rowCount );

	QModelIndex currentIndex = index(m_rowCount, 0);
	setData(currentIndex, word1);
	setData( currentIndex, QColor( 255, 255, 230 ), Qt::BackgroundRole);
	currentIndex = index(m_rowCount, 1);
	setData(currentIndex, word2);
	setData( currentIndex, QColor( 255, 255, 230 ), Qt::BackgroundRole);

	++m_rowCount;
}


/**
 * research a regular expression in the entire vocabulary
 */
bool QSearchItemModel::searchSelection( QRegExp searchRegExp )
{
	insertRow( 0 );//status row
	setData( index(0, 0), tr("search in progress...") );
	setData( index(0, 0), QColor( 255, 240, 240 ), Qt::BackgroundRole);
	setData( index(0, 1), QColor( 255, 240, 240 ), Qt::BackgroundRole);

	QFont foundFont = QApplication::font();
	foundFont.setBold(true);
	short success = 0;

	for ( short row=1; row < m_rowCount; ++row )
	{
	 for ( short col=0; col < 2; ++col )
	 {
		const QString dataString = data( index(row, col) ).toString();

		if ( dataString.contains( searchRegExp ) )
		{
			++success;
			item( row, col )->setFont(foundFont);
		}
	 }
	}

	if ( success == 0 )
	{
		setData( index(0, 0), tr("No result found.") );
		setData( index(0, 0), QColor( 255, 240, 240 ), Qt::BackgroundRole);
		setData( index(0, 1), QColor( 255, 240, 240 ), Qt::BackgroundRole);
	}
	else
	{
		QString msg = QString::number( success );
		msg.append(tr(" expressions found with: ")).append( searchRegExp.pattern() );
		setData( index(0, 0), msg );
		setData( index(0, 0), QColor( 240, 255, 240 ), Qt::BackgroundRole);
		setData( index(0, 1), QColor( 240, 255, 240 ), Qt::BackgroundRole);
	}
	return true;
}


/**
 * research a regular expression in the entire vocabulary
 */
bool QSearchItemModel::searchVoc( QRegExp searchRegExp )
{
	this->restoreModel();
	insertRow( 0 );//status row
	setData( index(0, 0), tr("search in progress...") );
	setData( index(0, 0), QColor( 255, 240, 240 ), Qt::BackgroundRole);
	setData( index(0, 1), QColor( 255, 240, 240 ), Qt::BackgroundRole);


	QDir vocDir(m_vocName);
	QStringList topicFileNames = vocDir.entryList( QStringList("*.vok"), QDir::Files );
	QStringList topicNames = topicFileNames;
	topicNames.replaceInStrings( ".vok", "" );

	QDir::setCurrent(m_vocName);
	for (short i=0; i < topicFileNames.size(); ++i)
	{
		QFile topicFile( topicFileNames.at(i) );

		if ( topicFile.open(QIODevice::ReadOnly | QFile::Text) )
		{
			//Do a text stream out of the file
			QTextStream stream(&topicFile);
			QString line;
			
			//search for searchRegExp
			while ( line = stream.readLine(), !line.isEmpty() )
			{
				QStringList tokens = line.split(" == ");
				QString lang1 = tokens.value(0);
				QString lang2 = tokens.value(1);
				
				if ( lang1.contains( searchRegExp ) || lang2.contains( searchRegExp ) )
					this->setSearchCouple( lang1, lang2, topicNames.at(i), searchRegExp );

			}
			topicFile.close();
		} 
		else
			continue;
	}
	QDir::setCurrent("..");

	if ( m_rowCount == 0 )
	{
		setData( index(0, 0), tr("No result found.") );
		setData( index(0, 0), QColor( 255, 255, 240 ), Qt::BackgroundRole);
		setData( index(0, 1), QColor( 255, 255, 240 ), Qt::BackgroundRole);
	}
	else
		removeRow( rowCount()-1 );

	return true;
}

