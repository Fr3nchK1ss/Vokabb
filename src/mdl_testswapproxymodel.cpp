#include <QtGui>

#include "mdl_testmodel.h"

/******************************************************
 * QTestSwapProxyModel implementation
 ******************************************************/

/**
 * constructor
 */
QTestSwapProxyModel::QTestSwapProxyModel(QObject * parent) : QSortFilterProxyModel(parent) 
{}


/**
 * reimplements mapFromSource, symetric of mapToSource in fact
 */
QModelIndex QTestSwapProxyModel::mapFromSource( const QModelIndex& sourceIndex) const
{
	return mapToSource(sourceIndex);
}

/**
 * Swap indexes so that column 3 data become column 1 data and vice versa.
 */
QModelIndex QTestSwapProxyModel::mapToSource( const QModelIndex& proxyIndex) const
{
	int col = proxyIndex.column();
	int row = proxyIndex.row();

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


void QTestSwapProxyModel::swapColumns( QAbstractItemModel* sourceModel )
{
	setSourceModel( sourceModel );
}
