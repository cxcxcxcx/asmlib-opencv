/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MODELFILE_H
#define MODELFILE_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include "cv.h"
using cv::PCA;
using cv::Mat_;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;

namespace StatModel {
// class ModelFile
// {
//     public:
//         virtual void writeInt(int i){ fwrite(&i, sizeof(int), 1, fp); }
//         virtual int readInt(int &i) { fread(&i, sizeof(int), 1, fp); return i; }
//
//         virtual void writeBool(bool b){ fwrite(&b, sizeof(bool), 1, fp); }
//         virtual int readBool(bool &b) { fread(&b, sizeof(bool), 1, fp); return b; }
//
//         virtual void writeReal(double d) { fwrite(&d, sizeof(double), 1, fp); }
//         virtual double readReal(double &d) { fread(&d, sizeof(double), 1, fp); return d; }
//
//         void writePCA(const PCA *p);
//         PCA * readPCA(PCA * &p);
//
//         template < class T >
//         virtual void writeMat(const Mat_<T> &m){
//             writeInt(m.rows);
//             writeInt(m.cols);
//             for (int i=0;i<m.rows;i++)
//                 for (int j=0;j<m.cols;j++)
//                     fwrite(&(m(i, j)), sizeof(T), 1, fp);
//         }
//
//         template < class T >
//         Mat_<T> & readMat(Mat_<T> &m){
//             int r,c;
//             readInt(r);
//             readInt(c);
//             m.create(r, c);
//             for (int i=0;i<r;i++)
//                 for (int j=0;j<c;j++)
//                     fread(&(m(i,j)), sizeof(T), 1, fp);
//             return m;
//         }
//
//         void openFile(const char *fName, const char *mode){
//             fp=fopen(fName, mode);
//             if (fp==NULL){
//                 printf("Model file %s not found!!\n", fName);
//                 throw("Model file %s not found!!");
//             }
//         }
//         void closeFile(){ fclose(fp); fp=NULL; }
//
//         ModelFile(){ fp = NULL; }
//         ~ModelFile(){ if (fp!=NULL) fclose(fp); }
//     private:
//         FILE *fp;
// };

//! Save/Load model to/from a file.
class ModelFile
{
    public:
        void writeInt(int i){ fs<<i<<endl; }
        int readInt(int &i) { fs>>i; return i; }

        void writeBool(bool b){ fs<<b<<endl; }
        int readBool(bool &b) { fs>>b; return b; }

        void writeReal(double d) { fs<<d<<endl; }
        double readReal(double &d) { fs>>d; return d; }

        void writePCA(const PCA *p);
        PCA * readPCA(PCA * &p);

        template < class T >
        void writeMat(const Mat_<T> &m){
            writeInt(m.rows);
            writeInt(m.cols);
            for (int i=0;i<m.rows;i++)
                for (int j=0;j<m.cols;j++)
                    fs<<m(i, j)<<endl;
        }


        template < class T >
        Mat_<T> & readMat(Mat_<T> &m){
            int r,c;
            readInt(r);
            readInt(c);
            m.create(r, c);
            for (int i=0;i<r;i++)
                for (int j=0;j<c;j++)
                    fs>>m(i,j);
            return m;
        }

        void openFile(const char *fName, const char *mode){
            if (mode[0]=='r')
                fs.open(fName, std::ios_base::in);
            else
                fs.open(fName, std::ios_base::out);
            if (!fs){
                printf("Model file \"%s\" not found!!\n", fName);
                throw("");
            }
        }
        void closeFile(){ fs.close(); }

        //ModelFile(){  }
        ~ModelFile(){ if (fs) fs.close(); }
    private:
        fstream fs;
};

typedef ModelFile ModelFileAscii;
} // Namespace
#endif // MODELFILE_H
