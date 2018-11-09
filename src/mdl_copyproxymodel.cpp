#include <QtGui>

#include "mdl_copymodel.h"

/******************************************************
 * QCopyProxyModel implementation
 ******************************************************/

/**
 * Constructor
 */
QCopyProxyModel::QCopyProxyModel(  QCopyItemModel *sourceModel, bool displayOnlyColumn2, QObject * parent ) : QSortFilterProxyModel(parent),  m_displayOnlyColumn2(displayOnlyColumn2)
{
	setSourceModel(sourceModel);
}



void QCopyProxyModel::toggleColDisplay()
{
	m_displayOnlyColumn2 = !m_displayOnlyColumn2;//toggle
	invalidateFilter();
}	


/**
 * Hide the third row of copyItemModel OR hide the two first row of copyItemModel
 * |	col 1	|	col 2	|	OR	|	col 3	|
 */
bool QCopyProxyModel::filterAcceptsColumn ( int source_column, const QModelIndex& /*source_parent*/ ) const
{
	if ( source_column == 0 || source_column == 1 )
		return !m_displayOnlyColumn2;
	else
		return m_displayOnlyColumn2;
}


