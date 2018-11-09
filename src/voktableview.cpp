#include <QtGui>
#include <QtWidgets>

#include "voktableview.h"
#include "voktablecornerbutton.h"
#include "mdl_copymodel.h"
#include "mdl_editmodel.h"
#include "mdl_searchmodel.h"
#include "mdl_testmodel.h"

#define EDITTAB 0
#define COPYTAB 1
#define EXAMTAB 2
#define SEARCHTAB 3

#define CORRECT_COL 2
#define NO_HIDDEN_COL -1
/*
 * The purpose of this view class is to display different models of data according to the tab selected
 * (edit,test, copy, search). In Qt View and Controler are merged so this view has the models as 
 * members. All changes to the models are processed through this view !
 * Its role can be split in 3 components:
 * - propose a specific tableView for Vokabb (headers...)
 * - models handling
 * - processing of tab events created by the user
 */

/**
 * Constructor
 */
QvokTableView::QvokTableView( int blankCount, bool displayOnlyColumn2, QString currentVocName, bool showExampleColumn, QWidget* parent ):QTableView(parent)
{
	//do not allow any selection
	setSelectionMode( QAbstractItemView::NoSelection );
	setSortingEnabled(true);

	
	/*******************************************
	*STRUCTURE OF THE VIEW
	*******************************************/	
	m_hHeader = new QHeaderView(Qt::Horizontal);
    m_hHeader->setSectionResizeMode(QHeaderView::Stretch);
	m_hHeader->setMinimumHeight(32);
	//m_hHeader->setSortIndicator(0, Qt::AscendingOrder);
	//m_hHeader->setSortIndicator(2, Qt::AscendingOrder);
	setHorizontalHeader(m_hHeader);
	
	m_vHeader = new QHeaderView(Qt::Vertical);
	m_vHeader->setMinimumWidth(32);
	setVerticalHeader(m_vHeader);
	
	//replace the upper left corner button of the header by a save button
	//QIcon icon = style()->standardIcon(QStyle::SP_DriveHDIcon);
	//QvokTableCornerButton* cornerBtt = new QvokTableCornerButton( QRect(2,2,32,32), icon, this);
	//cornerBtt->setIconSize( QSize(32,32) );


	/*********************************************
	*SETUP MODELS
	*********************************************/

	/*********** edit model *********************/
    if (showExampleColumn) {
       m_editItemModel = new QEditItemModel(40, 3, this);
    } else {
       m_editItemModel = new QEditItemModel(40, 2, this);
    } 
    m_editItemModel->setHeader(currentVocName);
	
	/********** copy exercise model ************/
    m_copyItemModel= new QCopyItemModel(0, 3, blankCount, this);
	m_copyItemModel->setHeader(currentVocName);
	m_copyProxyModel = new QCopyProxyModel( m_copyItemModel, displayOnlyColumn2, this);

	/********** test model ********************/
	m_testItemModel = new QTestItemModel(0, 2, this);
	m_testItemModel->setHeader(currentVocName);
	m_testProxyModel = new QTestProxyModel(this);
	m_testRandProxyModel = new QTestRandProxyModel(this);
	m_testSwapProxyModel = new QTestSwapProxyModel(this);

	/************* search model ***************/
	m_searchItemModel = new QSearchItemModel(0, 2, this);
	m_searchItemModel->setHeader(currentVocName);

	/**********************************************
	 * inner connections
	 **********************************************/
	connect(m_hHeader, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));
	//connect(cornerBtt, SIGNAL(clicked()), this, SLOT(saveTopic()));

	/**********************************************
	*SET INITIAL PARAMETERS
	***********************************************/

	m_selection = QStringList();
	m_currentTabIndex = EDITTAB;
	this->changeDataView(EDITTAB);
}


/**
 * clean the data in all the models, and remove rows depending on the model
 */ 
void QvokTableView::restoreItemModels()
{
	m_editItemModel->restoreModel();
	m_copyItemModel->restoreModel();
	m_testItemModel->restoreModel();
	m_searchItemModel->restoreModel();
}

/**
 * Open a vokabb file (.vok) and update the models accordingly.
 * openTopicFile() should be implemented in models classes but since there are
 * 2 models, it is more efficient to open the files once and fill the models in 
 * parallel.
 * @arg fileName : the name of the file to open (example.vok,...) 
 */
bool QvokTableView::openTopicFile(QString fileName)
{
	QFile file(fileName);
	
	if ( file.exists() )
	{
		if (file.open(QIODevice::ReadOnly | QFile::Text))
		{
			//Do a text stream out of the file
			QTextStream stream(&file);
			QString line;
			
			//we append the data of this file to the current model
			while ( line = stream.readLine(), !line.isEmpty() )
			{
				QStringList tokens = line.split(" == ");

				m_editItemModel->setCouple(tokens.value(0), tokens.value(1), tokens.value(2));
				m_copyItemModel->setCouple(tokens.value(0), tokens.value(1));
				m_testItemModel->setCouple(tokens.value(0), tokens.value(1));
				m_searchItemModel->setCouple(tokens.value(0), tokens.value(1));
			}

			file.close();
		} else {
			QMessageBox::warning(this, tr("File error"),
					     tr("Unable to open. Verify your access rights on this file."));
			return false;
		}
	} else {
		QMessageBox::warning(this, tr("File error"),
				     tr("The file corresponding to this topic does not exist."));
		return false;
	}
	return true;
}


/**
 * reimplement keyPressEvent to handle event according to the current tab
 * The code is directly taken from the library sources with a few modifications.
 */
