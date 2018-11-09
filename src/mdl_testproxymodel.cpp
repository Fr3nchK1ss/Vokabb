#include <QtGui>

#include "mdl_testmodel.h"

#define ANSWER_COL 1
#define NO_HIDDEN_COL -1

/******************************************************
 * QTestProxyModel implementation
 ******************************************************/

/**
 * Constructor
 */
QTestProxyModel::QTestProxyModel( QObject * parent ) : QSortFilterProxyModel(parent)
{
	m_table_size = 0;
	m_lookup_table = NULL;
	m_hiddenColumn = NO_HIDDEN_COL;
	
}

/*******************************
 * protected 
 *******************************/
/**
 * Hide a column of the model
 */
bool QTestProxyModel::filterAcceptsColumn ( int source_column, const QModelIndex& /*source_parent*/ ) const
{
	if (source_column == m_hiddenColumn)
		return false;
	else
		return true;
}


/**
 * Hide a row of the model
 */
bool QTestProxyModel::filterAcceptsRow ( int source_row, const QModelIndex& /*source_parent*/ ) const
{
	if ( source_row >= m_table_size && source_row != m_finishRow)
		return false;
	else
		return true;
}


/******************************
 * public
 ******************************/
/**
 * Reimplements flags() so that the user can not modify the left column of the test model, neither the 'finish' row.
 */
Qt::ItemFlags QTestProxyModel::flags(const QModelIndex &index) const
{
	if ( index.column() == ANSWER_COL && index.row() != m_finishRow )
		return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
	//else, if column == 0 or 2
	return Qt::ItemIsEnabled;
}


/**
 * See mapToSource
 */
QModelIndex QTestProxyModel::mapFromSource( const QModelIndex& sourceIndex) const
{
	//qDebug()<<"mapFromSource";
	int col = sourceIndex.column();
	int row = sourceIndex.row();

	for (int i=0; i < m_table_size; ++i)
	{
		if ( m_lookup_table[i] == row )
			return sourceModel()->index( i, col );
	}

	return sourceModel()->index( row, col );
}


/**
 * Randomizes indexes whose rows are inferior to m_table_size, that is to 
 * say, just the rows displayed by filterAcceptRow.
 * For the row number m_table_size, display a blue 'finish' background.
 * For the other rows, do nothing.
 */
QModelIndex QTestProxyModel::mapToSource( const QModelIndex& proxyIndex) const
{
	//qDebug()<<"mapToSource";
	int col = proxyIndex.column();
	int row = proxyIndex.row();

	if (row < m_table_size)//row is in the look_up table
		return sourceModel()->index( m_lookup_table[row], col );
	else if (row == m_table_size)
		return sourceModel()->index( m_finishRow, col);
	else
		return sourceModel()->index( row, col );

}


/**
 * hide a column on request according to the test step.
 */
void QTestProxyModel::hideColumn( int column )
{
	m_hiddenColumn = column;
    invalidateFilter();/* -> sometimes not powerfull enough*/
}


/**
 * Builds a lookup table to remap the data in a random way.
 * The lookup table is used in mapToSource and mapFromSource.
 * The lookup table is of size testCount, because only 'testCount'
 * rows are displayed so it is not necessary to randomize the others.
 * @arg sourceModel : model to shuffle
 * @arg testCount : the number of rows to shuffle
 */
void QTestProxyModel::shuffle( QAbstractItemModel* sourceModel, int testCount )
{
	m_finishRow = sourceModel->rowCount()-1;//the blue 'finish' row
	if ( testCount >= m_finishRow )//should never happen but...
		testCount = m_finishRow;
	m_table_size = testCount;

	if (m_lookup_table != NULL)
	{
		delete[] m_lookup_table;
		m_lookup_table = NULL;
	}
	m_lookup_table = new int[ m_table_size ];
	short empty = -1;
	int row;

	//init rows to be randomized
	for ( row=0; row < m_table_size; ++row)
		m_lookup_table[row] = empty;

	//randomize rows
	int tmp, i;
	bool alreadyThere;
	for ( row=0; row < m_table_size; ++row)
	{
		//we want to fill the row 'row' which is 'empty'
		while( m_lookup_table[row] == empty )
		{
			alreadyThere = false;
			tmp = qrand()%m_table_size;
			//we look for tmp in the previous filled row
			for (i=0; i<row; ++i)
			{
				if (m_lookup_table[i] == tmp)
				{	//it is a miss, tmp already used
					alreadyThere = true;
					break;
				}

			}
			if (!alreadyThere)
				m_lookup_table[row] = tmp;
		}
	}
	
	invalidate();
	setSourceModel(sourceModel);
}

