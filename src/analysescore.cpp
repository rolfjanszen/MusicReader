#include "analysescore.h"
#include <cmath>


AnalyseScore::AnalyseScore()
{
	angle_bin = 2;
}

void AnalyseScore::DecipherScore(Mat &sheet) //main fundtion
{
    sheet_copy=sheet;

    vector<int> staff_locations= FindStave();
    CreateBars(staff_locations,  sheet);

    SegmentNotes();

    FindPrevalantEllipse(sheet);

    IdNotes();

}


void AnalyseScore::FindPrevalantEllipse(Mat &sheet)
{
	DataProc<float> doMath;
	vector<RotatedRect> ellipses = calcProj.DetectEllipses(sheet);
	vector<float> angles, widht, height;

	unsigned int size = ellipses.size();

	for(unsigned int i=0; i <size; i++)
	{
		angles.push_back(round(ellipses[i].angle/angle_bin));
		widht.push_back(round(ellipses[i].size.width));
		height.push_back(round(ellipses[i].size.height));
		cout<<"sz "<<ellipses[i].size<<endl;
	}

	note_angle = doMath.CalcMedian(angles);
	note_widht = doMath.CalcMedian(widht);
	note_height = doMath.CalcMedian(height);
	waitKey();
}

 vector<int> AnalyseScore::FindStave()
{
    GetProjection findYproj;
//    imshow("sheet",sheet);
//    waitKey();

    vector<int> Y_projections = findYproj.ProjectPixels(sheet_copy,Y_axis,210);
    int avg_Y_height=findYproj.GetAvarage();

    findYproj.PlotProjections(Y_projections,"staves");

    cout<<"avg_Y_height "<<avg_Y_height<<endl;
    avg_Y_height*=2.5;

    vector<int> staves;

    int top_height=0, top_index;
    int staff_counter=0;

    for(int i=0;i<Y_projections.size();i++)
    {

        if(Y_projections[i]>avg_Y_height)
        {
            cout<<"found staff "<<endl;
            if(Y_projections[i] > top_height)
            {
                top_height=Y_projections[i];
                top_index=i;
            }
        }
        else
        {
            if(top_height>0 && top_height > Y_projections[top_index -1] && top_height > Y_projections[top_index +1]){

                staves.push_back(top_index);
                staff_counter++;
                cout<<"new staff @ "<<   top_index<<endl;
            }

            top_height=0;
        }
    }

    cout<<"staff_counter "<<   staff_counter<<endl;
    return staves;
}



void AnalyseScore::CreateBars(vector<int> &staves, Mat &sheet_img)
{

    while(staves.size()>=4){

        int bar_height= 25;//staves[4]-staves[0];
        int range=bar_height/1.5;

        Bar new_bar;
        new_bar.y_loc_start=staves[0];
        new_bar.y_loc_end=staves[4];

        for(int i=0;i<5;i++)
            new_bar.staves[i]=staves[i];

        if (staves[4] - staves[0] < bar_height)
		 {
			 new_bar.bar_segment=sheet_copy.rowRange( new_bar.y_loc_start-range,new_bar.y_loc_end+range);
			 bars.push_back(new_bar);
			 cout<<"staves "<<staves.size()<<endl;

			if(staves.size() > 4)
				staves.erase(staves.begin(),staves.begin()+4);
//                     System.Console.WriteLine("staves {0}", staves.Length);
//			 imshow(" new_bar.bar_segment ", new_bar.bar_segment);
//			 waitKey();
		 }
		 else
			 staves.erase(staves.begin());



        cout<<"staves "<<staves.size()<<endl;

    }
}


void AnalyseScore::SegmentNotes()
{

//    int bar_height= staves[4]-staves[0];
//    int range=bar_height/1.5;
    GetProjection findXbarproj;
    Mat blurred_bar;
    int margin_note=6;
    cout<<"bars.size() "<<bars.size()<<endl;
    int note_staff=0;

    for(int i=0;i<bars.size();i++)
    {

        cout<<"start_stave "<<bars[i].y_loc_start<<" "<<bars[i].y_loc_end<<endl;

//        Mat bar_segment=sheet_copy.rowRange(start_stave-range,staves[i]+range);
        note_staff++;
        GaussianBlur(bars[i].bar_segment,blurred_bar,Size(5,5), 1,1,BORDER_DEFAULT );

//        imshow("bar_segment",bars[i].bar_segment);
//        waitKey();

        vector<int> Xbar_projections = findXbarproj.ProjectPixels(blurred_bar,X_axis,190);
//        findXbarproj.PlotProjections(Xbar_projections);
//        waitKey();

        int median = findXbarproj.Getmedian();
        median *= 1.3; //plus margin;
        bool recording_note =false;
        int note_start_point, note_end_point;
        bool write_note=false;
        cout<<"Xbar_projections.size() "<<Xbar_projections.size()<<endl;
//        int delta_y=1;

        for(int j = 0; j < Xbar_projections.size(); j++ )
        {


            if( Xbar_projections[j] > median && !recording_note )
            {
                recording_note=true;
                note_start_point=j;
            }
            else if(Xbar_projections[j] < median && recording_note )
            {
                recording_note=false;
                note_end_point=j;
                write_note=true;
            }

            if(write_note)
            {
//                cout<<"write note "<<endl;

//                margin_note = (note_end_point-note_start_point)
                Note new_note;
                int range_start =note_start_point - margin_note;
                int range_end=note_end_point + margin_note;
                Mat note_image = bars[i].bar_segment.colRange(range_start, range_end);
                vector<int> projecX(Xbar_projections.begin() + range_start, Xbar_projections.begin() + range_end);

                new_note.Xprojection=projecX;
                note_image.copyTo( new_note.image);

                new_note.bar_location = note_start_point;
                bars[i].notes.push_back(new_note);
                write_note=false;

//                imshow("note_image", new_note.image);
//                waitKey();
//                destroyWindow("note_image");

            }
        }
    }
    cout<<"done segnment notes "<<endl;
}



