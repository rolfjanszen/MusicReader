#include "analysescore.h"

int main(int argc, char *argv[])
{
	AnalyseScore analyze;

	Mat sheetmusic=imread("C:\\Users\\rjanszen\\Documents\\free_sheet_music1.jpg",0);

   vector<Bar> playbar = analyze.Run(sheetmusic);


    //TODO simpleplayer

    return 0;
}
