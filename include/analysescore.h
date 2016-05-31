#ifndef ANALYSESCORE_H
#define ANALYSESCORE_H

#include "getprojection.h"
#include "DataProc.h"
#include "bar.h"

class AnalyseScore
{

public:
    AnalyseScore();
    void DecipherScore(Mat &sheet);


private:
	vector<Bar> bars;
	GetProjection calcProj;
	float angle_bin;
	Mat sheet_copy;
	float note_angle, note_widht, note_height;
	vector<vector<Point2i > > ClusterConvexHulls(vector<int> data);
	vector< pair <int,int > >  FindROIRanges(vector<int> data, int min_grade_length, int grad_tresh);

    vector<int> FindStave();
    void SegmentNotes();
    void IdNotes();
    void CreateBars(vector<int> &staves, Mat &sheet_img);
    void FindPrevalantEllipse(Mat &sheet);
};

#endif // ANALYSESCORE_H
