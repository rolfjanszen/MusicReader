/*
 * DataProc.h
 *
 *  Created on: 30 mei 2016
 *      Author: rjanszen
 */
#include "essential.h"

#ifndef DATAPROC_H_
#define DATAPROC_H_

template<class T> class DataProc
{

public:
	DataProc(){};
	virtual ~DataProc(){};
//	template<typename T>

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
