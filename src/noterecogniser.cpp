/*
 * noterecogniser.cpp
 *
 *  Created on: 1 jun. 2016
 *      Author: rjanszen
 */

#include "noterecogniser.h"

NoteRecogniser::NoteRecogniser() {
	// TODO Auto-generated constructor stub
	trainImgDimension=20;
	dilation_size =1;
	K = 10;

	dataLocation = "C:\\Users\\rjanszen\\workspace\\Readmusic\\traindata\\collecteded_data\\";

}

NoteRecogniser::~NoteRecogniser() {
	// TODO Auto-generated destructor stub
}

Mat CreateVec(Mat input, int length)
{
	Mat output = Mat(1, length, CV_8UC1);
	int colsize = input.cols;
	for(int i=0;i<colsize; i++){
		Mat newImg = input.rowRange(i,i+1);
		newImg.copyTo(output.colRange(colsize*i,colsize*(i+1)));

	}
//	cout<<"output"<<output<<endl;
	return output.clone();
}

Mat NoteRecogniser::GetDataVec(string folder, vector<string> names, int dimension)
{
	Mat Data(names.size(),dimension,CV_8UC1);
//	Mat badData(dimension, names.size(),CV_8UC1);

	for(int i=2;i < names.size();i++)
	{

		Mat goodimg = imread(folder+names[i],0);

		Mat data=CreateVec(goodimg,dimension);
		data.copyTo(Data.rowRange(i,i+1));
//		cout<<"learn data "<<Data.rowRange(i,i+1)<<endl;

	}
	return Data;
}

