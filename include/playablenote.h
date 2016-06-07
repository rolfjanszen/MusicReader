#ifndef PLAYABLENOTE_H
#define PLAYABLENOTE_H


class PlayableNote{
public :
	int note_ID;
	double duration;
	int bar_location;

	PlayableNote(){}
  PlayableNote(int note_ID_, double duration_, int bar_location_) :
    note_ID(note_ID_),
    duration(duration_),
    bar_location(bar_location_)
  {}
};

/**
  18 is de g van de G sleutel
  14 is de onderste c van de g sleutel.
  9 is de f van de f sleutel balk.

  */


#endif
