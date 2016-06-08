#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <vector>

#include "../include/playablenote.h"

using namespace std;


struct BeepStruct
{
  double frequency;
  double duration;

  BeepStruct(double f, double d) :
    frequency(f),
    duration(d)
  {}
};


class ToneGenerator
{
public:
  ToneGenerator();

  void readNotes(const vector<PlayableNote>& notes);

  void play() const;

  void vaderJacob();

private:
  vector<BeepStruct> _beeps;

};

#endif // TONEGENERATOR_H