void CheckEllipse(Mat image)
{

	int edgeThresh = 1;
	int lowThreshold;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;

	Mat detected_edges;
	/// Reduce noise with a kernel 3x3
	blur( image, detected_edges, Size(3,3) );

	/// Canny detector
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

	imshow("detected_edges ",detected_edges);
	waitKey();

}

void  AnalyseScore::IdNotes( )
{
	RNG rng(12345);
	vector<Point2f> notes;
	double thresh = 10;
	float dimension_ratio =	note_widht/ note_height;

	for(vector<Bar>::iterator bar_it=bars.begin();bar_it != bars.end() ; bar_it++)
	{
		int notebar_tresh = round(bars[0].notes[0].image.rows/3);

		for(vector<Note>::iterator note_it = bar_it->notes.begin()+1 ; note_it !=  bar_it->notes.end(); note_it++ )
		{
			Mat segment_cpy;
			Mat threshold_output;

			note_it->image.copyTo(segment_cpy);

			threshold(segment_cpy, threshold_output, thresh, 255, THRESH_BINARY);

			vector<RotatedRect> ellipses = calcProj.DetectEllipses(threshold_output);

			for(unsigned int i=0; i<ellipses.size(); i++)
			{
				if(abs(dimension_ratio- ellipses[i].size.width/ ellipses[i].size.height)<1 &&abs( ellipses[i].size.width -note_widht) < 4 && abs( ellipses[i].size.height -note_height) < 4)
				{

					cout<<"ellipse "<<ellipses[i].center<<" sz "<<ellipses[i].size<<" a "<<ellipses[i].angle<<endl;

					notes.push_back(ellipses[i].center);
					cout<<"not found : "<<ellipses[i].center<<endl;

					Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
					// contour
//					drawContours(segment_cpy, ellipses[i], i, color, 1, 8, vector<Vec4i>(), 0, Point() );
					// ellipse
					ellipse(segment_cpy, ellipses[i], color, 2, 8 );
					imshow("ellipse ",segment_cpy);
					imshow("threshold_output ",threshold_output);
					CheckEllipse(segment_cpy);
					waitKey();
					destroyWindow("ellipse ");

				}
			}

		/// Detect edges using Threshold

//			namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//			imshow( "Contours", drawing );
//			waitKey();
//			destroyWindow( "Contours");
		}
	}
}


//vector<vector<Point2i > >  AnalyseScore::ClusterConvexHulls(vector<int> data){
//
//    vector<vector<Point2i > > clusters;
//    int x_avg=0;
//    int y_avg=0;
//    int nr_clusters=0;
//    int i=0;
////int median =0;
//    int median = GetMedian(data);
//
//
//    cout<<"ClusterConvexHulls median "<<median<<endl;
//    clusters.resize(nr_clusters+1);
//    clusters[nr_clusters];
//    for(int i=0; i < (data.size()) ;i++)
//    {
//        if(data[i]>0)
//        {
//            if(clusters[nr_clusters].size()<2)
//                clusters[nr_clusters].push_back(Point2i(i,data[i]));
//            else
//            {
//                x_avg=0;
//                y_avg=0;
//                int clust_sz=clusters[nr_clusters].size();
//
//                for(int k=0; k < clust_sz ;k++)
//                {
//                    x_avg+=clusters[nr_clusters][k].x;
//                    y_avg+=clusters[nr_clusters][k].y;
//
//                }
//
//                x_avg /=(clust_sz);
//                y_avg /=clust_sz;
//
//                bool in_convex_hull = true;
//                cout<<"A (avg) "<<Point2i(x_avg, y_avg)<<" B "<<Point2i(i,data[i])<<endl;
//
//                for(int k=x_avg+1; k < clust_sz ;k++){
//                    cout<<"M1 "<<clusters[nr_clusters][k]<<" M2 "<<clusters[nr_clusters][k-1]<<endl;
//
//                   bool convexity = CheckCross( clusters[nr_clusters][k-1],clusters[nr_clusters][k], Point2i(x_avg, y_avg), Point2i(i,data[i]));
//                   if(!convexity || data[i]<=(median*1.1))
//                   {
//                       cout<<"lost convexity"<<endl;
//                       in_convex_hull = false;
//                   }
//                }
//
//                if(in_convex_hull)
//                {
//                 clusters[nr_clusters].push_back(Point2i(i,data[i]));
//                }
//                else
//                {
//                    nr_clusters++;
//                    clusters.resize(nr_clusters+1);
//                    clusters[nr_clusters].reserve(data.size()-i);
//                    clusters[nr_clusters].push_back(Point2i(i,data[i]));
//
//
//                }
//
//            }
//            }
//
//    }
//
//    cout<<"show cluseter"<<endl;
//
//    for(int i=0; i < (clusters.size()) ;i++){
//        cout<<" (clusters[i].size() "<<clusters[i].size()<<endl;
//        for(int j=0; j < (clusters[i].size()) ;j++)
//            cout<<clusters[i][j]<<" ";
//
//        cout<<endl;
//    }
//
//cout<<"done cluseter"<<endl;
//    return clusters;
//}
//


