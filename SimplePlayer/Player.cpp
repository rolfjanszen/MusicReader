#include "Player.h"

#include <iostream>

#include "../include/playablenote.h"

Player::Player() :
  _keyToNoteString(89, "")
{
  fillArray();
}

Player::~Player()
{

}

void Player::printNotes(const vector<PlayableNote>& notes) const
{
  for(const PlayableNote& p : notes)
  {
    cout << _keyToNoteString[p.note_ID] << endl;
  }
}

void Player::fillArray()
{
  int key = 1;
  int oct = 0;
  char note[2];
  note[0] = 'A';
  note[1] = 0;

  char varS[2];
  varS[0] = '#';
  varS[1] = 0;

  char varB[2];
  varB[0] = 'b';
  varB[1] = 0;

  int noteInOctave = 0;

  while(key <= 88)
  {
    string str = "";

    // Add note.
    str += string(note);

    // add sharp
    if(noteInOctave == 1 || noteInOctave == 4 || noteInOctave == 6 || noteInOctave == 9 || noteInOctave == 11 )
    {
      str += string(varB) ;
    }
    else
    {
      if(note[0] == 'C')
      {
        ++oct;
      }
      note[0] = (char)(note[0] + 1);
    }

    // Add octave number;
    str += to_string(oct);

    _keyToNoteString[key] = str;

    // Increase all.
    if(noteInOctave == 11)
    {
      noteInOctave = -1;
      note[0] = 'A';
    }
    ++noteInOctave;
    ++key;
  }
}
