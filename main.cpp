#include <QApplication>
#include <QDebug>
#include "viewer.h"
#include "graph_viewer.h"
#include "db_handler.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(viewer); //initializes resource file for viewer
							//resource file includes icons etc.
							//should be called from main()

	Viewer viewer;
	viewer.show();

       GraphViewer gv;
        gv.show();

	return app.exec(); 
}
