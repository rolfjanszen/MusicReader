#include "SimplePlayer.h"


SimplePlayer::SimplePlayer() :
  _toneGenerator()
{
}

void SimplePlayer::playNotes(const vector<PlayableNote>& notes)
{
  _toneGenerator.readNotes(notes);

  _toneGenerator.play();
}
