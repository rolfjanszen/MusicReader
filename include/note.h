#ifndef NOTE_H
#define NOTE_H

#include "opencv/cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "noterecogniser.h"
#include "getprojection.h"
#include "playablenote.h"

using namespace std;
using namespace cv;
//
//class NoteSegment{
//public:
//    NoteSegment(){note=false;}
//    int start_x, start_y;
//    int stop_x, stop_y;
//    bool note;
//    Mat sub_I;
//};

class Note
{
public:

	Note(){}

	Note(int bar_location_, Mat image_, float &avg_bar_dist, float &highest_staff) :
		bar_location(bar_location_),image(image_), y_highest_staff(highest_staff), avg_staff_distance(avg_bar_dist){}

	vector<int> Xprojection; //along bar
	vector<PlayableNote> FindGoodTones(int dataCount, double tresh, int med_dimension_ratio, int med_note_widht, int med_note_height,  NoteRecogniser &recogniser, bool Fkey);
	int X_median;
	int X_avg;
	//    int tone;
	//    double duration;

//	vector<NoteSegment> sub_segments;


private :
	GetProjection calcProj;
	bool saveTrainData;
//	NoteRecogniser Identifier;
	PlayableNote GetNewTone( Point2f center, bool Fkey);
	void OrderNotes(vector<PlayableNote>& input);
	int bar_location;
	Mat image;
	float y_highest_staff;
	float avg_staff_distance ;

};

class ProcessingNote{

};


#endif // NOTE_H
