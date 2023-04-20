#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Define the derivative function
vector<double> f(vector<double> y, double h, double x){
    double y_next = y[1] ;
    double yp_next = y[2] ;
    double ypp_next = y[3] ;
    double yppp_next = -4*y[2] - 4*y[0] ;
    y[0] = y_next;
    y[1] = yp_next;
    y[2] = ypp_next;
    y[3] = yppp_next;
    return y;
}

// Define the Adams-Bashforth method
vector<double> adams_bashforth(double x0, double y0, double y1, double y2, double y3, double h, int n) {
    auto start = high_resolution_clock::now();

    vector<double> y(n+1),y_next(4);
    vector< vector<double> >  y_list { {y0,y1,y2,y3} };
    // Set initial conditions
    y[0] = y_next[0] = y0;
    y_next[1] = y1;
    y_next[2] = y2;
    y_next[3] = y3;
    
    for(int i=1; i<=3;i++) {
        vector<double> yp_next = f(y_next,h,x0+h*(i-1));
        y_next[0] = y_next[0] + h*(yp_next[0]) ;
        y_next[1] = y_next[1] + h*(yp_next[1]) ;
        y_next[2] = y_next[2] + h*(yp_next[2]) ;
        y_next[3] = y_next[3] + h*(yp_next[3]) ;
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
        y_next[2] = y_next[2] + h*( (23*k1[2]) - 16*(k2[2]) + 5*(k3[2]) )/12;
        y_next[3] = y_next[3] + h*( (23*k1[3]) - 16*(k2[3]) + 5*(k3[3]) )/12;

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
vector<double> adams_moulton(double x0, double y0, double y1, double y2, double y3, double h, int n) {
    
    auto start = high_resolution_clock::now();

    vector<double> y(n+1),y_next(4);
    vector< vector<double> >  y_list { {y0,y1,y2,y3} };
    // Set initial conditions
    y[0] = y_next[0] = y0;
    y_next[1] = y1;
    y_next[2] = y2;
    y_next[3] = y3;
    
    for(int i=1; i<=3;i++) {
        vector<double> yp_next = f(y_next,h,x0+h*(i-1));
        y_next[0] = y_next[0] + h*(yp_next[0]) ;
        y_next[1] = y_next[1] + h*(yp_next[1]) ;
        y_next[2] = y_next[2] + h*(yp_next[2]) ;
        y_next[3] = y_next[3] + h*(yp_next[3]) ;
        y[i] = y_next[0];
        y_list.push_back(y_next);
    }

    for (int i = 3; i < n; i++) {
        vector<double> yp(4) ;
        vector<double> k1 = f(y_list[i],h,x0+h*(i)) ;
        vector<double> k2 = f(y_list[i-1],h,x0+h*(i-1));
        vector<double> k3 = f(y_list[i-2],h,x0+h*(i-2));

        yp[0] = y_next[0] + h*( (23*k1[0]) - 16*(k2[0]) + 5*(k3[0]) )/12;
        yp[1] = y_next[1] + h*( (23*k1[1]) - 16*(k2[1]) + 5*(k3[1]) )/12;
        yp[2] = y_next[2] + h*( (23*k1[2]) - 16*(k2[2]) + 5*(k3[2]) )/12;
        yp[3] = y_next[3] + h*( (23*k1[3]) - 16*(k2[3]) + 5*(k3[3]) )/12;
        
        k1 = f(yp,h,x0+h*(i+1)) ;
        k2 = f(y_list[i],h,x0+h*(i));
        k3 = f(y_list[i-1],h,x0+h*(i-1));

        y_next[0] = y_next[0] + h*( (5*k1[0]) + 8*(k2[0]) - (k3[0]) )/12;
        y_next[1] = y_next[1] + h*( (5*k1[1]) + 8*(k2[1]) - (k3[1]) )/12;
        y_next[2] = y_next[2] + h*( (5*k1[2]) + 8*(k2[2]) - (k3[2]) )/12;
        y_next[3] = y_next[3] + h*( (5*k1[3]) + 8*(k2[3]) - (k3[3]) )/12;
        
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
    double y0 = 1;
    double y1 = 0;
    double y2 = 0;
    double y3 = -4;
    double h = 0.1;
    int n = 100000;

    // Calculate the exact solution
    vector<double> y_exact;
    for (int i = 0; i <= n; i++) {
        double x = x0 + i*h;
        double y = ( ( (x-1)*sin( sqrt(2)*x ) )/sqrt(2) ) 
        + ( (x+1)*cos( sqrt(2)*x ) ) ;
        y_exact.push_back(y);
    }

    // Calculate the Adams-Bashforth solution
    vector<double> y_ab = adams_bashforth(x0, y0, y1, y2, y3, h, n);

    // Calculate the Adams-Moulton method
    vector<double> y_am = adams_moulton(x0, y0, y1, y2, y3, h, n);

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
    fprintf(gnu, "set title 'Plot of Fourth-order n=%d'\n",(n)); // กำหนดหัวเรื่องกราฟ
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
