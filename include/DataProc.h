/*
 * DataProc.h
 *
 *  Created on: 30 mei 2016
 *      Author: rjanszen
 */
#include "essential.h"

#include <dirent.h>
#ifndef DATAPROC_H_
#define DATAPROC_H_

template<class T> class DataProc
{

public:
	DataProc(){};
	virtual ~DataProc(){};
//	template<typename T>


//	template<class T> vector<string> DataProc<T>::GetFileNames(string directory)
//	vector<string> GetFileNames(string directory)
//	{
//
//		DIR *dir;
//		struct dirent *ent;
//		vector<string> filenames;
//
//		if ((dir = opendir(directory.c_str())) != NULL) {
//			/* print all the files and directories within directory */
//			while ((ent = readdir(dir)) != NULL) {
//				printf("%s\n", ent->d_name);
//				filenames.push_back(ent->d_name);
//			}
//			closedir(dir);
//		} else {
//			/* could not open directory */
//			perror("");
//
//		}
//
//		return filenames;
//	}
	 T CalcMedian(const vector<T> &data_set)
	 {
	    int median_=0;
	    vector<T> occurences;

	   occurences.resize(data_set.size(),0);

	   for(int i=0;i<data_set.size();i++)
	   {
	       occurences[data_set[i]]++;
	   }

	   int most_occuring=0;

	   for(int i=0;i<occurences.size();i++)
	   {
			if(occurences[i]>most_occuring)
			{
				most_occuring =  occurences[i];
				median_=i;
			}
	   }

	   cout<<"median_ "<<median_<<endl;
	   return median_;

	}

	bool CheckCross( Point2i M1,Point2i M2, Point2i A, Point2i B);
	vector<int> SmoothVector(vector<int> input, int smooth_factor);

};

#endif /* DATAPROC_H_ */
