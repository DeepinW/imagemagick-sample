#include <iostream>
#include <cstring>
#include <cassert>

template<class T>
class Haar2D
{
public:

    Haar2D(int point);
    ~Haar2D();

    void Transform(T *mat);

    size_t GetMatSize() const;

private:

    void HaarRow(int n, int row_num, T *mat);
    void HaarCol(int n, int col_num, T *mat);

private:

    int point_;
    size_t mat_size_;
    T *buff_;
};

template<class T>
Haar2D<T>::Haar2D(int point):
    point_(point)
{
    mat_size_ = 1 << point;
    buff_ = new T[mat_size_ * mat_size_];
    assert(buff_ != NULL);
}

template<class T>
Haar2D<T>::~Haar2D()
{
    delete buff_;
}

template<class T>
void Haar2D<T>::HaarRow(int n, int row_num, T *mat)
{
    assert(n > 0);
    assert(n <= point_);

    T *a = &buff_[0];
    T *d = &buff_[1 << (n - 1)];

    T *row = mat + (row_num * mat_size_);

    for (int i = 0; i < (1 << (n - 1)); i++)
    {
        a[i] = (row[i * 2] + row[i * 2 + 1]) / 2;
        d[i] = (row[i * 2] - row[i * 2 + 1]) / 2; 
    }

    memcpy(row, buff_, sizeof(T) * (1 << n));
}

template<class T>
void Haar2D<T>::HaarCol(int n, int col_num, T *mat)
{
    assert(n > 0);
    assert(n <= point_);

    T *a = &buff_[0];
    T *d = &buff_[1 << (n - 1)];

    for (int i = 0; i < (1 << (n - 1)); i++)
    {
        a[i] = (mat[i * 2 * mat_size_ + col_num] + mat[(i * 2 + 1) * mat_size_ + col_num]) / 2;
        d[i] = (mat[i * 2 * mat_size_ + col_num] - mat[(i * 2 + 1) * mat_size_ + col_num]) / 2;
    }

    for (int i = 0; i < (1 << (n - 1)); i++)
    {
        mat[i * mat_size_ + col_num] = a[i];
        mat[((1 << (n - 1)) + i) * mat_size_ + col_num] = d[i];
    }
}

template<class T>
void Haar2D<T>::Transform(T *mat)
{
    int n = point_;

    while (n > 0)
    {
        for (int i = 0; i < (1 << n); i++)
        {
            HaarRow(n, i, mat);
        }

        for (int i = 0; i < (1 << n); i++)
        {
            HaarCol(n, i, mat);
        }

        --n;
    }
}

template<class T>
size_t Haar2D<T>::GetMatSize() const
{
    return mat_size_;
}

