
#include <iostream>

#include "../include/playablenote.h"

// Own inlcudes.
//#include "ToneGenerator.h"
#include "Player.h"
#include "SimplePlayer.h"

using namespace std;


vector<PlayableNote> generateExampleNotes()
{
  vector<PlayableNote> result;
  int noteDuration  = 300;

  // Vader Jacob.
  result.push_back(PlayableNote(13, noteDuration, 0));
  result.push_back(PlayableNote(14, noteDuration, 0));
  result.push_back(PlayableNote(15, noteDuration, 0));
  result.push_back(PlayableNote(13, noteDuration, 0));

  result.push_back(PlayableNote(13, noteDuration, 0));
  result.push_back(PlayableNote(14, noteDuration, 0));
  result.push_back(PlayableNote(15, noteDuration, 0));
  result.push_back(PlayableNote(13, noteDuration, 0));

  // Slaapt gij nog?
  result.push_back(PlayableNote(15, noteDuration, 0));
  result.push_back(PlayableNote(16, noteDuration, 0));
  result.push_back(PlayableNote(17, noteDuration * 2, 0));

  result.push_back(PlayableNote(15, noteDuration, 0));
  result.push_back(PlayableNote(16, noteDuration, 0));
  result.push_back(PlayableNote(17, noteDuration * 2, 0));

  // Alle Klokken luiden
  result.push_back(PlayableNote(17, noteDuration/2, 0));
  result.push_back(PlayableNote(18, noteDuration/2, 0));
  result.push_back(PlayableNote(17, noteDuration/2, 0));
  result.push_back(PlayableNote(16, noteDuration/2, 0));
  result.push_back(PlayableNote(15, noteDuration, 0));
  result.push_back(PlayableNote(13, noteDuration, 0));

  result.push_back(PlayableNote(17, noteDuration/2, 0));
  result.push_back(PlayableNote(18, noteDuration/2, 0));
  result.push_back(PlayableNote(17, noteDuration/2, 0));
  result.push_back(PlayableNote(16, noteDuration/2, 0));
  result.push_back(PlayableNote(15, noteDuration, 0));
  result.push_back(PlayableNote(13, noteDuration, 0));

  // Bim Bam Bom
  result.push_back(PlayableNote(13, noteDuration, 0));
  result.push_back(PlayableNote(10, noteDuration, 0));
  result.push_back(PlayableNote(13, noteDuration*2, 0));

  result.push_back(PlayableNote(13, noteDuration, 0));
  result.push_back(PlayableNote(10, noteDuration*2, 0));
  result.push_back(PlayableNote(13, noteDuration*4, 0));

  return result;

  /**
    18 is de g van de G sleutel
    14 is de onderste c van de g sleutel.
    9 is de f van de f sleutel balk.

    */
}

//int main(int argc, char *argv[])
//{
//  Player* musicPlayer;
//
//  musicPlayer = new SimplePlayer();
//
//  // Generate PlayableNotes vector.
//  vector<PlayableNote> song = generateExampleNotes();
//
//  cout<<"CAUTION! This program plays sound!"<<endl;
////  system("pause");
//
//
//  musicPlayer->playNotes(song);
//
//
//
//  delete musicPlayer;
//
//  return 0;
//}
//
//

