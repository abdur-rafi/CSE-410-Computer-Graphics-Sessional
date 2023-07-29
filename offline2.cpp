#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "bitmap_image.hpp"

#define EPS 1e-6

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

    vector<double> getRow(int i){
        if (i >= shape.first){
            printf("out of index\n");
            return {0};
        }
        vector<double> v;
        for(int j = 0; j < shape.second; ++j)
            v.push_back(mat[i][j]);
        return v;
    }
    vector<double> getCol(int i){
        if (i >= shape.second){
            printf("out of index\n");
            return {0};
        }
        vector<double> v;
        for(int j = 0; j < shape.first; ++j)
            v.push_back(mat[j][i]);
        return v;
    }

    pair<int,int> getShape(){
        return shape;
    }

    friend class Printer;

};

class Printer{
public:
    static void print(const Matrix& m, const pair<int,int>& to, ostream& o, bool colWise = false){
        // o << fixed ;
        if(!colWise){

            for(int i = 0; i < to.first; ++i){
                for(int j = 0; j < to.second; ++j){
                    o << fixed << setprecision(7) << m.mat[i][j] << " ";
                }
                o << "\n";
            }
        }
        else{
            for(int i = 0; i < to.second; ++i){
                for(int j = 0; j < to.first; ++j){
                    o << fixed << setprecision(7) << m.mat[j][i] << " ";
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
    void print(){
        cout << "x : "  << x << " y : " << y << " z : " << z << "\n";
    }
};

point crossProduct(const point& p1, const point& p2){
    return {p1.y * p2.z - p2.y * p1.z, -p1.x * p2.z + p2.x * p1.z, p1.x * p2.y - p2.x * p1.y};
}

void normalize(point& p){
    double mag = sqrt(p.x  * p.x + p.y * p.y + p.z * p.z);
    if(mag > 1e-8){
        // p = p * (1. / mag);
        p.x /= mag;
        p.y /= mag;
        p.z /= mag;
    }
}

double dotProduct(const point &p1,const point &p2){
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

static unsigned long int g_seed = 1; 
inline int random() 
{ 
    g_seed = (214013 * g_seed + 2531011); 
    return (g_seed >> 16) & 0x7FFF; 
}

struct Triangle{
    point p1,p2,p3;
    rgb_t color;

};

struct Line{
    point base, direction;
};

class Solver{
    string inpPath, outPath;
    point eye,look,up;
    double fovY, ar, near, far;
    int trCount = 0;
    int w, h;

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

    void scaleCol(Matrix &m, int i){
        // cout << m.getShape().first << " " << m.getShape().second << "\n";
        vector<double> v = m.getCol(i);
        double val = m.get(m.getShape().first - 1, i);
        for(int i = 0; i < v.size(); ++i)
            if(abs(val) > (1e-7))
                v[i] /=  val;
        
        m.assignToCol(i, v);
    }

    double mxOf3(double x, double y, double z){
        return max(x, max(y, z));
    }
    double mnOf3(double x, double y, double z){
        return min(x, min(y, z));
    }

    double tAtIntersection(Line &l1, double yVal){
        // base.y + t * direction.y = yVal
        if(abs(l1.direction.y) < EPS){
            return -2;
        }
        return (yVal - l1.base.y) / l1.direction.y;
    }

    double tAtIntersectionX(Line &l1, double xVal){
        // base.x + t * direction.x = xVal
        if(abs(l1.direction.x) < EPS){
            return 0;
        }
        return (xVal - l1.base.x) / l1.direction.x;
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

        auto trs = modellingTransformation(scene, of);
        of.close();
        scene.close();
        ifstream stage1_i;
        ofstream stage2_o;
        stage2_o.open(outPath + "/stage2.txt", ios::out);
        viewTransformation(trs, stage2_o);
        stage2_o.close();

        ofstream stage3_o;
        stage3_o.open(outPath + "/stage3.txt", ios::out);
        projectionTransformation(trs, stage3_o);
        stage3_o.close();

        ifstream config;
        config.open(inputPath + "/config.txt", ios::in);
        config >> w >> h ;
        config.close();
        clipAndScanBuffer(trs);
    }

    void clipAndScanBuffer(vector<Matrix> &trMs){
        // cout << w << " " << h;
        vector<Triangle> trs;
        for(auto v : trMs){
            vector<point> points;
            for(int i = 0; i < 3; ++i){
                auto x = v.getCol(i);
                points.push_back({x[0], x[1], x[2]});
            }
            rgb_t color = {random() % 256, random() % 256, random() % 256};
            trs.push_back({points[0], points[1], points[2], color});
        }

        // for(auto x : trs){
        //     x.p1.print();
        //     x.p2.print();
        //     x.p3.print();
        //     x.color.print();
        // }
        
        bitmap_image image(w, h);
        image.set_all_channels(0);

        
        double** buffer = new double*[h];
        for(int i = 0; i < h; ++i){
            buffer[i] = new double[w];
            for(int j = 0; j < w; ++j)
                buffer[i][j] = 2;
        }
        
        
        double dx = 2./ w;
        double dy = 2. / h;
        double topY = 1 - dy / 2;
        double bottomY = -1 + dy / 2; 
        double leftX = -1 + dx / 2;
        
        for(auto tr : trs){
            double mxY = mxOf3(tr.p1.y, tr.p2.y, tr.p3.y);
            double mnY = mnOf3(tr.p1.y, tr.p2.y, tr.p3.y);
            mxY = min(mxY, 1.);
            mnY = max(mnY, -1.);
            int topIndex = round(abs(mxY - 1.) / dy);
            int bottomIndex = round(abs(mnY - 1) / dy);
            // if(abs(mnY - 1) / dy - bottomIndex >= .5)
            //     ++bottomIndex;
            // bottomIndex = min(bottomIndex, h - 1);

            Line l1 = {tr.p1, tr.p2 - tr.p1};
            Line l2 = {tr.p2, tr.p3 - tr.p2};
            Line l3 = {tr.p3, tr.p1 - tr.p3};
            Line p1L, p2L;
            
            // cout << "topIndex : " << topIndex << " bottomIndex : " << bottomIndex << "\n";
            point p1,p2;
            for(int i = topIndex; i < bottomIndex; ++i){
                double yVal = topY - i * dy;
                double t1, t2;
                t1 = tAtIntersection(l1, yVal);
                p1 = l1.base + l1.direction * t1 ;
                p1L = l1;
                t2 = tAtIntersection(l2, yVal);
                p2 = l2.base + l2.direction * t2;
                p2L = l2;
                if(t1 < 0 || t1 > 1){
                // cout << "t1 : " << t1 <<  "\n";
                    t1 = tAtIntersection(l3, yVal);
                    p1 = l3.base + l3.direction * t1;
                    p1L = l3;
                }
                else if(t2 < 0 || t2 > 1){
                    t2 = tAtIntersection(l3, yVal);
                    p2 = l3.base + l3.direction * t2;
                    p2L = l3;
                }
                // Line left, right;
                // if(p1.x < p2.x){
                //     left = p1L;
                //     right = p2L;
                // }
                // else{
                //     left = p2L;
                //     right = p1L;
                // }

                double left = min(p1.x, p2.x);
                double right = max(p1.x, p2.x);
                left = max(left, -1.);
                right = min(right, 1.);
                int leftIndex = round((left + 1.) / dx);
                int rightIndex = round((right + 1.) / dx);
                // cout << leftIndex << " " << rightIndex << "\n";
                Line l3 = {p1, p2 - p1};

                for(int j = leftIndex; j < rightIndex; ++j){
                    double currX = leftX + j * dx;
                    double t = tAtIntersectionX(l3, currX);
                    point pt = l3.base + l3.direction * t;
                    if(pt.z < buffer[i][j]){
                        buffer[i][j]  = pt.z;
                        image.set_pixel(j, i, tr.color);
                    }
                    // cout << pt.z << "\n";
                }
                // cout << "t1 : " << t1 << " t2: " << t2 << "\n";
            }
        }


        ofstream zbuffer;
        zbuffer.open(outPath + "/z_buffer.txt");

        for(int i = 0; i < h; ++i){
            for(int j = 0; j < w; ++j){
                if(buffer[i][j] == 2.)
                    continue;
                else
                    zbuffer << buffer[i][j] << "   ";
            }
            zbuffer << "\n";
        }

        image.save_image("out.bmp");
    }

    void viewTransformation(vector<Matrix> &trs, ofstream &o){
        point l = look - eye;
        normalize(l);
        point r = crossProduct(l, up);
        normalize(r);
        point u = crossProduct(r, l);
        Matrix T(4, true);
        T.assignToCol(3, {-eye.x, - eye.y, -eye.z, 1});
        Matrix R(4, true);
        R.assignToRow(0, {r.x, r.y, r.z, 0});
        R.assignToRow(1, {u.x, u.y, u.z, 0});
        R.assignToRow(2, {-l.x, -l.y, -l.z, 0});
        Matrix V = R * T;

        for(int i = 0; i < trs.size(); ++i){
            trs[i] = V * trs[i];
            Printer::print(trs[i], {3,3}, o, true);
        }
    }

    void projectionTransformation(vector<Matrix> &trs, ofstream &o){
        double fovX = fovY * ar;
        double t = near * tan((fovY / 2.) * (acos(-1) / 180.));
        double r = near * tan((fovX / 2.) * (acos(-1) / 180.));
        Matrix P(4);
        P.assignToRow(0, {near/r, 0, 0, 0});
        P.assignToRow(1, {0, near / r, 0, 0});
        P.assignToRow(2, {0, 0, -(far + near) / (far - near) , (-2*far*near)/(far - near)});
        P.assignToRow(3, {0, 0, -1 , 0});


        for(int i = 0; i < trs.size(); ++i){
            trs[i] = P * trs[i];
            scaleCol(trs[i], 0);
            scaleCol(trs[i], 1);
            scaleCol(trs[i], 2);
            Printer::print(trs[i], {3,3}, o, true);
        }
    }

    Matrix triangleMatFromFile(ifstream &s){
        point p1,p2,p3;
        readTriplet(p1, s);
        readTriplet(p2, s);
        readTriplet(p3, s); 
        return triangleMat(p1, p2, p3);

    }

    vector<Matrix> modellingTransformation(ifstream &s, ofstream &o){
        stack<Matrix> st;
        Matrix curr(4, true);
        st.push(curr);
        vector<Matrix> triangles;

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
                triangles.push_back(tr);
                Printer::print(tr, {3, 3}, o, true);
                ++trCount;
                
            }
            else if(command == "push"){
                st.push(curr);
            }
            else if(command == "scale"){
                point p1;
                readTriplet(p1, s);
                Matrix mt = scaleMatrix(p1);
                curr = curr * mt;
            }
            else if(command == "translate"){
                point p1;
                readTriplet(p1, s);
                Matrix mt = translateMatrix(p1);
                curr = curr * mt;
            }
            else if(command == "rotate"){
                point p1;
                double angle;
                s >> angle;
                readTriplet(p1, s);
                Matrix mt = rotationMatrix(angle * ((double)acos((double)-1) / 180.), p1);
                curr = curr * mt;
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
        return triangles;
        // cout << "file parsed\n";
    }
};


int main(int argc, char** argv){
    // Matrix m;
    // Printer::print(m, {3, 3}, cout);
    // Matrix m2 = m;
    // m.set(1, 2, 4);
    // m2 = m;
    string dir = "./TestCases_/1";
    // if(argc > 1){
    //     dir = argv[1];
    // }
    Solver solve(dir, ".");
    // cout << m.shape.first << " " << m.shape.second << "\n";
}