void QvokTableView::keyPressEvent(QKeyEvent *event)
{

#ifdef QT_KEYPAD_NAVIGATION
    switch (event->key()) {
    case Qt::Key_Select:
        if (QApplication::keypadNavigationEnabled()) {
            if (!hasEditFocus()) {
                setEditFocus(true);
                return;
            }
        }
        break;
    case Qt::Key_Back:
        if (QApplication::keypadNavigationEnabled() && hasEditFocus())
            setEditFocus(false);
        else
            event->ignore();
        return;
    default:
        if (QApplication::keypadNavigationEnabled() && !hasEditFocus()) {
            event->ignore();
            return;
        }
    }
#endif

#if !defined(QT_NO_CLIPBOARD) && !defined(QT_NO_SHORTCUT)
    if (event == QKeySequence::Copy) {
        QVariant variant;
        if (model())
            variant = model()->data(currentIndex(), Qt::DisplayRole);
        if (variant.type() == QVariant::String)
            QApplication::clipboard()->setText(variant.toString());
        event->accept();
    }
#endif

    QPersistentModelIndex newCurrent;
    switch (event->key()) {
    case Qt::Key_Down:
        newCurrent = moveCursor(MoveDown, event->modifiers());
        break;
    case Qt::Key_Up:
        newCurrent = moveCursor(MoveUp, event->modifiers());
        break;
    case Qt::Key_Left:
        newCurrent = moveCursor(MoveLeft, event->modifiers());
        break;
    case Qt::Key_Right:
        newCurrent = moveCursor(MoveRight, event->modifiers());
        break;
    case Qt::Key_Home:
        newCurrent = moveCursor(MoveHome, event->modifiers());
        break;
    case Qt::Key_End:
        newCurrent = moveCursor(MoveEnd, event->modifiers());
        break;
    case Qt::Key_PageUp:
        newCurrent = moveCursor(MovePageUp, event->modifiers());
        break;
    case Qt::Key_PageDown:
        newCurrent = moveCursor(MovePageDown, event->modifiers());
        break;
    case Qt::Key_Tab:
        newCurrent = moveCursor(MoveNext, event->modifiers());
        break;
    case Qt::Key_Backtab:
        newCurrent = moveCursor(MovePrevious, event->modifiers());
        break;
    }

    QPersistentModelIndex oldCurrent = currentIndex();
    if (newCurrent != oldCurrent && newCurrent.isValid()) {
        if (!hasFocus() && QApplication::focusWidget() == indexWidget(oldCurrent))
            setFocus();
        QItemSelectionModel::SelectionFlags command = selectionCommand(newCurrent, event);
        if (command != QItemSelectionModel::NoUpdate
             || style()->styleHint(QStyle::SH_ItemView_MovementWithoutUpdatingSelection, 0, this)) {
            if (command & QItemSelectionModel::Current) {
                selectionModel()->setCurrentIndex(newCurrent, QItemSelectionModel::NoUpdate);
		/*
                if (d->pressedPosition == QPoint(-1, -1))
                    d->pressedPosition = visualRect(oldCurrent).center();
                QRect rect(d->pressedPosition - d->offset(), visualRect(newCurrent).center());
                setSelection(rect, command);
		*/
            } else {
                selectionModel()->setCurrentIndex(newCurrent, command);
                //d->pressedPosition = visualRect(newCurrent).center() + d->offset();
            }
            return;
        }

    }

    switch (event->key()) {
    // ignored keys
    case Qt::Key_Down:
    case Qt::Key_Up:
#ifdef QT_KEYPAD_NAVIGATION
        if (QApplication::keypadNavigationEnabled()) {
            event->accept(); // don't change focus
            break;
        }
#endif
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Escape:
    case Qt::Key_Shift:
    case Qt::Key_Control:
        event->ignore();
        break;
    case Qt::Key_Space:
    case Qt::Key_Select:
        if (!edit(currentIndex(), AnyKeyPressed, event) && selectionModel())
            selectionModel()->select(currentIndex(), selectionCommand(currentIndex(), event));
#ifdef QT_KEYPAD_NAVIGATION
/*
        if ( event->key()==Qt::Key_Select ) {
            // Also do Key_Enter action.
            if (currentIndex().isValid()) {
                if (state() != EditingState)
                    emit activated(currentIndex());
            } else {
                event->ignore();
            }
        }*/
#endif
        break;
#ifdef Q_WS_MAC
    case Qt::Key_Enter:
    case Qt::Key_Return:
        // Propagate the enter if you couldn't edit the item and there are no
        // current editors (if there are editors, the event was most likely propagated from it).
        if (!edit(currentIndex(), EditKeyPressed, event) && d->editors.isEmpty())
            event->ignore();
        break;
    case Qt::Key_O:
        if (event->modifiers() & Qt::ControlModifier && currentIndex().isValid())
            emit activated(currentIndex());
        break;
#else
    case Qt::Key_F2:
        if (!edit(currentIndex(), EditKeyPressed, event))
            event->ignore();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        // ### we can't open the editor on enter, becuse
        // some widgets will forward the enter event back
        // to the viewport, starting an endless loop
        if (state() != EditingState || hasFocus()) {
            if (currentIndex().isValid())
                emit activated(currentIndex());
            event->ignore();
        }
        break;
#endif
    case Qt::Key_A:
        if (event->modifiers() & Qt::ControlModifier) {
            selectAll();
            break;
        }
    default: {
        bool modified = (event->modifiers() == Qt::ControlModifier)
                        || (event->modifiers() == Qt::AltModifier)
                        || (event->modifiers() == Qt::MetaModifier);
        if (!event->text().isEmpty() && !modified) {
            if (!edit(currentIndex(), AnyKeyPressed, event))
                keyboardSearch(event->text());
        }
        event->ignore();
        break; }
    }
}
