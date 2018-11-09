#include <QtGui>

#include "mdl_testmodel.h"

/************************************************************************************
 * QTestRandProxyModel implementation
 * Logically QTestRandProxyModel should be a simple inheritance of 
 * QAbstractItemModel because only mapToSource is used.
 * But due to QT bug I experimented, a QSortFilterProxyModel can not be set correctly
 * on top of a QAbstractItemModel (through setSourceModel() ).
 * So QTestRandProxyModel is an inheritance of QSortFilterProxyModel.
 ************************************************************************************/

/**
 * Constructor
 */
QTestRandProxyModel::QTestRandProxyModel( QObject * parent ) : QSortFilterProxyModel(parent)
{
	m_table_size = 0;
	m_isSwitchedRow = NULL;
}


/**
 * Not used according to the qDebug("fromsource") commented below.
 * But in case... it is implemented.
 */
QModelIndex QTestRandProxyModel::mapFromSource( const QModelIndex& sourceIndex) const
{	
	//qDebug("fromsource");
	//mapToSource is the same than mapFromSource because the columns swap is symetric
	return mapToSource(sourceIndex);
}

/**
 * Randomizes indexes between columns 1 and 3
 */
QModelIndex QTestRandProxyModel::mapToSource( const QModelIndex& proxyIndex) const
{
	int col = proxyIndex.column();
	int row = proxyIndex.row();

	if ( row < m_table_size && m_isSwitchedRow[row] )
	{
		switch (col)
		{
			case 0: //swap
				return sourceModel()->index(row, 2);
				break;
			case 1: //do nothing
				return sourceModel()->index(row, 1);
				break;
			case 2: //swap
				return sourceModel()->index(row, 0);
				break;
			default://never reached
				return QModelIndex();
		}
	}
	else
		return sourceModel()->index(row, col);
}


/**
 * Builds a lookup table m_isSwitchedRow to remap the columns in a random way.
 * @arg sourceModel : model to shuffle
 */
void QTestRandProxyModel::randColumns( QAbstractItemModel* sourceModel )
{
	m_table_size = sourceModel->rowCount() - 1;
	//-1 because we do not rand the 'finish' blue row, it would be useless

	if (m_isSwitchedRow != NULL)
	{
		delete[] m_isSwitchedRow;
		m_isSwitchedRow = NULL;
	}
	m_isSwitchedRow = new bool[ m_table_size ];

	//rowCount() below is valid only because we just used setSourceModel()
	for (int row=0; row < m_table_size; ++row)
	{
		if ( (qrand()%2) == 0 )
		 m_isSwitchedRow[row] = true;
		else
		 m_isSwitchedRow[row] = false;
	}

	invalidate();
	setSourceModel(sourceModel);
}
