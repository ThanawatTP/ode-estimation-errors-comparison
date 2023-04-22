
# ODE Estimation Errors Comparison
comparison of the errors between the exact solution and the approximate solution using the Explicit method and Implicit method of the Single-step Euler method, as well as the Multi-step methods using the Adams-Bashforth and Adams-Moulton methods for ODEs of sizes 1 through 4. Measures for comparison, such as the error compared to the exact solution, the computation time for each method, and the programming languages used, including Python, MATLAB, Julia, and C++.

## Single-step Euler method
The Single-step Euler method is a numerical method used to solve ordinary differential equations (ODEs) with an initial value given. It involves numerical approximations and is composed of
- Explicit Euler Method
- Implicit Euler Method
## Multi-step methods
In the comparison of the Multiple-step method, the Adams-Bashforth method and the Multi-step Adams-Moulton method were chosen as the two numerical methods commonly used to solve ordinary differential equations (ODEs). In this study, both methods were included for paired comparison with the Single-step Euler method.
- Adams-Bashforth Methods
    : are explicit methods, meaning that the value at the next time step can be explicitly calculated from the values at the previous time steps.
- Adams-Moulton Methods
    : are implicit methods, meaning that the value at the next time step is not explicitly calculated from the values at the previous time steps. 

## Taylor Series 
A numerical method for approximating the value of a function near a given point by expressing the function as a polynomial of infinitely many terms.
## Mid-point Method
A numerical method for approximating the solution of an ordinary differential equation (ODE) by taking one step of the Euler method using the average of the derivative at the beginning and the end of the interval.
## Heun Method
A numerical method for approximating the solution of an ODE that is an extension of the mid-point method, using an additional step to estimate the derivative at the midpoint and improve accuracy.
## Runge-Kutta Method
A family of numerical methods for approximating the solution of an ODE that involves computing several intermediate values of the derivative function at different points within each time step, then using a weighted average of these values to update the solution at each time step. The method is widely used due to its accuracy and versatility.
## summarize 
1. Single-step Forward and Backward Euler methods are found to be less accurate than other methods studied, with error percentages increasing as the order of the differential equations increases. The Backward Euler method is more accurate than the Forward Euler method.
2. Multiple-step Adams-Bashforth and Adams-Moulton methods are found to be more accurate than Euler method, but less accurate than other methods studied. The error percentages for these methods decrease, but they require more computational time as the order of the differential equations increases.
3. Taylor Series method is more accurate than the Euler method, but the error percentages increase as the number of methods used increases.
4. Mid-point and Heun methods are found to be equally accurate, with the same error percentages, but the Mid-point method takes less time to compute. The error percentages decrease as the order of the differential equations increases. These methods have lower resource requirements than Taylor Series but higher than Euler method.
5. Runge-Kutta method is the most accurate among the methods studied, with error percentages decreasing as the order of the differential equations increases. However, this method is the most resource-intensive among the methods studied.
