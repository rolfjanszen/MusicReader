#include "bar.h"


void Bar::WriteNote(const int range_start, const int range_end, const vector<int> Xbar_projections)
{
	Mat note_image = bar_segment.colRange(range_start, range_end);
	vector<int> projecX(Xbar_projections.begin() + range_start, Xbar_projections.begin() + range_end);


	Note new_note( range_start,note_image.clone(), avg_staff_distance, y_highest_staff);

	notes.push_back(new_note);

}

void Bar::GetPlayableNotes(const int note_widht , const int note_height, int &dataCount ,NoteRecogniser &Identifier, bool Fkey )
{

	RNG rng(12345);
//	vector<Point2f> notes;
	double thresh = 10;
	float dimension_ratio =	note_widht/ note_height;


	for(vector<Note>::iterator note_it = notes.begin(); note_it !=  notes.end(); note_it++ )
	{
		vector<PlayableNote> foundNotes = note_it->FindGoodTones( dataCount,  thresh,  dimension_ratio,  note_widht, note_height,Identifier,Fkey);

		if(foundNotes.size() > 0)
		{
			playableNotes.insert(playableNotes.end(),foundNotes.begin(),foundNotes.end());
			dataCount++;
		}
	}

}

void Bar::GetNoteSegment(int note_staff, const int margin_note)
{

    GetProjection findXbarproj;
    Mat blurred_bar;

	GaussianBlur(bar_segment,blurred_bar,Size(5,5), 1,1,BORDER_DEFAULT );

	vector<int> Xbar_projections = findXbarproj.ProjectPixels(blurred_bar,X_axis,190);

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
			int range_start =note_start_point - margin_note;
			int range_end=note_end_point + margin_note;

			WriteNote(range_start, range_end,  Xbar_projections);

			write_note=false;
		}
	}

}

