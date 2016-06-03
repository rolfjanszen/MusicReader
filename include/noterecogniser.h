/*
 * noterecogniser.h
 *
 *  Created on: 1 jun. 2016
 *      Author: rjanszen
 */


#ifndef NOTERECOGNISER_H_
#define NOTERECOGNISER_H_


#include <dirent.h>
#include "opencv/ml.h"
#include "essential.h"


class NoteRecogniser {
public:
	NoteRecogniser();
	virtual ~NoteRecogniser();
	KNearest TrainKNN();

	float EvalData(Mat image,RotatedRect ellipse);
	void SaveLearnImage(Mat image, RotatedRect contour, string folder, string name);
private :
	Mat TrainData, TrainClassifier;

	int K;
	int trainImgDimension;
	int dilation_size;

	Mat PreProcessData(const Mat &image,RotatedRect ellipse);
	Mat GetDataVec(string folder, vector<string> names, int dimension);
	vector<string> GetFileNames(string directory);
	KNearest knn;
};

#endif /* NOTERECOGNISER_H_ */
