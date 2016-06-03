#ifndef BAR
#define BAR
#include "note.h"


class Bar{
public:

    vector<Note> notes;
    int staves[5];
    int y_loc_start;
    int y_loc_end;
    Mat bar_segment;
    int avg_staff_distance;
    int y_highest_staff;
    void GetNewTone(Note note, Point2f center);
};

#endif // BAR

