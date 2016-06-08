#include "SimplePlayer.h"


SimplePlayer::SimplePlayer() :
  Player(),
  _toneGenerator()
{
}

void SimplePlayer::playNotes(const vector<PlayableNote>& notes)
{
  _toneGenerator.readNotes(notes);

  //_toneGenerator.play();

  _toneGenerator.vaderJacob();
}
