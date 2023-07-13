#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <math.h>

using namespace std;


class Matrix{
    pair<int, int> shape;
    double** mat;

    void allocateMat(int r, int c){
        mat = new double*[r];
        for(int i = 0; i < r; ++i){
            mat[i] = new double[c];
        }
    }

public:
    Matrix(){
        // cout << "in constructor\n";
        shape = {4, 4};
        allocateMat(4, 4);
    }
    Matrix(int r, int c){
        // cout << "in constructor\n";
        shape = {r, c};
        allocateMat(r, c);
    }
    Matrix(int r, int c, const vector<double> &v):Matrix(r, c){
        for(int i = 0; i < r * c; ++i){
            mat[i / c][i % c] = v[i];
        }
    }
    Matrix(const Matrix &mt){
        // cout << "in copy constructor\n";
        shape = mt.shape;
        allocateMat(mt.shape.first, mt.shape.second);
        for(int i = 0; i < mt.shape.first; ++i){
            for(int j = 0; j < mt.shape.second; ++j){
                mat[i][j] = mt.mat[i][j];
            }
        }
    }
    Matrix(int r) : Matrix(r, r){}

    Matrix(int r, const vector<double> &v) : Matrix(r, r, v) {}

    Matrix(int r, bool identity) : Matrix(r){
        if(identity) makeIdentity();
    }

    ~Matrix(){
        // cout << "in destructor\n";
        deallocate();
    }

    bool makeIdentity(){
        if(shape.first != shape.second){
            cout << "not squre mat\n";
            return  false;
        }
        for(int i = 0; i < shape.first; ++i){
            for(int j = 0; j < shape.second; ++j){
                if(i == j){
                    mat[i][j] = 1;
                }
                else{
                    mat[i][j] = 0;
                }
            }
        }
        return true;
    }

    bool assignToRow(int r, const vector<double> &v){
        if(r > shape.first || v.size() < shape.second){
            cout << "invalid params\n";
            return false;
        }
        for(int i = 0; i < shape.second; ++i){
            mat[r][i] = v[i];
        }
        return true;
    }

    bool assignToCol(int c, const vector<double> &v){
        if(c >= shape.second || v.size() < shape.first){
            cout << "invalid params\n";
            return false;
        }
        for(int i = 0; i < shape.first; ++i){
            mat[i][c] = v[i];
        }
        return true;
    }

    bool set(int i, int j, double val){
        if(i >= shape.first || j >= shape.second){
            cout << "invalid params\n";
            return false;
        }
        mat[i][j] = val;
        return true;
    }

    Matrix operator*(double s){
        Matrix mt(this->shape.first, this->shape.second);
        for(int i = 0; i < shape.first; ++i){
            for(int j = 0; j < shape.second; ++j)
                mt.mat[i][j] = this->mat[i][j] * s;
        }
        return mt;
    }

    void operator=(const Matrix &m){
        // cout << "in equal op\n";

        // Matrix mt(m.shape.first, m.shape.second);
        // shape = m.shape;
        // allocateMat(shape.first, shape.second);
        deallocate();
        shape = m.shape;
        allocateMat(shape.first, shape.second);
        for(int i = 0; i < m.shape.first; ++i){
            for(int j = 0;j < m.shape.second; ++j)
                mat[i][j] = m.mat[i][j];
        }
        // return *this;
    }

    void deallocate(){
        for(int i = 0; i < shape.first; ++i)
            delete[] mat[i];
        delete[] mat;
    }

    Matrix operator*(const Matrix &m){
        Matrix res(shape.first, m.shape.second);
        if(shape.second != m.shape.first){
            cout << "Mat mul shpape mismatch\n";
            return res;
        }
        for(int i = 0; i < res.shape.first; ++i){
            for(int j = 0; j < res.shape.second; ++j){
                double r = 0;
                for(int k = 0; k < shape.second; ++k){
                    r += this->mat[i][k] * m.mat[k][j];
                    // cout << "sdf";
                }
                // cout << "r : " << r << "\n";
                res.set(i, j, r);
            }
        }
        // cout << "here\n";
        return res;
    }
    
    double get(int i, int j){
        if(i >= shape.first || j >= shape.second){
            cout << "Out of index\n";
            return -1;
        }
        return mat[i][j];
    }

    friend class Printer;

};

class Printer{
public:
    static void print(const Matrix& m, const pair<int,int>& to, ostream& o, bool colWise = false){
        if(!colWise){

            for(int i = 0; i < to.first; ++i){
                for(int j = 0; j < to.second; ++j){
                    o << m.mat[i][j] << " ";
                }
                o << "\n";
            }
        }
        else{
            for(int i = 0; i < to.second; ++i){
                for(int j = 0; j < to.first; ++j){
                    o << m.mat[j][i] << " ";
                }
                o << "\n";
            }
        }
        o << "\n";
    }
};

class point {
public:
    double x, y, z;
    point operator*(float s) const{
        point t = {x * s, y * s, z * s};
        return t;
    }
    point operator+(const point &p) const{
        point t = {x + p.x, y + p.y, z + p.z};
        return t;
    }

    point operator-(const point& p) const{
        point t = {x - p.x, y - p.y, z - p.z};
        return t;
    }
};

