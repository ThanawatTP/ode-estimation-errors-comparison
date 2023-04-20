#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

double f(double x, double y, double yp, double ypp, double yppp) {
    return -4 * ypp - 4 * yp - y;
}

double exact_solution(double x) {
    double root2 = sqrt(2);
    return ((x - 1) * sin(root2 * x) / root2) + ((x + 1) * cos(root2 * x));
}

int main() {
    double x0 = 0.0;
    double y0 = 1.0;
    double yp0 = 0.0;
    double ypp0 = 0.0;
    double yppp0 = -4.0;
    double h = 0.1;

    double x = x0;
    double y = y0;
    double yp = yp0;
    double ypp = ypp0;
    double yppp = yppp0;
    double n = 100000 ; 
    

    // Explicit
    auto start = high_resolution_clock::now();
    
    vector<double> E_X(n + 1);
    vector<double> E_Y(n + 1);

    E_X[0] = x0;
    E_Y[0] = y0;

    for (int i = 1; i <= n; i++) {

        double wt = (yp*h) + y ;
        double xt = (ypp*h) + yp ;
        double yt = (yppp*h) + ypp ;
        double zt = ( ( (-4)*ypp ) - ( 4*y ) ) *h + yppp ;

        y = wt;
        yp = xt;
        ypp = yt;
        yppp = zt;
        x = x+h;

        E_X[i] = x;
        E_Y[i] = y;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Fourth Order Explicit\nN : " << n << "\n";
    cout << "Time taken: " << duration.count() << " microseconds\n";


    // Implicit
    start = high_resolution_clock::now();
    x = x0;
    y = y0;
    yp = yp0;
    ypp = ypp0;
    yppp = yppp0;

    vector<double> I_X(n + 1);
    vector<double> I_Y(n + 1);

    I_X[0] = x0;
    I_Y[0] = y0;

    for (int i = 1; i <= n; i++) {

        double wt = (yp*h) + y ;
        double xt = (ypp*h) + yp ;
        double yt = (yppp*h) + ypp ;
        double zt = ( ( (-4)*ypp ) - ( 4*y ) ) *h + yppp ;

        y = wt;
        yp = xt;
        ypp = yt;
        yppp = zt;

        wt = (yp*h) + y ;
        xt = (ypp*h) + yp ;
        yt = (yppp*h) + ypp ;
        zt = ( ( (-4)*ypp ) - ( 4*y ) ) *h + yppp ;

        y = wt;
        yp = xt;
        ypp = yt;
        yppp = zt;
        x = x+h;

        I_X[i] = x;
        I_Y[i] = y;
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    cout << "Fourth Order Implicit\nN : " << n << "\n";
    cout << "Time taken: " << duration.count() << " microseconds\n";


    // Exact
    vector<double> X(n+1);
    vector<double> Y(n+1);
    x = x0;  
    y = y0;

    X[0] = x;
    Y[0] = y;

    for(int i=1;i<=n;i++) {
        y = exact_solution(x);
        x = x + h;

        X[i] = x;
        Y[i] = y;
    }


    // Error 
    float error_ex=0;
    float error_im=0;
    float v_size = Y.size();
    float y_sum = accumulate(Y.begin(), Y.end(), 0);
    for(int i=1; i<v_size ;i++){
        error_ex += abs(Y[i]-E_Y[i]);
        error_im += abs(Y[i]-I_Y[i]);
    }
    cout << "Error Explicit = "  << (error_ex/y_sum)*100 << "\n";
    cout << "Error Implicit = "  << (error_im/y_sum)*100 << "\n";


    // Plot the solution
    FILE *gnu = popen("gnuplot -persistent", "w");
    fprintf(gnu, "set title 'Plot of Fourth-order n=%.0f'\n",(n)); // กำหนดหัวเรื่องกราฟ
    fprintf(gnu, "plot '-' with lines title '( (x-1)sin( (2^1/2)*x )/(2^1/2))+(x+1)cos((2^1/2) *x)' ,'-' with lines title 'Explicit','-' with lines title 'Implicit' \n");
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", X[i], Y[i]);
    }
    fprintf(gnu, "e\n");
    for(int i=0;i<=n;i++) {
        if( !isnan(E_Y[i]) && !isinf(E_Y[i])){
            fprintf(gnu, "%f %f\n", E_X[i], E_Y[i]);
        }   
    }
    fprintf(gnu, "e\n");
    for(int i=0;i<=n;i++) {
        if( !isnan(I_Y[i]) && !isinf(I_Y[i])){
            fprintf(gnu, "%f %f\n", I_X[i], I_Y[i]);
        }   
    }
    fprintf(gnu, "e\n");

    
    pclose(gnu);

}
