using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace MatrixClass
{
    //Matrix class, all of codes in this class are changed from http://blog.csdn.net/jdh99/article/details/7889499
    public class _Matrix
    {
        public int m;
        public int n;
        public float[,] arr;

        public _Matrix(int mm, int nn)
        {
            m = mm;
            n = nn;
            arr = new float[mm,nn];
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    arr[i, j] = 0;
                }
            }
        }

        //read matrix[i,j]
        public float read(int i, int j)
        {
            if (i >= m || j >= n)
            {
                throw new System.Exception("Out of matrix index");
            }
            return arr[i,j];
        }

        //write matrix[i,j]
        public void write(int i, int j, float val)
        {
            if (i >= m || j >= n)
            {
                throw new System.Exception("Out of matrix index");
            }
            arr[i,j] = val;
        }
    };

    class _Matrix_Calc
    {
        //C = A * B     
        public static _Matrix multiply(_Matrix A, _Matrix B)
        {
            float temp = 0;
            _Matrix C = new _Matrix(A.m, B.n);
            if (A.n == B.m)
            { 
                for (int i = 0; i < C.m; i++)
                {
                    for (int j = 0; j < C.n; j++)
                    {
                        temp = 0;
                        for (int k = 0; k < A.n; k++)
                        {
                            temp += A.read(i, k) * B.read(k, j);
                        }
                        C.write(i, j, temp);
                    }
                }
            } else
            {
                throw new System.Exception("Two martixs' row and column not equal");
            }
            return C;
        }

        //calculate the det of a 2*2 matrix
        public static float detTwoTwo(_Matrix A)
        {
            float value = 0;

            //check if the matrix's def can be caculated 
            if (A.m != A.n || (A.m != 2))
            {
                throw new System.Exception("Can't calculate this matrix's def");
            }
 
            value = A.read(0, 0) * A.read(1, 1) - A.read(0, 1) * A.read(1, 0);
            return value;
        }

        //calculate transposition Matrix,B = AT    
        public static _Matrix transpositionMatrix(_Matrix A)
        {
            _Matrix B = new _Matrix(A.n, A.m);   
            for (int i = 0; i < B.m; i++)
            {
                for (int j = 0; j < B.n; j++)
                {
                    B.write(i, j, A.read(j, i));
                }
            }
            return B;
        }

        //this get inverse matrix code is from  https://www.cnblogs.com/renge-blogs/p/6308912.html
        public static _Matrix Inverse(_Matrix matrix)
        {
            int m = matrix.m;
            int n = matrix.n;
            float[,] array = new float[2 * m + 1, 2 * n + 1];
            for (int k = 0; k < 2 * m + 1; k++)  //Initialize
            {
                for (int t = 0; t < 2 * n + 1; t++)
                {
                    array[k, t] = 0f;
                }
            }
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    array[i, j] = matrix.read(i, j);
                }
            }

            for (int k = 0; k < m; k++)
            {
                for (int t = n; t <= 2 * n; t++)
                {
                    if ((t - k) == m)
                    {
                        array[k, t] = 1f;
                    }
                    else
                    {
                        array[k, t] = 0;
                    }
                }
            }
            //get inverse matrix
            for (int k = 0; k < m; k++)
            {
                if (array[k, k] != 1)
                {
                    float bs = array[k, k];
                    array[k, k] = 1;
                    for (int p = k + 1; p < 2 * n; p++)
                    {
                        array[k, p] /= bs;
                    }
                }
                for (int q = 0; q < m; q++)
                {
                    if (q != k)
                    {
                        float bs = array[q, k];
                        for (int p = 0; p < 2 * n; p++)
                        {
                            array[q, p] -= bs * array[k, p];
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            _Matrix inverse = new _Matrix(m,n);
            for (int x = 0; x < m; x++)
            {
                for (int y = n; y < 2 * n; y++)
                {
                    inverse.write(x, y - n, array[x, y]);
                }
            }
            return inverse;
        }
    };
}