point crossProduct(const point& p1, const point& p2){
    return {p1.y * p2.z - p2.y * p1.z, -p1.x * p2.z + p2.x * p1.z, p1.x * p2.y - p2.x * p1.y};
}

void normalize(point& p){
    double mag = sqrt(p.x  * p.x + p.y * p.y + p.z * p.z);
    if(mag > 1e-7){
        p = p * (1. / mag);
    }
}

double dotProduct(const point &p1,const point &p2){
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

class Solver{
    string inpPath, outPath;
    point eye,look,up;
    double fovY, ar, near, far;

    void readTriplet(point& x, ifstream& s){
        s >> x.x >> x.y >> x.z;
    }

    

    Matrix triangleMat(point &p1, point &p2, point &p3){
        Matrix mt(4,3);
        mt.assignToCol(0, {p1.x, p1.y, p1.z, 1});
        mt.assignToCol(1, {p2.x, p2.y, p2.z, 1});
        mt.assignToCol(2, {p3.x, p3.y, p3.z, 1});
        // Printer::print(mt, {3, 3}, cout);
        return mt;
    }

    Matrix translateMatrix(point &p1){
        Matrix mt(4, true);
        mt.assignToCol(3, {p1.x, p1.y, p1.z, 1});
        // Printer::print(mt, {4, 4}, cout);
        return mt;
    }

    Matrix scaleMatrix(point &p1){
        Matrix mt(4, true);
        mt.set(0, 0, p1.x);
        mt.set(1, 1, p1.y);
        mt.set(2, 2, p1.z);
        return mt;
    }
    
    point rodriguesFormula(point x, point a, double angle){
        point rt, cp;
        rt = x * cos(angle);
        rt = rt +  a * ((1 - cos(angle)) * dotProduct(a, x));
        cp = crossProduct(a, x);
        rt = rt + cp * sin(angle);
        return rt;
    }

    Matrix rotationMatrix(double angle, point& p1){
        point axis = p1;
        normalize(axis);
        point c1 = rodriguesFormula({1, 0, 0}, axis, angle);
        point c2 = rodriguesFormula({0, 1, 0}, axis, angle);
        point c3 = rodriguesFormula({0, 0, 1}, axis, angle);
        Matrix mt(4, true);
        mt.assignToCol(0, {c1.x, c1.y, c1.z, 0});
        mt.assignToCol(1, {c2.x, c2.y, c2.z, 0});
        mt.assignToCol(2, {c3.x, c3.y, c3.z, 0});
        return mt;
    }


public:
    Solver(string inputPath, string outputPath){
        inpPath = inputPath;
        outPath = outputPath;

        ifstream scene;
        scene.open(inputPath + "/scene.txt", ios::in);
        ofstream of;
        of.open(outPath + "/stage1.txt", ios::out);

        if(!scene){
            cout << "scene file does not exist\n";
            return;
        }

        readTriplet(eye, scene);
        readTriplet(look, scene);
        readTriplet(up, scene);
        scene >> fovY >> ar >> near >> far;

        modellingTransformation(scene, of);
    }

    void modellingTransformation(ifstream &s, ofstream &o){
        stack<Matrix> st;
        Matrix curr(4, true);
        st.push(curr);

        string command;
        while(s.peek() != EOF){
            s >> command;
            if(command == "triangle"){
                point p1,p2,p3;
                readTriplet(p1, s);
                readTriplet(p2, s);
                readTriplet(p3, s); 
                Matrix tr = triangleMat(p1, p2, p3);
                tr = curr * tr;
                // tr = tr * (1. / tr.get(3, 3));
                // Printer::print(tr, {4, 4}, cout);
                // Printer::print(curr, {4, 4}, cout);
                Printer::print(tr, {4, 3}, o, true);
                // cout << tr.get(3, 3) << "\n";
                
            }
            else if(command == "push"){
                st.push(curr);
            }
            else if(command == "scale"){
                point p1;
                readTriplet(p1, s);
                Matrix mt = scaleMatrix(p1);
                // Printer::print(curr, {4,4}, cout);
                curr = curr * mt;
                // curr = mt * curr;
                // Printer::print(curr, {4,4}, cout);
            }
            else if(command == "translate"){
                point p1;
                readTriplet(p1, s);
                Matrix mt = translateMatrix(p1);
                Printer::print(curr, {4,4}, cout, false);
                curr = curr * mt;
                Printer::print(curr, {4,4}, cout, false);
            }
            else if(command == "rotate"){
                point p1;
                double angle;
                s >> angle;
                readTriplet(p1, s);
                // Printer::print(curr, {4,4}, cout);
                Matrix mt = rotationMatrix(angle * (M_PI / 180.), p1);
                curr = curr * mt;
                // curr = mt * curr;
                // Printer::print(mt, {4,4}, cout);
                // Printer::print(curr, {4,4}, cout);
            }
            else if(command == "pop"){
                if(!st.empty()){
                    curr = st.top();
                    st.pop();
                }
            }
            else if(command == "end"){
                break;
            }
        }
        cout << "file parsed";
    }
};


int main(){
    // Matrix m;
    // Printer::print(m, {3, 3}, cout);
    // Matrix m2 = m;
    // m.set(1, 2, 4);
    // m2 = m;
    Solver solve("./TestCases/4", ".");
    // cout << m.shape.first << " " << m.shape.second << "\n";
}