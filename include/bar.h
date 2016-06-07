#ifndef BAR
#define BAR
#include "note.h"
#include "getprojection.h"

class Bar{
public:
	vector<PlayableNote> playableNotes;
    vector<Note> notes;

    int staves[5];
    int y_loc_start;
    int y_loc_end;
    Mat bar_segment;
    float avg_staff_distance;
    float y_highest_staff;
    void GetNoteSegment(int note_staff, const int margin_note);
    void GetPlayableNotes(const int note_widht , const int note_height, int &dataCount, NoteRecogniser &Identifier , bool Fkey);
private :
   void WriteNote(const int range_start, const int range_end, const vector<int> Xbar_projections);
};

#endif // BAR

