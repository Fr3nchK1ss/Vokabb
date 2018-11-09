#ifndef COPYMODEL_H
#define COPYMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class QModelIndex;
class QString;


/*****************************************************
 * model and proxy associated to copy tab
 *****************************************************/
/**
 * Copy model for the copy exercise.
 * The copy model is used with the following view:
 * _________________________________________
 * | language 1   |       language 2       |
 * | word 1       | translation word 1     |
 * |              |                        | <- left blank for user
 * |              |                        | <- left blank for user
 * |              |                        | <- left blank for user
 * | word 2       | translation word 2     |
 * |              |                        |
 */
class QCopyItemModel : public QStandardItemModel
{
Q_OBJECT

	int m_rowCount;
	int m_currentBlankRows;

public:
	QCopyItemModel(int rows, int columns, int blankRows, QObject * parent);
	void setHeader( QString vocName );
	void setCouple(QString word1, QString word2);
	void restoreModel();
	void setBlankRows( int blankRows );

protected:
	Qt::ItemFlags flags(const QModelIndex &index) const;

};


/**
 * Hide the central row of CopyItemModel.
 */
class QCopyProxyModel : public QSortFilterProxyModel
{
Q_OBJECT

	bool m_displayOnlyColumn2;

public:
	QCopyProxyModel(  QCopyItemModel* sourceModel, bool displayOnlyColumn2, QObject *parent );
	void toggleColDisplay();

protected:
	bool filterAcceptsColumn ( int source_column, const QModelIndex& /*source_parent*/ ) const;

};

#endif


