#include "note.h"
//#include "playablenotes.h"

//Note::Note()
//{
//
//}

void Note::OrderNotes(vector<PlayableNote>& input)
{

	PlayableNote temp;
	for(unsigned int i2=0; i2 <= input.size(); i2++)
	{
		for(unsigned int j=0; j < input.size()-1; j++)
		{
			//Swapping element in if statement
			if(input[j].bar_location > input[j+1].bar_location)
			{
				temp=input[j];
				input[j]=input[j+1];
				input[j+1]=temp;
			}
		}
	}
}

vector<PlayableNote> Note::FindGoodTones(int dataCount, double tresh, int med_dimension_ratio, int med_note_widht, int med_note_height,  NoteRecogniser &recogniser)
{
	//TODO Move to notes
	Mat segment_cpy;
	Mat threshold_output;

	image.copyTo(segment_cpy);

	threshold(segment_cpy, threshold_output, tresh, 255, THRESH_BINARY);

	vector<RotatedRect> ellipses = calcProj.DetectEllipses(threshold_output);
	vector<PlayableNote> foundnotes;

	for(unsigned int i=0; i<ellipses.size(); i++)
	{
		if(abs(med_dimension_ratio- ellipses[i].size.width/ ellipses[i].size.height) < 20 && abs( ellipses[i].size.width - med_note_widht) < 10 && abs( ellipses[i].size.height -med_note_height) < 10)
		{

			if(saveTrainData){
				ostringstream convert;   // stream used for the conversion
				convert << dataCount;      // insert the textual representation of 'Number' in the characters in the stream
				string name = convert.str() +".png";
				recogniser.SaveLearnImage(segment_cpy,ellipses[i] , name);
				dataCount++;
			}

			float result = recogniser.EvalData(segment_cpy,ellipses[i]);
			cout<<"result "<<result<<endl;

			if(result == 1)
			{
				PlayableNote foundNote = GetNewTone(ellipses[i].center);


				if(foundNote.octave > 0 && foundNote.octave <5)
				{
					Mat cpy;
					segment_cpy.copyTo(cpy);
					circle(cpy,Point((int)ellipses[i].center.x,(int)ellipses[i].center.y), 5, Scalar(20,20,20) );
					imshow("detected ",cpy);

					foundnotes.push_back(foundNote);

					cout<<"ided note "<<endl;
					waitKey();
					destroyWindow("detected");
				}

			}
		}
	}

	if(foundnotes.size()>1)
		OrderNotes(foundnotes);
	///
	return foundnotes;
}


PlayableNote Note::GetNewTone( Point2f center)
{

	PlayableNote new_note;
	int highestOctave =2;
	int notes_per_octave= 7;

	float start_ = y_highest_staff - center.y;
	float note_dist =  avg_staff_distance*0.5;
	cout<<"note_dist "<<note_dist<<endl;
	float half_staves_removed = highestOctave * notes_per_octave + (start_ /note_dist );

	new_note.note_ID = (int)((int)half_staves_removed) % notes_per_octave;//notes per octave
	new_note.octave =highestOctave + (int)( start_/ (avg_staff_distance*4));
	new_note.bar_location = bar_location + (int)center.x;
	new_note.duration = 1;

	cout<<"plabale note : @ "<<new_note.bar_location <<" id "<<new_note.note_ID<<" oc "<<	new_note.octave<<endl;

	return new_note;

}

