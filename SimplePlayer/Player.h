#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

using namespace std;

// Forward declarations.
class PlayableNote;

class Player
{
public:
  Player();
  virtual ~Player();

  virtual void playNotes(const vector<PlayableNote>& notes) = 0;
};

#endif // PLAYER_H
