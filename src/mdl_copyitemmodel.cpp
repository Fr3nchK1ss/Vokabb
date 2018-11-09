#include <QtGui>

#include "mdl_copymodel.h"

/******************************************************
 * QCopyItemModel implementation
 ******************************************************/

QCopyItemModel::QCopyItemModel(int rows, int columns, int blankRows, QObject * parent) : QStandardItemModel(rows, columns, parent)
{
	//insert a finish row
	insertRow( 0 );
	const QImage finishImage(":/finish.png");
	QBrush finishBrush(finishImage);	

	setData(index(0,0), finishBrush, Qt::BackgroundRole);
	setData(index(0,1), finishBrush, Qt::BackgroundRole);
	setData(index(0,2), finishBrush, Qt::BackgroundRole);

	m_rowCount = 0;//number of rows actually filled in the models by data
	m_currentBlankRows = blankRows;
}


/**
 * Allows to set the text of the headerView
 */
void QCopyItemModel::setHeader(QString vocName)
{
	QStringList tokens = (vocName).split("__");
	QString lang1 = tokens.value(0);
	QString lang2 = tokens.value(1);

	//set the header data
	setHeaderData(0, Qt::Horizontal, lang1);
	setHeaderData(1, Qt::Horizontal, lang2);
	setHeaderData(2, Qt::Horizontal, lang1.append(" <> ").append(lang2));
}


/**
 * restore model with high violence
 */
void QCopyItemModel::restoreModel()
{
	//Clear the previous selection
	removeRows(0, m_rowCount*(m_currentBlankRows+1));
	m_rowCount = 0;
}


/**
 * update the model
 */
void QCopyItemModel::setCouple(QString word1, QString word2)
{
	insertRows( m_rowCount*(m_currentBlankRows+1), m_currentBlankRows+1);

	QModelIndex currentIndex = index(m_rowCount*(m_currentBlankRows+1), 0);
	setData(currentIndex, word1);
	setData(currentIndex, QColor( 255, 255, 230 ), Qt::BackgroundRole);

	currentIndex = index(m_rowCount*(m_currentBlankRows+1), 1);
	setData(currentIndex, word2);
	setData(currentIndex, QColor( 255, 255, 230 ), Qt::BackgroundRole);

	currentIndex = index(m_rowCount*(m_currentBlankRows+1), 2);
	setData(currentIndex, word1.append(" <> ").append( word2 ));
	setData(currentIndex, QColor( 255, 255, 230 ), Qt::BackgroundRole);
	setData(currentIndex, Qt::AlignCenter, Qt::TextAlignmentRole);

	++m_rowCount;

}



/**
 * Reimplements flags() so that the user can not modify the leading copy line.
 */
Qt::ItemFlags QCopyItemModel::flags(const QModelIndex &index) const
{
	if ( (index.row())%(m_currentBlankRows+1) == 0 )
		    return Qt::ItemIsEnabled;
	
	return 	Qt::ItemIsEnabled
		| Qt::ItemIsEditable;
	
}


/**
 * Put m_currentBlankRows blanks rows between each data row to allow the copy exercise.
 * Example (blankRows=3)
 * _________________________________________
 * | language 1   |       language 2       |
 * | word 1       | translation word 1     |
 * |              |                        | <- left blank for user
 * |              |                        | <- left blank for user
 * |              |                        | <- left blank for user
 * | word 2       | translation word 2     |
 * |              |                        |
 *
 * @param rowCount : correspond to the tableview m_rowCount, which is the number of rows actually filled with data != rowCount()
 * @param blankRows : the new number of blankRows the user wants.
 *
 */
void QCopyItemModel::setBlankRows(int blankRows)
{
	int diff = blankRows - m_currentBlankRows; //diff = number of rows to add/remove

	if (diff > 0)//rows to add
	{
		/*
		rowCount is the last row NUMBER filled with data. Row NUMBER start at 0.
		The iteration starts at the end of the model. This is why i starts at m_rowCount;
		Do a drawing for a couple of iterations to understand how it works.
		*/
		for (int i=m_rowCount; i > 0; --i)
			insertRows( i*(m_currentBlankRows+1), diff);
			//insertRows( i*(m_currentBlankRows + 1 line containing the data to copy) )
	}
	else if (diff < 0)//rows to remove
	{
		diff = -diff; //for better understanding, now diff is positive
		for (int i=m_rowCount+1; i > 0; --i)
			removeRows( i*(m_currentBlankRows+1)-diff, diff);
	
	}

	m_currentBlankRows = blankRows;

}
