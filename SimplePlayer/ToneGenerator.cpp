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
  _beeps.clear();
  for(const PlayableNote& pn : notes)
  {
    double frequency = pianoKeyFrequencies(pn.note_ID);
    int duration = 500;
    if(pn.duration > 200)
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
