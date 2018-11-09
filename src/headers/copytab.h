#ifndef COPYTAB_H
#define COPYTAB_H

#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

class QCopyTab : public QWidget
{
//use of signals/slots
Q_OBJECT

//members
	QSpinBox* m_linesSpinBox;
	QPushButton* m_oneColBtt;
	QPushButton* m_twoColBtt;
	QLabel *m_colLabel;
	bool m_displayOnlyColumn2;

public:
	QCopyTab(int blankCount, bool displayOnlyColumn2, QWidget *parent=0);
	const int getBlankRowsCount();
	bool getDisplayOnlyColumn2();

signals:
	void launchCopy(int);
	void colCountChanged();

private slots:
	void displayOneCol();
	void displayTwoCol();
	
};

#endif
