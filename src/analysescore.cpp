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

	KNN.TrainKNN();

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
			 new_bar.avg_staff_distance = (staves[4] - staves[0])/4;
			 new_bar.y_highest_staff = range;
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
    	//TODO Move to bars
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



void  AnalyseScore::IdNotes()
{
	string dataLocation = "C:\\Users\\rjanszen\\workspace\\Readmusic\\data\\";
	RNG rng(12345);
	vector<Point2f> notes;
	double thresh = 10;
	float dimension_ratio =	note_widht/ note_height;
	int trainImgDimensio =20;
	int dataCount = 0;
	KNN.TrainKNN();

	for(vector<Bar>::iterator bar_it=bars.begin();bar_it != bars.end() ; bar_it++)
	{
		int notebar_tresh = round(bars[0].notes[0].image.rows/3);
		vector<PlayableNote> music;
		for(vector<Note>::iterator note_it = bar_it->notes.begin()+1 ; note_it !=  bar_it->notes.end(); note_it++ )
		{

			//TODO Move to notes
			Mat segment_cpy;
			Mat threshold_output;

			note_it->image.copyTo(segment_cpy);

			threshold(segment_cpy, threshold_output, thresh, 255, THRESH_BINARY);

			vector<RotatedRect> ellipses = calcProj.DetectEllipses(threshold_output);

			for(unsigned int i=0; i<ellipses.size(); i++)
			{

				if(abs(dimension_ratio- ellipses[i].size.width/ ellipses[i].size.height) < 20 && abs( ellipses[i].size.width -note_widht) < 10 && abs( ellipses[i].size.height -note_height) < 10)
				{

					if(saveTrainData){
						ostringstream convert;   // stream used for the conversion
						convert << dataCount;      // insert the textual representation of 'Number' in the characters in the stream
						string name = convert.str() +".png";
						KNN.SaveLearnImage(segment_cpy,ellipses[i] ,dataLocation, name);
					}
//					const KNearest newknn = KNN.TrainKNN();
//					write(&newknn,"knntree");
//					newknn.
					float result = KNN.EvalData(segment_cpy,ellipses[i]);
					cout<<"result "<<result<<endl;
					if(result == 1)
					{
						PlayableNote newNote;

					}
				}
			}
		}
	}

	cout<<"done ID notes"<<endl;
}

