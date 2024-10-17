#ifndef _MATRIX2_H_
#define _MATRIX2_H_

/*
1、const成员函数的意义是能够作用于const对象。作用于指的是const成员对象只能调用const成员函数，他们不能调用非const成员函数
这个的一个实例就是const Matrix& m，将前面这个作为函数参数，则这个m只能调用const成员函数，比如m[1][2]，如果[]只重载了非const版本，那这里就不能使用如m[1][2]
*/

#include<iostream>
#include<vector>

using std::vector;

class Matrix{
    public:
        Matrix(int n, int m);

        //size()、empty()方法
        size_t size()const{ return matrix.size()-1; }
        bool empty()const{ return (matrix.size()<=1); }

        //重载[]，const和非const两个版本
        vector<int>& operator[](int i);
        const vector<int>& operator[](int i)const;

        //重载+、*
        Matrix operator+(const Matrix& other);

        //矩阵快速幂

        ~Matrix() {}

    private:
        vector<vector<int>> matrix;
};

Matrix::Matrix(int n,int m){
    if(n<=0 || m<=0){
        throw std::invalid_argument("Matrix dimensions must be positive");
    }

    matrix.resize(n+1);
    for(int i=0;i<=n;++i){
        matrix[i]=vector<int>(m+1);
    }
}

vector<int>& Matrix::operator[](int i){
    if(i<0 || i>static_cast<int>(matrix.size())-1){
        throw std::out_of_range("Row index out of range");
    }
    return matrix[i];
}
const vector<int>& Matrix::operator[](int i)const{
    if(i<0 || i>static_cast<int>(matrix.size())-1){
        throw std::out_of_range("Row index out of range");
    }
    return matrix[i];
}

Matrix Matrix::operator+(const Matrix& other){
    if(matrix.size()<=1 || other.size()<=1){
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    if(this->size()!=other.size() || matrix[0].size()!=other[0].size()){
        throw std::invalid_argument("Matrix dimensions must be equal");
    }

    int row=matrix.size(), col=matrix[0].size()-1;
    Matrix ret(row, col);
    for(int i=1;i<=row;++i){
        for(int j=1;j<=col;++j){
            ret[i][j] = matrix[i][j]+other[i][j];
        }
        std::cout<<"ok"<<std::endl;
    }
    std::cout<<"ok"<<std::endl;
    return ret;
}

#endif // _MATRIX2_H_
