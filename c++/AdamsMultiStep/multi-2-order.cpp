#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Define the derivative function
vector<double> f(vector<double> y, double h, double x){
    double yp_next = y[1] ;
    double ypp_next = (sin(x) - y[0]) ;
    y[0] = yp_next;
    y[1] = ypp_next;
    return y;
}

// Define the Adams-Bashforth method
vector<double> adams_bashforth(double x0, double y0, double y1, double h, int n) {
    auto start = high_resolution_clock::now();

    vector<double> y(n+1),y_next(2);
    vector< vector<double> >  y_list { {y0,y1} };
    // Set initial conditions
    y[0] = y_next[0] = y0;
    y_next[1] = y1;
    
    for(int i=1; i<=3;i++) {
        vector<double> yp_next = f(y_next,h,x0+h*(i-1));
        y_next[0] = y_next[0] + h*(yp_next[0]) ;
        y_next[1] = y_next[1] + h*(yp_next[1]) ;
        y[i] = y_next[0];
        y_list.push_back(y_next);
    }
    
    // Use the Adams-Bashforth formula to compute y_{i+1} for i >= 3
    for (int i = 3; i < n; i++) {
        vector<double> k1 = f(y_list[i],h,x0+h*(i)) ;
        vector<double> k2 = f(y_list[i-1],h,x0+h*(i-1));
        vector<double> k3 = f(y_list[i-2],h,x0+h*(i-2));
        
        y_next[0] = y_next[0] + h*( (23*k1[0]) - 16*(k2[0]) + 5*(k3[0]) )/12;
        y_next[1] = y_next[1] + h*( (23*k1[1]) - 16*(k2[1]) + 5*(k3[1]) )/12;
        y[i+1] = y_next[0];
        y_list.push_back(y_next);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "adams_bashforth\nN : " << n << "\n" ;
    cout << "Time taken: " << duration.count() << " microseconds\n";

    return y;
}

// Define the Adams-Moulton method
vector<double> adams_moulton(double x0, double y0, double y1, double h, int n) {
    
    auto start = high_resolution_clock::now();

    vector<double> y(n+1),y_next(2);
    vector< vector<double> >  y_list { {y0,y1} };
    // Set initial conditions
    y[0] = y_next[0] = y0;
    y_next[1] = y1;
    
    for(int i=1; i<=3;i++) {
        vector<double> yp_next = f(y_next,h,x0+h*(i-1));
        y_next[0] = y_next[0] + h*(yp_next[0]) ;
        y_next[1] = y_next[1] + h*(yp_next[1]) ;
        y[i] = y_next[0];
        y_list.push_back(y_next);
    }

    for (int i = 3; i < n; i++) {
        vector<double> yp(2) ;
        vector<double> k1 = f(y_list[i],h,x0+h*(i)) ;
        vector<double> k2 = f(y_list[i-1],h,x0+h*(i-1));
        vector<double> k3 = f(y_list[i-2],h,x0+h*(i-2));

        yp[0] = y_next[0] + h*( (23*k1[0]) - 16*(k2[0]) + 5*(k3[0]) )/12;
        yp[1] = y_next[1] + h*( (23*k1[1]) - 16*(k2[1]) + 5*(k3[1]) )/12;
        
        k1 = f(yp,h,x0+h*(i+1)) ;
        k2 = f(y_list[i],h,x0+h*(i));
        k3 = f(y_list[i-1],h,x0+h*(i-1));

        y_next[0] = y_next[0] + h*( (5*k1[0]) + 8*(k2[0]) - (k3[0]) )/12;
        y_next[1] = y_next[1] + h*( (5*k1[1]) + 8*(k2[1]) - (k3[1]) )/12;
        
        y[i+1] = y_next[0];
        y_list.push_back(y_next);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "adams_moulton\nN : " << n << "\n" ;
    cout << "Time taken: " << duration.count() << " microseconds\n";

    return y;
}

int main() {
    // Set the initial conditions and parameters
    double x0 = 0;
    double y0 = 0;
    double y1 = 1;
    double h = 0.1;
    int n = 100000;

    // Calculate the exact solution
    vector<double> y_exact;
    for (int i = 0; i <= n; i++) {
        double x = x0 + i*h;
        double y = 0.5*(3*sin(x) - x*cos(x));
        y_exact.push_back(y);
    }

    // Calculate the Adams-Bashforth solution
    vector<double> y_ab = adams_bashforth(x0, y0, y1, h, n);

    // Calculate the Adams-Moulton method
    vector<double> y_am = adams_moulton(x0, y0, y1, h, n);

    float error_ex=0;
    float error_im=0;
    float v_size = y_exact.size();
    float y_sum = accumulate(y_exact.begin(), y_exact.end(), 0);
    for(int i=0; i<v_size ;i++){
        error_ex += abs(y_exact[i]-y_ab[i]);
        error_im += abs(y_exact[i]-y_am[i]);
    }
    cout << "Error adams_bashforth = "  << abs((error_ex/y_sum)*100) << "\n";
    cout << "Error adams_moulton = "  << abs((error_im/y_sum)*100) << "\n";


    // // Plot the solution 
    FILE *gnu = popen("gnuplot -persistent", "w");
    fprintf(gnu, "set title 'Plot of Second-order n=%d'\n",(n)); // กำหนดหัวเรื่องกราฟ
    fprintf(gnu, "plot '-' with lines title 'adams-bashforth', '-' with lines title 'y = x(x + 1)','-' with lines title 'adams-moulton' \n");
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", (x0 + i*h), y_ab[i]);
    }
    fprintf(gnu, "e\n"); 
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", (x0 + i*h), y_exact[i]);
    }
    fprintf(gnu, "e\n");  
    for(int i=0;i<=n;i++) {
        fprintf(gnu, "%f %f\n", (x0 + i*h), y_am[i]);
    }
    fprintf(gnu, "e\n");
    pclose(gnu); 

    return 0;
}
