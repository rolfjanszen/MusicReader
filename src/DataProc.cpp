/*
 * DataProc.cpp
 *
 *  Created on: 30 mei 2016
 *      Author: rjanszen
 */

#include "DataProc.h"
//
//template<class T> DataProc<T>::DataProc() {
//	// TODO Auto-generated constructor stub
//
//}
//
//template<class T> DataProc<T>::~DataProc() {
//	// TODO Auto-generated destructor stub
//}




template<class T> vector<int> DataProc<T>::SmoothVector(vector<int> input, int smooth_factor)
{
    vector<int> output(input.size()-smooth_factor);
    int sum_inp=0;

    for(int i=0;i<input.size()-smooth_factor;i++)
    {
        sum_inp=0;
        for(int j=0;j<smooth_factor;j++)
           sum_inp += input[i+j];

         output[i] = sum_inp/smooth_factor;
    }

    return output;
}




template<class T> bool DataProc<T>::CheckCross( Point2i M1,Point2i M2, Point2i A, Point2i B)
{
    int resultA = (M1.x-M2.x)*(A.y-M1.y)-(M1.y-M2.y)*(A.x-M1.x);

     int resultB = (M1.x-M2.x)*(B.y-M1.y)-(M1.y-M2.y)*(B.x-M1.x);

if(B.y>30)
    return false;

     cout<<"resultA "<<resultA<<" resultB "<<resultB<<"  resul: "<<signbit(resultA)<<"  "<<signbit(resultB)<<endl;
     if(signbit(resultA) == signbit(resultB))
         return true;

     //check gradient follow.
     int Y_sgn_AB=M2.y-M1.y;

     int Y_sgn_BM2=B.y-M2.y;


     cout<<" Y_sgn_AB "<<Y_sgn_AB<<"   Y_sgn_BM2 "<<Y_sgn_BM2<<endl;


     if(signbit(Y_sgn_AB)== signbit(Y_sgn_BM2))
         return true;

     return false;
}

