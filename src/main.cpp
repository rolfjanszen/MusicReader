#include "analysescore.h"
#include "../SimplePlayer/SimplePlayer.h"

int main(int argc, char *argv[])
{
	AnalyseScore analyze;

	Mat sheetmusic=imread("C:\\Users\\rjanszen\\Documents\\marylamb.jpg",0);

   vector<Bar> playbar = analyze.Run(sheetmusic);

   Player* musicPlayer;

   musicPlayer = new SimplePlayer();

   // Generate PlayableNotes vector.
//   vector<PlayableNote> song = generateExampleNotes();

   for(Bar& b : playbar)
	   musicPlayer->playNotes(b.playableNotes);

   delete musicPlayer;

    return 0;
}
