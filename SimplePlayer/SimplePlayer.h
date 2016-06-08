#ifndef SIMPLEPLAYER_H
#define SIMPLEPLAYER_H

#include <vector>

// Inlcudes from the MusicReader.

// Own inlcudes.
#include "Player.h"
#include "ToneGenerator.h"


class SimplePlayer : public Player
{
public:
  SimplePlayer();

  void playNotes(const vector<PlayableNote>& notes);

private:
  ToneGenerator _toneGenerator;
};

#endif // SIMPLEPLAYER_H
