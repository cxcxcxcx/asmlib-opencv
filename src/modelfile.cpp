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

#include "modelfile.h"

namespace StatModel {

void ModelFile::writePCA(const PCA* p)
{
    int i, j;
    int rows, cols;
    rows = p->eigenvectors.rows;
    cols = p->eigenvectors.cols;
    writeInt(rows);
    writeInt(cols);
    for (i=0;i<rows;i++)
        for (j=0;j<cols;j++)
            writeReal(p->eigenvectors.at<double>(i, j));
    for (i=0;i<rows;i++)
        writeReal(p->eigenvalues.at<double>(i, 0));
    
    for (i=0;i<cols;i++)
        writeReal(p->mean.at<double>(i, 0));
}

PCA * ModelFile::readPCA(PCA * &p)
{
    int i, j;
    p = new PCA();
    int rows, cols;
    readInt(rows);
    readInt(cols);
    p->eigenvectors = Mat_<double>::zeros(rows, cols);
    for (i=0;i<rows;i++)
        for (j=0;j<cols;j++)
            readReal(p->eigenvectors.at<double>(i, j));
    p->eigenvalues = Mat_<double>::zeros(rows, 1);
    for (i=0;i<rows;i++)
        readReal(p->eigenvalues.at<double>(i, 0));

    p->mean = Mat_<double>::zeros(cols, 1);
    for (i=0;i<cols;i++){
        readReal(p->mean.at<double>(i, 0));
    }
    return p;
}

} // Namespace
