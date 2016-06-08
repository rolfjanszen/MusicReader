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
	void Train();

	float EvalData(Mat image,RotatedRect ellipse);
	void SaveLearnImage(Mat image, RotatedRect contour,  string name);
private :
	Mat TrainData, TrainClassifier;
	int dataDimension;
//	int K;
	int trainImgDimension;
	int dilation_size;
	string dataLocation;
	Mat PreProcessData(const Mat &image,RotatedRect ellipse);
	Mat GetDataVec(string folder, vector<string> names, int dimension);
	vector<string> GetFileNames(string directory);
	void ConcatData(Mat &trainData, Mat &classifier, const string datafolder, float id);
	Mat GetDescriptor(Mat image);
//	KNearest knn;
	CvSVM m_svm;
};

#endif /* NOTERECOGNISER_H_ */
