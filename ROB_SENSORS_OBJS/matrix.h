//
// Created by stefan on 18/08/15.
//

#ifndef PROJECT_MATRIX_H
#define PROJECT_MATRIX_H

#include "iostream"
#include "cstdlib"
#include "map"
#include "vector"

template <class T>
class Matrix{
public:
    typedef std::map<size_t, std::map<size_t , T> > map_t;
    typedef typename map_t::iterator row_iter;
    typedef std::map<size_t , T> col_t;
    typedef typename col_t::iterator col_iter;

    Matrix(size_t i){
        m = i;
        n = i;
    }
    Matrix(size_t i, size_t j){
        m = i, n = j;
    }

    inline T& operator()(size_t i, size_t j){
        if(i >= m || j >= n) throw ;
        return mat[i][j];
    }

    inline T operator()(size_t i, size_t j) const{
        if(i >= m || j >= n) throw ;
        return mat[i][j];
    }

    /*Computes y=A * x */
    std::vector<T> operator* (const std::vector<T>& x){
        if(this->m != x.size()) throw;

        std::vector<T> y(this->m);
        T sum;

        row_iter ii;
        col_iter jj;

        for(ii = this->mat.begin(); ii != this-> mat.end(); ii++){
            sum = 0;
            for(jj=(*ii).second.begin(); jj != (*ii).second.end();jj++){
                sum += (*jj).second * x[(*jj).first];
            }
            y[(*ii).first] = sum;
        }
        return  y;
    }

    void print_matrix(){
        row_iter ii;
        col_iter jj;

        for(ii = this->mat.begin(); ii != this->mat.end(); ii++){
            for(jj = (*ii).second.begin(); jj != (*ii).second.end(); jj++){
                std::cout << (*ii).first << ' ';
                std::cout << (*jj).first << ' ';
                std::cout << (*jj).second << std::cout;
            }
        }
        std::cout << std::endl;
    }

protected:
    Matrix(){}

private:
    map_t mat;
    size_t m;
    size_t n;
};



#endif //PROJECT_MATRIX_H
