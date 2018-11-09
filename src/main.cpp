#include <QApplication>
#include <QTranslator>
#include <QTime>
#include <QLocale>
#include <QSettings>
#include <QDebug>

#include "vokmainwindow.h"


int main(int argc, char* argv[])
{
	//QApplication::setStyle(QStyleFactory::create(styleName));
	QApplication vokabb(argc, argv);
	

	/*************** SET APPLICATION LANGUAGE *******************/
	QSettings settings("./vokabb.ini", QSettings::IniFormat);
	QString translation = settings.value("translation").toString();

	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;

	if ( translation.compare("Francais") == 0 )
	{
		translator.load("vokabb_fr","languages");
		vokabb.installTranslator(&translator);
	}
	if (translation.compare("Svenska") == 0 )
	{
		translator.load("vokabb_sv","languages");
		vokabb.installTranslator(&translator);
	}

	//translator.load(QString("vokabb_") + locale,"/languages");
	

	QTime time = QTime::currentTime();
	uint seed = time.msec() + time.second()*1000;
	qsrand(seed);

	QvokMainWindow vokMainWindow;
	vokMainWindow.show();
	
	return vokabb.exec();

}
	

