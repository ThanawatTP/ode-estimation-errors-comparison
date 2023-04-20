#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

double f(double x, double y, double z, double w)
{
    return 2*x - 1 - w - z - y;
}

double exact_solution(double x)
{
    return 2*x + 2*exp(-x) + 2*cos(x) - 3;
}

int main()
{
    double x0 = 0;  // initial value of x
    double y0 = 1;  // initial value of y
    double z0 = 0;  // initial value of y'
    double w0 = 0;  // initial value of y''
    double h = 0.1; // step size
    double x = x0;  // current value of x
    double y = y0;  // current value of y
    double z = z0;  // current value of y'
    double w = w0;  // current value of y''
    double n = 100000;  // final value of x

    // Explicit
    auto start = high_resolution_clock::now();

    vector<double> E_X(n+1);
    vector<double> E_Y(n+1);

    E_X[0] = x0;
    E_Y[0] = y0;

    for(int i=1;i<=n;i++) {
        double y_next = y + h*z;
        double z_next = z + h*w;
        double w_next = w + h*f(x,y,z,w);

        y = y_next;
        z = z_next;
        w = w_next;
        x = x + h;

        E_X[i] = x;
        E_Y[i] = y;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    cout << "Explicit\nN : " << n << "\n" ;
    cout << "Time taken: " << duration.count() << " microseconds\n";


    // Impilcit
    start = high_resolution_clock::now();

    vector<double> I_X(n+1);
    vector<double> I_Y(n+1);
    x = x0;  
    y = y0;
    z = z0;
    w = w0;

    I_X[0] = x0;
    I_Y[0] = y0;

    double y_next;
    double z_next;
    double w_next;

    for(int i=1; i<=n;i++) {
        y_next = y + h*z;
        z_next = z + h*w;
        w_next = w + h*f(x,y,z,w_next);

        x = x + h;
        y = y_next;
        z = z_next;
        w = w_next;

        I_X[i] = x;
        I_Y[i] = y;
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    
    cout << "Impilcit\nN : " << n << "\n" ;
    cout << "Time taken: " << duration.count() << " microseconds\n";


    // Exact
    vector<double> X(n+1);
    vector<double> Y(n+1);
    x = x0;  
    y = y0;

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
    fprintf(gnu, "set title 'Plot of Third-order n=%.0f'\n",(n)); // กำหนดหัวเรื่องกราฟ
    fprintf(gnu, "plot '-' with lines title 'Explicit', '-' with lines title 'y = x(x + 1)','-' with lines title 'Implicit' \n");
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", E_X[i], E_Y[i]);
    }
    fprintf(gnu, "e\n");
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", X[i], Y[i]);
    }
    fprintf(gnu, "e\n");
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", I_X[i], I_Y[i]);
    }
    fprintf(gnu, "e\n");
    pclose(gnu);

}