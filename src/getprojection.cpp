#include "getprojection.h"

GetProjection::GetProjection(){}


vector<int> GetProjection::ProjectPixels(Mat &sheet_image, proj_axis axis_choice, int threshold)
{
    vector<int> black_pixel_proj;

    assert(sheet_image.channels()==1);
    avg_heigth=0;

    vector<int> proj_occurences;

    if(axis_choice == Y_axis)
    {
        black_pixel_proj.resize(sheet_image.rows);
        proj_occurences.resize(sheet_image.cols+1,0);
        cout<<"sheet size "<<sheet_image.rows<<endl;

        for(int i=0; i<sheet_image.rows; i++)
        {
            int counter =0;
            for(int j=0;j<sheet_image.cols;j++)
            {
                if(sheet_image.at<uchar>(i,j)<threshold)
                counter++;
            }
            black_pixel_proj[i]=counter;

            proj_occurences[counter]++;
            avg_heigth+=counter;
//            cout<<" black_pixel_proj[i] "<<i<<" "<<black_pixel_proj[i]<<endl;

        }

        avg_heigth/=sheet_image.rows;

    }
    else if(axis_choice == X_axis)
    {
        black_pixel_proj.resize(sheet_image.cols);
        proj_occurences.resize(sheet_image.rows+1,0);

        for(int i=0; i<sheet_image.cols; i++)
        {
            int counter =0;
            for(int j=0;j<sheet_image.rows;j++)
            {
                if(sheet_image.at<uchar>(j,i)<threshold)
                    counter++;
            }
            black_pixel_proj[i]=counter;
            if(counter>0)
            	proj_occurences[counter]++;
//            cout<<" black_pixel_proj[i] "<<black_pixel_proj[i]<<endl;
            avg_heigth+=counter;
        }
        avg_heigth/=sheet_image.cols;
    }

    median = distance(proj_occurences.begin(), max_element(proj_occurences.begin(),proj_occurences.end()));
    cout<<"median "<<median<<endl;

    return black_pixel_proj;

}


void GetProjection::PlotProjections(vector<int> input, string plot_name)
{
    cout<<"PlotProjections"<<endl;

    int white_margin =60;
    int largest_val ;//= std::max_element(input.begin(), input.end()) + white_margin;
    Mat plot = Mat::ones(largest_val, input.size(), CV_8UC1)*245;
    cout<<" largest_val "<<largest_val<<" input.size() "<<input.size()<<" plot sz "<<plot.cols<<"  "<<plot.rows<<endl;

    for(int i=0;i<input.size();i++)
    {

        plot.rowRange(largest_val-input[i]-1,largest_val).colRange(i,i+1)=Mat::ones(input[i]+1,1,CV_8UC1);
    }

//    imshow(plot_name, plot);
//    waitKey();

}

vector<RotatedRect> GetProjection::DetectEllipses(const Mat &threshold_output)
{

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cout<<"IdNotes"<<endl;

	GetProjection findYnoteproj;

	Mat segment_cpy;
	int min_grade_length=2;
	int grad_tresh = 3;

	vector<pair < int,int > > start_stop;


	/// Find contours
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Find the rotated rectangles and ellipses for each contour
	vector<RotatedRect> minRect( contours.size() );
	vector<RotatedRect> minEllipse;

	for(unsigned int i = 0; i < contours.size(); i++ )
	{
//		minRect[i] = minAreaRect( Mat(contours[i]) );
		if( contours[i].size() > 5 )
		{
		RotatedRect ellipse = fitEllipse( Mat(contours[i]) );
		if(ellipse.size.height >0)
			minEllipse.push_back(ellipse);

		}
	}

	int nr_contours= minEllipse.size() ;
	cout<<"contours.size() "<<nr_contours <<endl;

	/// Draw contours + rotated rects + ellipses
//	Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
//
//	for( int i = 0; i< contours.size(); i++ )
//	{
//		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//		// contour
//		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//		// ellipse
//		ellipse(drawing, minEllipse[i], color, 2, 8 );
//		// rotated rectangle
//		Point2f rect_points[4]; minRect[i].points( rect_points );
//		for( int j = 0; j < 4; j++ )
//			line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
//	}

	return minEllipse;
}


