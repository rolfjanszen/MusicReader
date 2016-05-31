#include "analysescore.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    AnalyseScore analyze;
    Mat sheetmusic=imread("C:\\Users\\rjanszen\\Documents\\free_sheet_music1.jpg",0);
//    medianBlur(sheetmusic,sheetmusic, 7);

//    imshow("sheetmusic ",sheetmusic);
//    waitKey();
    analyze.DecipherScore(sheetmusic);

    return 0;
}
