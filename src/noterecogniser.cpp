/*
 * noterecogniser.cpp
 *
 *  Created on: 1 jun. 2016
 *      Author: rjanszen
 */

#include "noterecogniser.h"

NoteRecogniser::NoteRecogniser() {
	// TODO Auto-generated constructor stub
	trainImgDimension=32;
	dilation_size =0;
//	K = 10;

	dataLocation = "traindata\\collecteded_data\\";
	dataDimension = trainImgDimension*trainImgDimension;
}

NoteRecogniser::~NoteRecogniser() {
	// TODO Auto-generated destructor stub
}

Mat CreateVec(Mat input, int length)
{
	Mat output = Mat(1, length, CV_8UC1);
	int colsize = input.cols;

	for(int i=0;i<colsize; i++)
	{
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

	for(unsigned int i=2;i < names.size();i++)
	{

		Mat goodimg = imread(folder+names[i],0);

		Mat description = GetDescriptor(goodimg);
		cout<<"description "<<description<<endl;
//		Mat data=CreateVec(description,dimension);
		description.copyTo(Data.rowRange(i,i+1));
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

	Mat procesed = GetROI(image,contour);
	imwrite(dataLocation +name,procesed);

}

Mat NoteRecogniser::GetDescriptor(Mat image)
{

	if(image.cols != trainImgDimension)
	resize(image,image,Size(trainImgDimension,trainImgDimension),0,0,INTER_NEAREST);

	int elementSize = 1;
	Mat element = getStructuringElement( MORPH_ELLIPSE,
											Size( 2*dilation_size + 1, 2*dilation_size+1 ),
											Point( dilation_size, dilation_size ) );

	 Mat erosion_dst;
//	         resize(trainImage,resizedImage,Size(trainImgDimension,trainImgDimension),0,0,INTER_NEAREST);
	 dilate( image, image, element );
	 erode( image, erosion_dst, element );


//	HOGDescriptor  Hog( Size(32,32), Size(16,16), Size(8,8), Size(8,8), 9);
//	vector<float> descriptors;
//	vector<Point>locs;
//
//	Hog.compute(erosion_dst, descriptors);
//	cout<<"fl "<<descriptors.size()<<endl;
//	for(float &fl : descriptors)
//		cout<<fl<<" ";
//	Mat hogVector = Mat(1,descriptors.size(),CV_32FC1);
//	 memcpy(hogVector.data,descriptors.data(),descriptors.size()*sizeof(float));
//	 cout<<"hogVector "<<hogVector<<endl;
//	 hogVector*=100;

	return  CreateVec(erosion_dst , trainImgDimension*trainImgDimension);//hogVector.clone();

}

Mat NoteRecogniser::GetROI(const Mat &image,RotatedRect ellipse)
{
//	int margin = 0.2;
	int cutsize;

	if(ellipse.size.width > ellipse.size.height)
		cutsize = (int)ellipse.size.width;
	else
		cutsize = (int) ellipse.size.height;

	int margin =cutsize*0.4;
	int leftCol  = (int)ellipse.center.x - cutsize/2 - margin;
	int rightCol = (int)ellipse.center.x + cutsize/2 + margin;
	int topRow = (int)ellipse.center.y - cutsize/2 - margin;
	int bottomRow  =(int) ellipse.center.y + cutsize/2 + margin;

	Mat processedImage;
	cout<<"GetROI"<<endl;

	if(leftCol > 0  && rightCol < image.cols && topRow >0 && bottomRow <image.rows)
	{
		Mat trainImage = image.rowRange(topRow, bottomRow ).colRange(leftCol, rightCol);

		resize(trainImage,processedImage,Size(trainImgDimension,trainImgDimension),0,0,INTER_NEAREST);

	}
	else
		processedImage = Mat::ones(trainImgDimension,trainImgDimension,CV_8UC1);

//	cout<<"done GetROI"<<processedImage<<endl;

	return processedImage.clone();
}

void NoteRecogniser::ConcatData(const string datafolder, float id,	int nrClasses ){

	vector<string> names = GetFileNames(datafolder);
	Mat goodimg = imread(datafolder+names[2],0);

	int dimension = dataDimension;//trainImgDimension*trainImgDimension;
	Mat data= GetDataVec(datafolder,names, dimension);

	Mat new_classifier;// = Mat(data.rows,1,CV_32FC1,id);

	if(learnMethod == ml_SVM){
		new_classifier = Mat(data.rows,1,CV_32FC1,id);
	}
	else if(learnMethod == ml_MLP){
		new_classifier = Mat::zeros(data.rows,nrClasses,CV_32FC1);

		for(int i=0;i<data.rows;i++)
		{
			new_classifier.at<float>(i,id-1)=1;
		}

	}

	if(TrainData.empty())
	{
		data.copyTo(TrainData);
		new_classifier.copyTo(TrainClassifier);
	}
	else
	{
		vconcat(TrainData,data.clone(), TrainData);
		vconcat(TrainClassifier,new_classifier.clone(), TrainClassifier);
	}
}


void NoteRecogniser::InitTrainData()
{
	nrClasses = 4;
	string goodDataLoc = "traindata\\note\\";
	string badDataLoc = "traindata\\notnote\\";
	string rest = "traindata\\rest\\";
	string quarter = "traindata\\quarter\\";

	string nameArr[nrClasses]={goodDataLoc,badDataLoc,rest,quarter};

//	  std::cout << "A number of elements in array is: "
//	              << sizeof(nameArr)/sizeof(nameArr[0]) << '\n';

	for(int i=0; i < nrClasses;	i++)
	{
		ConcatData( nameArr[i], 1+i,nrClasses);
	}
}


void NoteRecogniser::Train(const mlMethod &method)
{
	learnMethod = method;
	InitTrainData();

	if(method == ml_SVM)
		TrainSVM();
	else if (method == ml_MLP)
		TrainMLP();

}

void NoteRecogniser::TrainSVM()
{

	TrainData.convertTo(TrainData,CV_32FC1);
//	KNearest newKNN(TrainData, TrainClassifier);
	///////////

   CvSVMParams param;
   param.svm_type = CvSVM::C_SVC;
   param.kernel_type = CvSVM::RBF; //CvSVM::RBF, CvSVM::LINEAR ...
   param.degree = 1; // for poly
   param.gamma = 20; // for poly/rbf/sigmoid
   param.coef0 = 1; // for poly/sigmoid

   param.C = 70; // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
   param.nu = 0.0; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
   param.p = 0.0; // for CV_SVM_EPS_SVR

   param.class_weights = NULL; // for CV_SVM_C_SVC
   param.term_crit.type = CV_TERMCRIT_ITER +CV_TERMCRIT_EPS;
   param.term_crit.max_iter = 1000;
   param.term_crit.epsilon = 1e-6;
	// Train the SVM
//	CvSVM SVM;
//	SVM.train(TrainData, TrainClassifier, Mat(), Mat(), param);
	//////////////////////
	m_svm.train(TrainData, TrainClassifier, Mat(), Mat(), param);
	cout<<"TrainClassifier "<<TrainClassifier<<endl;
//	newKNN.train(TrainData, TrainClassifier);
//	knn=newKNN;
//	cout<<"newKNN "<<newKNN.get_max_k()<<endl;
//	CvMat* nearests = cvCreateMat( 1, K, CV_32FC1);

	for(unsigned int i=0;i<TrainData.rows;i++)
	{
		Mat test = TrainData.rowRange(i,i+1);
//		test.convertTo(test,CV_32FC1);
//		cout<<"test "<<test<<" "<<test.cols" "<<test.rows<<endl;
//		float response = newKNN.find_nearest(test,K);
		 float response = m_svm.predict(test);
		cout<<"response "<<response<<endl;
	}
}



void NoteRecogniser::TrainMLP()
{
//	TrainClassifier =TrainClassifier/2;
//	TrainData=TrainData/255;
//	InitTrainData();
//	int K = 10;
	cout<<"TrainClassifier "<<TrainClassifier<<endl;

	TrainData.convertTo(TrainData,CV_32FC1);
//	KNearest newKNN(TrainData, TrainClassifier);
	///////////
    cv::Mat layers = cv::Mat(6, 1, CV_32SC1);

    layers.row(0) = cv::Scalar(dataDimension);
    layers.row(1) = cv::Scalar((int)dataDimension*0.8);
    layers.row(2) = cv::Scalar((int)dataDimension*0.6);
    layers.row(3) = cv::Scalar((int)dataDimension*0.4);
    layers.row(4) = cv::Scalar((int)dataDimension*0.4);
    layers.row(5) = cv::Scalar(nrClasses);

    CvANN_MLP_TrainParams params;
    CvTermCriteria criteria;
    criteria.max_iter = 600;
    criteria.epsilon = 0.0000001f;
    criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
    params.train_method = CvANN_MLP_TrainParams::BACKPROP;
    params.bp_dw_scale = 0.05f;
    params.bp_moment_scale = 0.05f;
    params.term_crit = criteria;

    m_mlp.create(layers);

    // train
    m_mlp.train(TrainData, TrainClassifier, cv::Mat(), cv::Mat(), params);

    cv::Mat response(1, 1, CV_32FC1);
    cv::Mat predicted(TrainClassifier.rows, 1, CV_32F);
    for(int i = 0; i < TrainData.rows; i++)
    {
        cv::Mat response(1, 1, CV_32FC1);
        cv::Mat sample = TrainData.row(i);

        m_mlp.predict(sample, response);
        predicted.at<float>(i,0) = response.at<float>(0,0);
        cout<<"response "<<response<<endl;
    }

}


float NoteRecogniser::EvalData(Mat image,RotatedRect ellipse)
{
	imshow("image",image);

	Mat testImage = GetROI( image, ellipse);

	Mat data = GetDescriptor(testImage);
	cout<<"here postproc "<<endl;
	imshow("testImage",testImage);

//	Mat data=CreateVec(description, dimension);

	data.convertTo(data,CV_32FC1);
	float  response=0;

	if(learnMethod == ml_SVM){
		response = m_svm.predict(data);
	}
	else if(learnMethod == ml_MLP)
	{
		Mat result;
		m_mlp.predict(data,result);
		cout<<"result "<<result<<endl;
		float highest=-100;

		for(int i=0;i<result.cols;i++){
			if(result.at<float>(0,i) > highest){
				response = 1+i;
				highest =result.at<float>(0,i) ;
			}
		}
	}

	cout<<"response "<<response<<endl;
	return response;

}
