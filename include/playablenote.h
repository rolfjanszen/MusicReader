#ifndef PLAYABLENOTE_H
#define PLAYABLENOTE_H


class PlayableNote{
public :
	int note_ID;
	int octave;
	double duration;
	int bar_location;

  PlayableNote(int note_ID_, int octave_, double duration_, int bar_location_) :
    note_ID(note_ID_),
    octave(octave_),
    duration(duration_),
    bar_location(bar_location_)
  {}
};

#endif
