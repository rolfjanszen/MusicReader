#ifndef GETPROJECTION_H
#define GETPROJECTION_H

#include "essential.h"

enum proj_axis{X_axis, Y_axis};


// void operator<<(ostream& os, vector<int> np)
//{
//    for(int i=0;i<np.size();i++)
//        os<<" "<<np[i];
//}

class GetProjection
{

public:

    GetProjection();
    vector<RotatedRect> DetectEllipses(const Mat &image);
    vector<int> ProjectPixels(Mat &sheet_image, proj_axis axis_choice, int threshold);
    void PlotProjections(vector<int> input, string plot_name="plot");
    int GetAvarage(){return avg_heigth;}
    int Getmedian(){return median;}

private:
    int avg_heigth;
    int median;
};

#endif // GETPROJECTION_H
