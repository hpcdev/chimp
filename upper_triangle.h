#ifndef UPPER_TRIANGLE_H
#define UPPER_TRIANGLE_H

struct SymmetryFix {
    void operator()(int & i, int & j) {
        if (j < i)
            std::swap(i,j);
    }
};

struct NullSymmetryFix {
    void operator()(int & i, int & j) {}
};

template <class T, class SymmetricFixer = NullSymmetryFix>
struct upper_triangle : std::vector<T> {
    typedef std::vector<T> super;

    upper_triangle(const int & n = 0) {
        resize(n);
    }

    void resize(const int & n) {
        this->n = n;
        super::resize((n*(n+1))/2);
    }

    inline int indx(int i, int j) const {
        SymmetricFixer()(i,j);
        return i*n - (i*(i+1))/2 + j;
    }

    T & operator()(const int & i, const int & j) {
        return this->operator[](indx(i,j));
    }

    const T & operator()(const int & i, const int & j) const {
        return this->operator[](indx(i,j));
    }
  private:
    int n;
};

#endif // UPPER_TRIANGLE_H
