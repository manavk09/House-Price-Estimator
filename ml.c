/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);

// main method starts here
int main(int argc, char** argv){
    // your code goes here
    double** x;
    double** y;
    int rowA;
    int col;
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        return 0;
    }
    fscanf(fp, "%d \n", &col);//col is counting starting at 0;
    fscanf(fp, "%d \n", &rowA);//row is counting starting at 1;
    col += 1;
    y = malloc(rowA* sizeof(double*));
    
    for(int i = 0; i < rowA; i++){
        y[i] = malloc(sizeof(double));
    }
    
    x = malloc(rowA* sizeof(double*));
    for(int i = 0; i< rowA; i++){
        x[i] = (double*)malloc(col*sizeof(double));
    }

    //set first row to 1
    for(int i = 0; i<rowA;i++){
        x[i][0] = 1.0;
    }
    //now lets load up the train data
    for(int r = 0; r < rowA; r++){
        for(int c = 1; c < col; c++){
         fscanf(fp,"%lf,", &x[r][c]);

        }
        fscanf(fp,"%lf \n", &y[r][0] );
    }
    fclose(fp);
    
    //now read the test file
    double** matB;
    int rowB; 
    FILE *fp1 = fopen(argv[2], "r");
    if(fp1 == NULL){
        return 0;
    }  
    fscanf(fp1, "%d \n", &rowB);
    //create the memory
    matB = (double**)malloc(rowB* sizeof(double*));
    for(int i = 0; i < rowB; i++){
        matB[i] = (double*)malloc((col)*sizeof(double));
    }
    //now load the data up;
    for(int r = 0; r < rowB; r++){
        for(int c = 0; c < col; c++){
            if(c==0){
                matB[r][c] = 1.0;
            }
            else{
            fscanf(fp1,"%lf,", &matB[r][c]);}
        }
    }
    //close 
    fclose(fp1);

    //now get all the values for W
    double** xTran = transposeMatrix(x,rowA,col);
    double **xtMULTx = multiplyMatrix(xTran,x,col, rowA, rowA, col);
    double **inverse = inverseMatrix(xtMULTx, col);
    double **inverseMultXt = multiplyMatrix(inverse,xTran,col,col, col,rowA);
    double **W = multiplyMatrix(inverseMultXt,y, col,rowA, rowA, 1);
    //final price
    double **finalPrice = multiplyMatrix(matB,W,rowB,col,col,1);
    //to print the prices
    for(int i = 0; i < rowB; i++){
        printf("%0.0lf \n", finalPrice[i][0]);
    }
    
    /*
    the next segement is just freeing the dynamically allocated memory
    in the opposite order they are all created.
    */

    for(int i =0; i < rowB; i++){
        free(finalPrice[i]);
    }
    free(finalPrice);

    for(int i = 0; i < col; i++){
        free(W[i]);
    }
    free(W);
    
    for(int i = 0; i < col; i++){
        free(inverseMultXt[i]);
    }
    free(inverseMultXt);

    for(int i = 0; i < col; i++){
        free(inverse[i]);
    }
    free(inverse);

    for(int i = 0; i < col; i++){
        free(xtMULTx[i]);
    }
    free(xtMULTx);

    for(int i = 0; i < col; i++){
        free(xTran[i]);
    }
    free(xTran);

    for(int i = 0; i < rowB; i++){
        free(matB[i]);
    }
    free(matB);
    
    for(int i = 0; i < rowA; i++){
        free(y[i]);
    }
    free(y);

        for(int i = 0; i < rowA; i++){
        free(x[i]);
    }
    free(x);

    
    return 0;
 
}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
    //base case if col and row are not equal we cant multiply
    if(c1 != r2){
        return NULL;
    }
    double** result=malloc(r1*sizeof(double*)); 
    //allocate memeory for the rows
    for(int i = 0; i < r1; i++){
        result[i] = malloc(c2*sizeof(double));
    }
    //lets multiply
    for(int r = 0; r < r1; r++){
        for(int c = 0; c < c2; c++){
            //first set them to 0 then change after
            result[r][c] = 0;
            //use this to traverse the columns of matA 1 by 1 and the rows of matB 1 by 1
            for(int i = 0; i < c1; i++){
                //dot product
                result[r][c] += matA[r][i] * matB[i][c];
            }
        }
    }        
    return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
    //printf("%d", mat[0][0]);
	double** matTran=malloc(col*sizeof(double*)); 

    //allocate memory for the rows
    for(int i = 0; i < col; i++){
        matTran[i] = malloc(row*sizeof(double));
    }
    //now lets make the change of cols to rows and rows to cols
    for(int r = 0; r < row; r++){
        for(int c = 0; c < col; c++){
            matTran[c][r] = mat[r][c];
        } 
    }   
    return matTran;        
}

double** inverseMatrix(double **matA, int dimension)
{
    
    double** matI=malloc(dimension*sizeof(double*)); 

    //allocate memory for rows
    for(int i = 0; i < dimension; i++){
        matI[i] = malloc(dimension*sizeof(double));
    }
    //set up the identity matrix with correct values (making the diagnoal all 1)
    for(int r = 0; r < dimension; r++){
        for(int c = 0; c < dimension; c++){
            if(r == c){
                matI[r][c] = 1;
            }
            else{
                matI[r][c] = 0;
            }
        }
    }

    for(int p = 0; p < dimension; p++)
    {
        double constant = matA[p][p];
        //now go through the entire column in identity and reg mat and divide it by the constant
        for(int c = 0; c < dimension; c++)
        {
            matA[p][c] /= constant;
            matI[p][c] /= constant;
        }
        //now lets subtract the bottom and make them zero
        for(int r = p+1; r < dimension; r++)
        {
            double f = matA[r][p];
            for(int i = 0; i < dimension; i++)
            {
                matA[r][i] -= (matA[p][i] * f); 
                matI[r][i] -= (matI[p][i]* f); 
            }
        }
    }
    // now for the top
    // do the same thing as above but start from bottom
    for(int p = dimension - 1; p > -1; p--){
        for(int i = p-1; i > -1; i--){
            double f = matA[i][p];
            for(int k = 0; k < dimension; k++){
                matA[i][k] -= (matA[p][k] * f);
                matI[i][k] -= (matI[p][k] * f);
            }
        }
    }
	return matI;
}