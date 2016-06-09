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

enum mlMethod{ml_SVM,ml_MLP};

class NoteRecogniser {
public:
	NoteRecogniser();
	virtual ~NoteRecogniser();



	void Train(const mlMethod &method);

	float EvalData(Mat image,RotatedRect ellipse);
	void SaveLearnImage(Mat image, RotatedRect contour,  string name);

private :
	int nrClasses;
	mlMethod learnMethod;
	Mat TrainData, TrainClassifier;
	int dataDimension;
//	int K;
	int trainImgDimension;
	int dilation_size;
	string dataLocation;

	CvSVM m_svm;
	CvANN_MLP m_mlp;

	Mat GetROI(const Mat &image,RotatedRect ellipse);
	Mat GetDataVec(string folder, vector<string> names, int dimension);
	vector<string> GetFileNames(string directory);
	void ConcatData(const string datafolder, float id, int nrClasses);
	Mat GetDescriptor(Mat image);
	void InitTrainData();

	void TrainSVM();
	void TrainMLP();
//	KNearest knn;

};

#endif /* NOTERECOGNISER_H_ */
