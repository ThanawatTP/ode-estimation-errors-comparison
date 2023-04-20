#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Define the derivative function
double f(double y, double x) {
    return 2*x + 1;
}

// Define the Adams-Bashforth method
vector<double> adams_bashforth(double x0, double y0, double h, int n) {
    auto start = high_resolution_clock::now();

    vector<double> y_list ;
    double y_next = y0 ;
    y_list.push_back(y0);

    // Euler      
    for(int i=1; i<=3; i++) {
        double yp = f(y_next,x0+h*(i-1)) ;
        y_next = y_next + h*(yp);
        y_list.push_back(y_next);
    }
    
    // Use the Adams-Bashforth formula to compute y_{i+1} for i >= 3
    for(int i=3;i<=n;i++){
        double k1 = f(y_list[i],x0+h*(i));
        double k2 = f(y_list[i-1],x0+h*(i-1));
        double k3 = f(y_list[i-2],x0+h*(i-2));
        y_next = y_next + h*( 23*(k1) - 16*(k2) + 5*(k3) )/12;
        y_list.push_back(y_next);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "adams_bashforth\nN : " << n << "\n" ;
    cout << "Time taken: " << duration.count() << " microseconds\n";
    
    return y_list;
}

// Define the Adams-Moulton method
vector<double> adams_moulton(double x0, double y0, double h, int n) {
    auto start = high_resolution_clock::now();

    vector<double> y_list ;
    double y_next = y0 ;
    y_list.push_back(y0);

    // Euler      
    for(int i=1; i<=3; i++) {
        double yp = f(y_next,x0+h*(i-1)) ;
        y_next = y_next + h*(yp);
        y_list.push_back(y_next);
    }
    
    // Use the Adams-Moulton formula to compute y_{i+1} for i >= 3
    for(int i=3;i<=n;i++){
        double k1 = f(y_list[i],x0+h*(i));
        double k2 = f(y_list[i-1],x0+h*(i-1));
        double k3 = f(y_list[i-2],x0+h*(i-2));
        double yp = y_next + h*( 23*(k1) - 16*(k2) + 5*(k3) )/12;

        k1 = f(yp, x0+h*(i+1)) ;
        k2 = f(y_list[i], x0+h*(i));
        k3 = f(y_list[i-1], x0+h*(i-1));

        y_next = y_next + h*( 5*(k1) + 8*(k2) - (k3) )/12;
        y_list.push_back(y_next);

    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "adams_moulton\nN : " << n << "\n" ;
    cout << "Time taken: " << duration.count() << " microseconds\n";
    
    return y_list;
}

int main() {
    // Set the initial conditions and parameters
    double x0 = 0;
    double y0 = 0;
    double h = 0.1;
    int n = 100000;

    // Calculate the exact solution
    vector<double> y_exact;
    for (int i = 0; i <= n; i++) {
        double x = x0 + i*h;
        double y = x*(x + 1);
        y_exact.push_back(y);
    }

    // Calculate the Adams-Bashforth solution
    vector<double> y_ab = adams_bashforth(x0, y0, h, n);

    // Calculate the Adams-Moulton method
    vector<double> y_am = adams_moulton(x0, y0, h, n);

    // Error
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


    // Plot the solution 
    FILE *gnu = popen("gnuplot -persistent", "w");
    fprintf(gnu, "set title 'Plot of First-order n=%d'\n",(n)); // กำหนดหัวเรื่องกราฟ
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
