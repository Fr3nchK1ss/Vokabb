#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class QModelIndex;
class QString;
class QRegExp;

/*****************************************************
 * model and proxy associated to copy tab
 *****************************************************/
/**
 *
 */
class QSearchItemModel : public QStandardItemModel
{
Q_OBJECT

	int m_rowCount;
	QString m_vocName;

public:
	QSearchItemModel(int rows, int columns, QObject * parent);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	void setHeader( QString vocName );
	void setCouple(QString word1, QString word2);
	void restoreModel();
	bool searchSelection( QRegExp searchRegExp );
	bool searchVoc( QRegExp searchRegExp );

private:
	
	void setSearchCouple(QString word1, QString word2,  QString topicName, QRegExp searchRegExp);

};

/*

class QCopyProxyModel : public QSortFilterProxyModel
{
Q_OBJECT

	bool m_displayOnlyColumn2;

public:
	QCopyProxyModel(  QSearchItemModel* sourceModel, bool displayOnlyColumn2, QObject *parent );
	void toggleColDisplay();

protected:
	bool filterAcceptsColumn ( int source_column, const QModelIndex& *source_parent* ) const;

};
*/
#endif


