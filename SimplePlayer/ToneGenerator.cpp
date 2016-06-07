#include "ToneGenerator.h"

#include <math.h>
#include <windows.h>

ToneGenerator::ToneGenerator() :
  _beeps()
{
}

double pianoKeyFrequencies(int note)
{
  double a4Key = 440.0;
  double powerTo = (note-49)/12.0;
  double result = (pow(2.0, powerTo) * a4Key);
  return result;
}


void ToneGenerator::readNotes(const vector<PlayableNote>& notes)
{
  int noteToPianoKey[27];

  noteToPianoKey[6] = 28;  // C3
  noteToPianoKey[7] = 30;  // D3
  noteToPianoKey[8] = 32;  // E3
  noteToPianoKey[9] = 33;  // F3
  noteToPianoKey[10] = 35; // G3
  noteToPianoKey[11] = 37; // A3
  noteToPianoKey[12] = 39; // B3

  int n = 13; // number of middel c

  noteToPianoKey[n++] = 40; // Middle C4
  noteToPianoKey[n++] = 42; // D4
  noteToPianoKey[n++] = 44; // E4
  noteToPianoKey[n++] = 45; // F4
  noteToPianoKey[n++] = 47; // G4
  noteToPianoKey[n++] = 49; // A4
  noteToPianoKey[n++] = 51; // B4

  noteToPianoKey[n++] = 52; // C5
  noteToPianoKey[n++] = 54; // D5
  noteToPianoKey[n++] = 56; // E5
  noteToPianoKey[n++] = 57; // F5
  noteToPianoKey[n++] = 59; // G5
  noteToPianoKey[n++] = 61; // A5
  noteToPianoKey[n++] = 63; // B5

  _beeps.clear();
  for(const PlayableNote& pn : notes)
  {
    int pianoKey = noteToPianoKey[pn.note_ID];
    double frequency = pianoKeyFrequencies(pianoKey);
    int duration = 500;
    if(pn.duration > 0)
    {
       duration = pn.duration;
    }
    _beeps.push_back( BeepStruct(frequency, duration) );
  }

}


void ToneGenerator::play() const
{
  for(const BeepStruct& b : _beeps)
  {
    Beep(b.frequency, b.duration);
  }
}



// We call A4 note = 49.



void ToneGenerator::vaderJacob()
{
  double noteC =  pianoKeyFrequencies(40);
  double noteD =  pianoKeyFrequencies(42);
  double noteE =  pianoKeyFrequencies(44);
  double noteF =  pianoKeyFrequencies(45);
  double noteG =  pianoKeyFrequencies(47);
  double noteA =  pianoKeyFrequencies(49);

  _beeps.clear();


  _beeps.push_back(BeepStruct(noteC, 500));
  _beeps.push_back(BeepStruct(noteD, 500));
  _beeps.push_back(BeepStruct(noteE, 500));
  _beeps.push_back(BeepStruct(noteC, 500));

  _beeps.push_back(BeepStruct(noteC, 500));
  _beeps.push_back(BeepStruct(noteD, 500));
  _beeps.push_back(BeepStruct(noteE, 500));
  _beeps.push_back(BeepStruct(noteC, 500));

  _beeps.push_back(BeepStruct(noteF, 500));
  _beeps.push_back(BeepStruct(noteG, 500));
  _beeps.push_back(BeepStruct(noteA, 800));

  _beeps.push_back(BeepStruct(noteF, 500));
  _beeps.push_back(BeepStruct(noteG, 500));
  _beeps.push_back(BeepStruct(noteA, 800));

  play();
}
