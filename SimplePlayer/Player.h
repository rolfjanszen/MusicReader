#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>

using namespace std;

// Forward declarations.
class PlayableNote;

class Player
{
public:
  Player();
  virtual ~Player();

  virtual void playNotes(const vector<PlayableNote>& notes) = 0;

  void printNotes(const vector<PlayableNote>& notes) const;
private:
  std::vector<string> _keyToNoteString;

  void fillArray();
};

#endif // PLAYER_H