vector<string> NoteRecogniser::GetFileNames(string directory)
{

	DIR *dir;
	struct dirent *ent;
	vector<string> filenames;

	if ((dir = opendir(directory.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			printf("%s\n", ent->d_name);
			filenames.push_back(ent->d_name);
		}
		closedir(dir);
	} else {
		/* could not open directory */
		perror("");

	}

	return filenames;
}

void NoteRecogniser::SaveLearnImage(Mat image, RotatedRect contour, string name){


	Mat processedImg = PreProcessData(image,contour);
	imwrite(dataLocation +name,processedImg);

}
Mat NoteRecogniser::PreProcessData(const Mat &image,RotatedRect ellipse)
{
	int margin = 2;
	int cutsize;

	if(ellipse.size.width > ellipse.size.height)
		cutsize = (int)ellipse.size.width;
	else
		cutsize = (int) ellipse.size.height;

	int leftCol  = (int)ellipse.center.x - cutsize/2 - margin;
	int rightCol = (int)ellipse.center.x + cutsize/2 + margin;
	int topRow = (int)ellipse.center.y - cutsize/2 - margin;
	int bottomRow  =(int) ellipse.center.y + cutsize/2 + margin;
	Mat dilation_dst,erosion_dst;
	cout<<"PreProcessData"<<endl;

	if(leftCol > 0  && rightCol < image.cols && topRow >0 && bottomRow <image.rows)
	{
		Mat trainImage = image.rowRange(topRow, bottomRow ).colRange(leftCol, rightCol);

		Mat element = getStructuringElement( MORPH_ELLIPSE,
											   Size( 2*dilation_size + 1, 2*dilation_size+1 ),
											   Point( dilation_size, dilation_size ) );

		Mat resizedImage;
		resize(trainImage,resizedImage,Size(trainImgDimension,trainImgDimension),0,0,INTER_NEAREST);

		dilate( resizedImage, dilation_dst, element );
		erode( dilation_dst, erosion_dst, element );
	}
	else
		erosion_dst = Mat::ones(trainImgDimension,trainImgDimension,CV_8UC1);

	cout<<"done PreProcessData"<<endl;

	return erosion_dst.clone();
}

void NoteRecogniser::ConcatData(Mat &trainData, Mat &classifier, const string datafolder, float id){

	vector<string> names = GetFileNames(datafolder);
	Mat goodimg = imread(datafolder+names[2],0);
	int dimension = trainImgDimension*trainImgDimension;
	Mat data= GetDataVec(datafolder,names, dimension);

	Mat new_classifier = Mat(data.rows,1,CV_32FC1,id);
	if(trainData.empty())
	{
		data.copyTo(trainData);
		new_classifier.copyTo(classifier);
	}
	else
	{
		vconcat(trainData,data.clone(), trainData);
		vconcat(classifier,new_classifier.clone(), classifier);
	}
}



KNearest NoteRecogniser::Train()
{
	int nrClasses = 3;
	string goodDataLoc = "C:\\Users\\rjanszen\\workspace\\Readmusic\\traindata\\note\\";
	string badDataLoc = "C:\\Users\\rjanszen\\workspace\\Readmusic\\traindata\\notnote\\";
	string quarter = "C:\\Users\\rjanszen\\workspace\\Readmusic\\traindata\\quarter\\";

	string nameArr[nrClasses]={goodDataLoc,badDataLoc,quarter};
//
//	for( int i=0;i<nrClasses;i++)
//	{
//		ConcatData(Mat &trainData, Mat &classifier, const string datafolder, float id)
//	}


	vector<string> goodNames = GetFileNames(goodDataLoc);
	vector<string> badNames = GetFileNames(badDataLoc);
//	vector<string> badNames = GetFileNames(badDataLoc);

//	Mat goodimg = imread(goodDataLoc+goodNames[2],0);
//	Mat badimg = imread(goodDataLoc+badNames[2],0);

//	assert(goodimg.cols == badimg.cols && goodimg.rows == badimg.rows && goodimg.cols == goodimg.rows);
	int dimension = trainImgDimension*trainImgDimension;

//	Mat goodData(dimension, goodNames.size(),CV_8UC1);
//	Mat badData(dimension, badNames.size(),CV_8UC1);

	Mat goodData= GetDataVec(goodDataLoc,goodNames, dimension);
	Mat badData= GetDataVec(badDataLoc,badNames, dimension);

	Mat goodclass = Mat::ones(goodData.rows,1,CV_32FC1);
	Mat badclass = Mat(badData.rows,1,CV_32FC1,2);

	Mat trainImg, trainClass;
	vconcat(goodData,badData, trainImg);
	vconcat(goodclass,badclass, trainClass);
	trainImg.copyTo(TrainData);
	trainClass.copyTo(TrainClassifier);

	int K = 10;
	TrainData.convertTo(TrainData,CV_32FC1);
	KNearest newKNN(TrainData, TrainClassifier);
	///////////
   CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	// Train the SVM
	CvSVM SVM;
	SVM.train(TrainData, TrainClassifier, Mat(), Mat(), params);
	//////////////////////
	m_svm.train(TrainData, TrainClassifier, Mat(), Mat(), params);;
	cout<<"TrainClassifier "<<TrainClassifier<<endl;
//	newKNN.train(TrainData, TrainClassifier);
//	knn=newKNN;
//	cout<<"newKNN "<<newKNN.get_max_k()<<endl;
	CvMat* nearests = cvCreateMat( 1, K, CV_32FC1);

	for(unsigned int i=0;i<TrainData.rows;i++)
	{
		Mat test = TrainData.rowRange(i,i+1);
//		test.convertTo(test,CV_32FC1);
//		cout<<"test "<<test<<" "<<test.cols" "<<test.rows<<endl;
//		float response = newKNN.find_nearest(test,K);
		 float response = SVM.predict(test);
		cout<<"response "<<response<<endl;
	}

	return newKNN;
}


float NoteRecogniser::EvalData(Mat image,RotatedRect ellipse)
{
	imshow("image",image);


	Mat testImage = PreProcessData( image, ellipse);

	cout<<"here postproc "<<endl;
	imshow("testImage",testImage);


	int dimension = trainImgDimension*trainImgDimension;
	Mat data=CreateVec(testImage, dimension);

	data.convertTo(data,CV_32FC1);


	float  response = m_svm.predict(data);

	cout<<"response "<<response<<endl;
	return response;

}
