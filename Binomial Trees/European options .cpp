/* C++ For Quantitative Finance                                 */
/* European options (dividend yield)                            */
/* Author: David Li                                             */


#include <iostream>
#include <string>
#include <cmath>
using namespace std;

double termVal(char iType, double strkPr, double termPr);
void printTree(double tree[][10], int numSteps, double timeStep);

int main(int argc, char **argv)
{
    if (argc != 7) 
    {
        cout << "S K r q T sig" << endl;
        return 1;
    }
    double S = atof(argv[1]);   /* price of stock or underlying security */
    double K = atof(argv[1]);   /* strike price                          */
    double r = atof(argv[1]);   /* interest rate                         */
    double q = atof(argv[1]);   /* dividend yield                        */
    double T = atof(argv[1]);   /* remaining life                        */
    double sig = atof(argv[1]); /* volatility                            */

    const int maxSize = 10;
    double stk[maxSize][maxSize];  /* array for the stock price tree     */
    double euro[maxSize][maxSize]; /* array for option valuation tree    */
    char optType;                  /* c for call and p for put           */
    int i, j, n;
    double dt, u, d, p, emrdt;

    n = 5;                         /* number of steps; must be < maxSize */
    optType = 'c';

    dt = T / n;                    /* step size in years       */
    u = exp(sig * sqrt(dt));       /* up movement multiplier   */
    d = 1/u;                       /* down movement multiplier */
    emrdt = exp(-r * dt);          /* discount factor per step */

    /*      p is risk neutral probability of up movement       */
    p = (exp((r -q) * dt) - d) / (u - d);

    stk[0][0] = S;                /* initial stock price at time 0 */


    /*  generate stock price tree  */
    for(j = 1; j <= n; j++)       /*  controls stepping       */
    {
        for(i = 0; i <= j; i++)
        {
          if (i == 0) 
          {
              stk[i][j] = stk[i][j-1] * u;
          }
          else 
          {
              stk[i][j] = stk[i-1][j-1] * d;
          }
        }
    }

    /* generate option value tree  */
    for(j = n; j >= 0; j--) /* control stepping  */
    {
        for(i = 0; i <= j; i++)
        {
          if (j == n) 
          {
              euro[i][j] = termVal(optType, K, stk[i][j]);
          }
          else 
          {
              euro[i][j] = (p * euro[i][j+1] + (1 -p) * euro [i+1][j+1]) * emrdt;
          }
        }
    }

    cout << "Value of option: " << euro[0][0];
    cout << endl;

    cout << endl << " Stock price tree" << endl << endl;
    printTree(stk, n, dt);
    cout << endl;

    cout << endl << " Option value tree" << endl << endl;
    printTree(euro, n, dt);
    cout << endl;

    return 0;
}



double termVal(char iType, double strkPr, double termPr)
{
    /*
    functions parameters:
    iType: position type, c, p, or u (underlying)
    strkPr: strike price of option or purchase price of u
    termPr: terminal price of u
    */

    double posVal = 0.;

    switch (iType)
    {
    case 'u':
        posVal = termPr - strkPr;
        break;

    case 'c':
        if(termPr > strkPr) 
        {
            posVal = termPr - strkPr;
        }
        break;

    case 'p':
        if(termPr < strkPr) 
        {
            posVal = strkPr - termPr;
        }
        break;
    }

    return posVal;
}



void printTree(double tree[][10], int numSteps, double timeStep)
{
    int i, j, k;

    cout << " Step";
    cout.setf (ios::fixed);

    for(j = 0; j <= numSteps; j++)
    {
        cout.width(10);
        cout << j;
    }

    cout << endl;
    cout << " Time";
    cout.precision(2);
    cout.setf (ios::fixed);
    cout.setf(ios::showpoint);

    for(j = 0; j <= numSteps; j++)
    {
        cout.width(10);
        cout << j * timeStep;
    }

    cout << endl;

    for(i = 0; i <= numSteps; i++)
    {
        cout << endl;
        for (k = 1; k <= i+1; k++) {
            cout << " ";
        }

        for(j = i; j <= numSteps; j++)
        {
            cout.width(10);
            cout << tree[i][j];
        }
    }

    return;
}
