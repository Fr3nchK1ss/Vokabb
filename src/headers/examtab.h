#ifndef EXAMTAB_H
#define EXAMTAB_H

#include <QtWidgets>

class QGroupBox;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QString;
class QStringList;
class QToolButton;

class QExamTab : public QWidget
{
//use of signals/slots
Q_OBJECT
	
//members
	QSpinBox* m_wordsSpinBox;
	QToolButton* m_maxBtt;
	QGroupBox* m_translationGroupBox;
	QRadioButton* m_translationRadio1;
	QRadioButton* m_translationRadio2;
	QRadioButton* m_translationRadio3;
	QPushButton* m_startStopBtt;
	bool m_isTest;

public:
	QExamTab(QString vocName, int testNumber, int boxChecked, QWidget *parent=0);
	const int getTestCount();
	const short getBoxChecked();

private:
	void setupViews();
	
signals:
	void startStopTest(const short testNumber, const short boxChecked, const bool isTest);

public slots:
	void stopExam();
	void toggleBtt();
	void setLanguages(QString vocName);

private slots:
	void setMaxValue();

};

#endif
