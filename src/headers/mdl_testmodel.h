#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QAbstractItemModel>

class QModelIndex;
class QString;
class QBrush;



/*****************************************************
 * model and proxy associated to test tab
 ******************************************************/
/**
 */
class QTestItemModel : public QStandardItemModel
{
Q_OBJECT
	int m_rowCount;
	QString m_vocName;
	QBrush m_finishBrush;

//functions
public:
	QTestItemModel(int rows, int columns, QObject * parent);

	void setHeader(QString vocName, short examType=0);
	void setCouple(QString word1, QString word2);
	void restoreModel();
	void correct(short testCount);
	void prepareTest( short examType );

private:
	void cleanTest();

};


/**
 */
class QTestProxyModel : public QSortFilterProxyModel
{
//use of signals/slots
Q_OBJECT

	int* m_lookup_table;
	int m_table_size;
	int m_hiddenColumn;
	int m_finishRow;

public:
	QTestProxyModel(QObject * parent/*, QAbstractItemModel* sourceModel*/);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
	QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
	void shuffle( QAbstractItemModel* sourceModel, int testCount );
	void hideColumn( int column );

protected:
	bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
	bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

};



/*****************************************************
* ProxyModels, sorting
******************************************************/

/**
 * This proxy model is used in exam mode to swap column 1 and 3 if the exam is from language 2 to language 1.
 * (From language 1 to language 2 no swap is needed)
 * ______________________________________________
 * | language 2   |  your answer   | language 1 |
 *
 */
class QTestSwapProxyModel : public QSortFilterProxyModel
{
Q_OBJECT

//functions
public:
	QTestSwapProxyModel( QObject * parent );
	QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
	QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
	void swapColumns( QAbstractItemModel* sourceModel );
};



/**
 * This proxy model is used in exam mode to switch randomly data from column 1 and 3.
 * __________________________________________________________
 * | language 1 and 2   |  your answer   | language 1 and 2 |
 *
 */
class QTestRandProxyModel : public QSortFilterProxyModel
{
Q_OBJECT

//members
	bool* m_isSwitchedRow;
	int m_table_size;

//functions
public:
	QTestRandProxyModel( QObject * parent );
	QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
	QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
	void randColumns( QAbstractItemModel* sourceModel );
};

#endif
