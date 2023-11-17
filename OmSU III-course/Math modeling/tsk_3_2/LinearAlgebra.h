#ifndef LINEARALGEBRA_H_
#define LINEARALGEBRA_H_

#include <vector>
#include "Matrix.h"

std::vector<double> gauss(const Matrix &linearEquationCoeeficients, const std::vector<double> &freeColumn){
	if(linearEquationCoeeficients.size.rowCount == freeColumn.size() and linearEquationCoeeficients.size.isSimmetrical()){
		Matrix matrix{linearEquationCoeeficients.size.rowCount, linearEquationCoeeficients.size.columnCount+1};
		for(size_t rIndex=0; rIndex<matrix.size.rowCount; ++rIndex){
			for(size_t cIndex=0; cIndex<linearEquationCoeeficients.size.columnCount; ++cIndex){
				matrix[rIndex][cIndex] = linearEquationCoeeficients.at(rIndex, cIndex);
			}
			matrix[rIndex][matrix.size.columnCount-1] = freeColumn[rIndex];
		}
		for(size_t mainIndex=0; mainIndex<matrix.size.rowCount; mainIndex++){
		    if(matrix[mainIndex][mainIndex] == 0.0){
		    	for(size_t rowIndex=mainIndex+1; rowIndex<matrix.size.rowCount; rowIndex++)
		    		if((matrix[mainIndex][rowIndex] > 0.0)||(matrix[mainIndex][rowIndex] < -0.0)){
		    			const double multyplicator = 1.0/matrix[rowIndex][mainIndex];
		                for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; columnIndex++)
		                    matrix[mainIndex][columnIndex] += multyplicator*matrix[rowIndex][columnIndex];
		                break;
		            }
		    }else if(matrix[mainIndex][mainIndex] != 1.0){
		    	const double multyplicator = 1.0/matrix[mainIndex][mainIndex];
		        for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; columnIndex++)
		        	matrix[mainIndex][columnIndex] *= multyplicator;
		        }
		        for(size_t rowIndex=mainIndex+1; rowIndex<matrix.size.rowCount; rowIndex++)
		        	if((matrix[rowIndex][mainIndex] > 0.0)||(matrix[rowIndex][mainIndex] < -0.0)){
		        		const double multyplicator = matrix[rowIndex][mainIndex]/matrix[mainIndex][mainIndex];
		        		for(size_t columnIndex=mainIndex; columnIndex<matrix.size.columnCount; columnIndex++)
		        			matrix[rowIndex][columnIndex] -= multyplicator*matrix[mainIndex][columnIndex];
		                }
		}
		std::vector<double> result(freeColumn.size());
		for(size_t i=0; i<freeColumn.size(); ++i){
			result[i] = matrix[i][matrix.size.columnCount-1];
		}
		return result;
	}else return std::vector<double>{};
}

#endif /* LINEARALGEBRA_H_ */
