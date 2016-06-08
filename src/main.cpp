#include "analysescore.h"
#include "../SimplePlayer/SimplePlayer.h"



vector<PlayableNote> generateExampleNotes()
{
  vector<PlayableNote> result;
  int noteDuration  = 300;

  // Vader Jacob.
  result.push_back(PlayableNote(40, noteDuration, 0));
  result.push_back(PlayableNote(42, noteDuration, 0));
  result.push_back(PlayableNote(44, noteDuration, 0));
  result.push_back(PlayableNote(40, noteDuration, 0));

  result.push_back(PlayableNote(40, noteDuration, 0));
  result.push_back(PlayableNote(42, noteDuration, 0));
  result.push_back(PlayableNote(44, noteDuration, 0));
  result.push_back(PlayableNote(40, noteDuration, 0));

  // Slaapt gij nog?
  result.push_back(PlayableNote(44, noteDuration, 0));
  result.push_back(PlayableNote(45, noteDuration, 0));
  result.push_back(PlayableNote(47, noteDuration * 2, 0));


  result.push_back(PlayableNote(44, noteDuration, 0));
  result.push_back(PlayableNote(45, noteDuration, 0));
  result.push_back(PlayableNote(47, noteDuration * 2, 0));

  // Alle Klokken luiden
  result.push_back(PlayableNote(47, noteDuration/2, 0));
  result.push_back(PlayableNote(49, noteDuration/2, 0));
  result.push_back(PlayableNote(47, noteDuration/2, 0));
  result.push_back(PlayableNote(45, noteDuration/2, 0));
  result.push_back(PlayableNote(44, noteDuration, 0));
  result.push_back(PlayableNote(40, noteDuration, 0));

  result.push_back(PlayableNote(47, noteDuration/2, 0));
  result.push_back(PlayableNote(49, noteDuration/2, 0));
  result.push_back(PlayableNote(47, noteDuration/2, 0));
  result.push_back(PlayableNote(45, noteDuration/2, 0));
  result.push_back(PlayableNote(44, noteDuration, 0));
  result.push_back(PlayableNote(40, noteDuration, 0));

  return result;

  /**
    18 is de g van de G sleutel
    14 is de onderste c van de g sleutel.
    9 is de f van de f sleutel balk.

    */
}

int main(int argc, char *argv[])
{
	AnalyseScore analyze;

	Mat sheetmusic=imread("C:\\Users\\rjanszen\\Documents\\vaderjacob.png",0);

   vector<Bar> playbar = analyze.Run(sheetmusic);

//	vector<PlayableNote> play =generateExampleNotes();
   Player* musicPlayer;

   musicPlayer = new SimplePlayer();

//   musicPlayer->playNotes(play);
   // Generate PlayableNotes vector.
//   vector<PlayableNote> song = generateExampleNotes();

   for(Bar& b : playbar)
	   musicPlayer->playNotes(b.playableNotes);

   delete musicPlayer;

    return 0;
}
