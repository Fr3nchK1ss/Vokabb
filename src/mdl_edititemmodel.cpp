#include <QtGui>

#include "mdl_editmodel.h"

/******************************************************
 * QEditItemModel implementation
 ******************************************************/

QEditItemModel::QEditItemModel(int rows, int columns, QObject * parent) : QStandardItemModel(rows, columns, parent)
{
	m_rowCount = 0;//number of rows actually filled in the models
}


/**
 * Allows to set the text of the headerView
 */
void QEditItemModel::setHeader(QString vocName)
{
   QStringList tokens = (vocName).split("__");
   //set the header data
   setHeaderData(0, Qt::Horizontal, tokens.value(0));
   setHeaderData(1, Qt::Horizontal, tokens.value(1));
   setHeaderData(2, Qt::Horizontal, trUtf8("Examples"));
}


/**
 * Clean completely the view
 */
void QEditItemModel::restoreModel()
{
	removeRows(0, m_rowCount);
	m_rowCount = 0;
}


/**
 * update the model
 */
void QEditItemModel::setCouple(QString word1, QString word2, QString word3)
{	
	setData(index(m_rowCount, 0), word1);
	setData(index(m_rowCount, 1), word2);
    if(!word3.isEmpty()) {
       setData(index(m_rowCount, 2), word3);
    }

	insertRow(rowCount());//insert a row a the very end of the model to compensate new data.
	++m_rowCount;;

}

/******************************************************
 * SLOTS
 ******************************************************/
/**
 * save the changes made to the topics in the model,
 * and suppress blank rows in the model to present a cleaner view.
 */
bool QEditItemModel::saveTopic(QString selection)
{
	if ( !selection.isEmpty() )
	{
		QFile file(selection);
		QTextStream stream(&file);

		if (file.open(QFile::WriteOnly | QFile::Text))
		{
			QString data0, data1, data2;
			for (int row=0; row < rowCount(); ++row)
			{
				data0 = data(index(row, 0)).toString();
				data1 = data(index(row, 1)).toString();
                data2 = data(index(row, 2)).toString();

				if ( !(data0.isEmpty() &&  data1.isEmpty()) )
					stream << data0 << " == " << data1 << " == " << data2 << "\n";
			}
			file.close();
			return true;
		}
		return false;
	}
	return false;
}


