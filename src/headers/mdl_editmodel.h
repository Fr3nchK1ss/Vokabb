#ifndef EDITMODEL_H
#define EDITMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class QModelIndex;
class QString;


/*****************************************************
* Edit Model
******************************************************/

/**
 * Edit model for vokabb data. 
 * It is used by editproxy model and testproxy model.
 */
class QEditItemModel : public QStandardItemModel
{
Q_OBJECT
	int m_rowCount;

public:
	QEditItemModel(int rows, int columns, QObject * parent);
	void setHeader(QString vocName);
	void setCouple(QString word1, QString word2, QString word3);
	void restoreModel();
	bool saveTopic(QString selection);

};

#endif
