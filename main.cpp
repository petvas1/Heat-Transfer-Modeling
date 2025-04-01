#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main()
{
    // Definition of variables
    double T0, Tf, Tz, Tc, cp, lambda, ro, x, y, dt, a, tend, t, alpha, fo, bi;
    const double X = 0.6, Y = 0.6, dx = 0.005, dy = 0.005;
    const int I = X / dx + 1;
    const int J = Y / dy + 1;
    double T[I][J], T2[I][J]; // T2 is used for storing updated values

    // Material and boundary properties
    fo = 0.19;
    T0 = 55;  // Initial temperature
    Tf = 30;  // Fluid temperature
    Tz = 45;  // Green regions temperature
    Tc = 15;  // Black regions temperature
    cp = 840; ro = 2500; lambda = 1.05;
    a = lambda / (ro * cp); // Thermal diffusivity
    alpha = 60;  // Heat transfer coefficient
    bi = alpha * dx / lambda; // Biot number
    dt = fo * dx * dx / a; // Time step size
    cout << "dt = " << dt << endl;
    tend = 20 * 3600; // Total simulation time (20 hours)

    // Initial temperature distribution setup
    for (int i = 0; i < I; i++) {
        x = i * dx;
        for (int j = 0; j < J; j++) {
            y = j * dy;

            // Assigning boundary conditions based on geometry (black and green areas)
            if ((x == 0 && y >= 0.1 && y <= 0.2) ||
                (x == 0.2 && y >= 0.5 && y <= 0.6) ||
                ((x == 0.3 || x == 0.5) && y > 0.4 && y <= 0.5) ||
                (x == 0.5 && y >= 0.1 && y <= 0.3) ||
                ((x == 0.4 || x == 0.6) && y >= 0 && y <= 0.1) ||
                (y == 0 && x > 0.4 && x < 0.6) ||
                (y == 0.1 && x > 0.5 && x < 0.6)) {
                T[i][j] = Tc; // Black regions
            }
            else if ((x == 0.1 && y > 0.2 && y < 0.6) ||
                     (x == 0.6 && y > 0.3 && y < 0.5) ||
                     (y == 0.4 && x > 0.3 && x < 0.5)) {
                T[i][j] = Tz; // Green regions
            }
            else if (x >= 0 && x < 0.4 && y >= 0 && y < 0.1) {
                T[i][j] = NAN; // Outside the object
            }
            else {
                T[i][j] = T0; // Interior of the body
            }
            T2[i][j] = T[i][j];
        }
    }

    // Time stepping function for temperature update
    auto step = [=, &x, &y](double T[I][J], double T2[I][J]) {
        for (int i = 0; i < I; i++) {
            x = i * dx;
            for (int j = 0; j < J; j++) {
                y = j * dy;
                if (y == 0.1 && x > 0 && x < 0.4) {
                    // Interaction with fluid
                    T2[i][j] = fo * (2 * T[i][j + 1] + T[i - 1][j] + T[i + 1][j] + 2 * bi * Tf)
                               + (1 - 4 * fo - 2 * bi * fo) * T[i][j];
                }
                else if (T[i][j] == Tc || T[i][j] == Tz || isnan(T[i][j])) {
                    // Keeping boundary and outside points unchanged
                    T2[i][j] = T[i][j];
                }
                else {
                    // Internal points update
                    T2[i][j] = fo * (T[i + 1][j] + T[i - 1][j] + T[i][j - 1] + T[i][j + 1])
                               + (1 - 4 * fo) * T[i][j];
                }
            }
        }
    };

    // Writing temperature data to file
    ofstream fout("temp.dat");

    for (t = 0; t <= tend; t += dt) {
        x = 0;
        for (int i = 0; i < I; i++) {
            y = 0;
            for (int j = 0; j < J; j++) {
                if (t >= 0.02 * tend - dt / 2 && t <= 0.02 * tend + dt / 2) {
                    fout << x << "\t" << y << "\t" << T2[i][j] << endl;
                }
                T[i][j] = T2[i][j]; // Update temperature matrix
                y += dy;
            }
            if (t >= 0.02 * tend - dt / 2 && t <= 0.02 * tend + dt / 2) {
                fout << endl;
            }
            x += dx;
        }
        step(T, T2); // Perform time step update
    }

    fout.close();
    return 0;
}