//
//void AnalyseScore::IdNotes()
//{
//
//    cout<<"IdNotes"<<endl;
//
//    GetProjection findYnoteproj;
//
//    Mat segment_cpy;
//    int min_grade_length=2;
//    int grad_tresh = 3;
//
//    vector<pair < int,int > > start_stop;
//
//    for(vector<Bar>::iterator bar_it=bars.begin();bar_it != bars.end() ; bar_it++){
//
//    	int notebar_tresh = bars[0].notes[0].image.rows/3;
//
//        cout<<"grad_tresh"<<grad_tresh<<endl;
//        for(vector<Note>::iterator note_it = bar_it->notes.begin() ; note_it !=  bar_it->notes.end(); note_it++ )
//        {
//
//            note_it->image.copyTo(segment_cpy);
//
//            findYnoteproj.PlotProjections(note_it->Xprojection,"Xprojection");
//            vector<int> proj_Y = findYnoteproj.ProjectPixels(segment_cpy,Y_axis ,140 );
//            cout<<"SmoothVector"<<endl;
//            vector<int> smooth_proj= SmoothVector( note_it->Xprojection,  3);
//            cout<<"PlotProjections"<<endl;
//            findYnoteproj.PlotProjections(smooth_proj,"smooth_proj");
//            cout<<"GetMedian"<<endl;
//
////            start_stop = FindROIRanges(smooth_proj, min_grade_length, grad_tresh);
//            ClusterConvexHulls(smooth_proj);
//
//            cout<<"PlotProjections"<<endl;
//
////            findYnoteproj.PlotProjections(proj_Y,"proj_Y");
//            cout<<"PlotProjections"<<endl;
//
////            findYnoteproj.PlotProjections(note_it->Xprojection,"proj x aaxis");
//
////            imshow("note_image",segment_cpy);
////            waitKey();
//
//        }
//    }
//}


//
//vector<pair< int,int > > AnalyseScore::FindROIRanges(vector<int> data,  int min_grade_length, int grad_tresh){
//
//    int grade_changes=0, no_change=0;
//
//
//    int start_X=0, end_X=0;
//    bool tracking_note=false;
//
//    vector<pair< int,int > > start_stop;
//    int prev_avg;
//    int avg_grad;
//    for(int i=0; i < (data.size()-1) ;i++)
//    {
//
//        int prev_proj = data[i+1];
//        int proj = data[i];
//        avg_grad=0;
//        for(int j=0;j<min_grade_length;j++)
//            avg_grad -=(proj-prev_proj);
//
//        cout<<"abs diff: "<<(proj-prev_proj) <<" avg_grad "<<avg_grad<<"  @ "<<i<<endl;
//
//        if((avg_grad) > grad_tresh)
//        {
//            grade_changes++;
//            no_change--;
//            cout<<"finalize"<<endl;
//            if(grade_changes>min_grade_length)
//                grade_changes=min_grade_length;
//            if(no_change<0)
//                no_change=0;
//        }
//        else if((-avg_grad) > grad_tresh){
//            grade_changes--;
//            no_change++;
//
//            if(no_change>min_grade_length)
//                no_change=min_grade_length;
//            if(grade_changes<0)
//                grade_changes=0;
//
//        }
//
//        if(grade_changes >= min_grade_length && !tracking_note)
//        {
//            no_change=0;
//            start_X = i  - min_grade_length;
//            tracking_note=true;
//        }
//        else if(no_change >= min_grade_length && tracking_note && avg_grad >=0 && prev_avg<0 && prev_avg >-3)
//        {
//            grade_changes=0;
//            end_X=i-min_grade_length;
//            tracking_note=false;
//            cout<<"start : "<<start_X<<" end "<<end_X<<"  grade_changes "<<grade_changes<<" no change e "<<no_change<<endl;
//
//        }
//
//        prev_avg=avg_grad;
//    }
//
//    return start_stop;
//}
//


