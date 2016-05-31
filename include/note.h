#ifndef NOTE_H
#define NOTE_H

#include "opencv/cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;
class NoteSegment{
public:
    NoteSegment(){note=false;}
    int start_x, start_y;
    int stop_x, stop_y;
    bool note;
    Mat sub_I;
};

class Note
{
public:

	Note();
    Note(int bar_location_, Mat image_): bar_location(bar_location_),image(image_){}
    vector<int> Xprojection; //along bar



    int X_median;
    int X_avg;

    int tone;
    double duration;
    int bar_location;
    Mat image;
    vector<NoteSegment> sub_segments;
};

#endif // NOTE_H
