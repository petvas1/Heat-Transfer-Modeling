# Heat Transfer Modeling in a Glass Body

## Project Overview
This project simulates heat transfer in a glass body using numerical methods in C++. The objective is to model the evolution of temperature over time within a 60 × 60 cm solid body, considering different boundary conditions and heat transfer interactions.

## Problem Definition
A cross-section of an infinite rectangular solid is considered, with a grid size of 10 cm (resulting in a 6 × 6 grid). The simulation begins with an initial temperature of **55°C** throughout the body. The boundary conditions are:
- **Black walls**: Fixed temperature at **15°C**
- **Green walls**: Fixed temperature at **45°C**
- **Red walls**: In contact with a fluid with properties:
  - **Heat transfer coefficient (α_f)**: 60 W/m²/K
  - **Fluid temperature (T_f)**: 30°C
- **Conflict points (junctions between boundaries)**:
  - If both boundary conditions have fixed temperatures, the arithmetic mean of the two temperatures is used.
  - If one condition has a fixed temperature and the other is in contact with the fluid, the fixed temperature is used.

## Material Properties
The body is made of **glass**, with the following properties:
- Density (ρ) = 2500 kg/m³
- Thermal conductivity (λ) = 1.05 W/m/K
- Specific heat capacity (c_p) = 840 J/kg/K

## Computational Setup
- **Fourier number** (Fo) = 0.19
- **Grid step size**: dx = dy = 0.5 cm
- **Thermal diffusivity**: a = λ / (ρ * c_p) = 5 × 10⁻⁷ m²/s
- **Biot number**: Bi = (α_f * dx) / λ = 0.2857
- **Time step**: dt = (Fo * dx²) / a = 9.5 s
- **Grid resolution**: I = J = (60 cm) / dx + 1

## Governing Equations
For the temperature at a given grid point (i, j) at time step **n+1**:
- **For internal points:**
  
  \[ T_{i,j}^{n+1} = Fo (T_{i+1,j}^{n} + T_{i-1,j}^{n} + T_{i,j+1}^{n} + T_{i,j-1}^{n}) + T_{i,j}^{n} (1 - 4 Fo) \]

  (This satisfies the stability criterion: **Fo ≤ 0.25**)

- **For red wall points (in contact with fluid):**
  
  \[ T_{i,j}^{n+1} = Fo(2T_{i,j+1}^{n} + T_{i-1,j}^{n} + T_{i+1,j}^{n} + 2BiT_f) + T_{i,j}^{n} (1 - 4Fo - 2BiFo) \]
  
  (This satisfies the stability criterion: **Fo(2 + Bi) ≤ 0.5**)

- **For all other boundary points, the temperature remains fixed.**

## Results and Discussion
### Temperature Evolution at Point (x = 40 cm, y = 30 cm)
The most significant temperature drop occurs within the first **three hours**, due to the high temperature difference between the body and the environment. After **10 hours**, the temperature decreases more slowly, stabilizing at approximately **31.2°C** after **20 hours**. The slow cooling rate is mainly due to the low thermal conductivity of glass.

### Thermal Profiles Over Time
1. **t = 0.4 h**: Initial cooling starts, most pronounced at black walls and the lower boundary in contact with the fluid.
2. **t = 1 h**: Further cooling is observed, particularly in the lower right corner, while the center retains its initial temperature.
3. **t = 2 h**: The remaining heat is concentrated around the green walls.
4. **t = 5 h**: Most of the body has cooled, with heat spreading from the hot walls to the cold ones.
5. **t = 10 h**: The core temperature stabilizes around **30°C**.
6. **t = 20 h**: Full stabilization, with heat transfer occurring between hot walls, fluid contact points, and cold walls.

A **time-lapse simulation** of the temperature evolution is also available.

## Source Code (C++)
The implementation follows these steps:
1. Define necessary variables and constants.
2. Initialize the temperature distribution according to the geometry.
3. Implement a function to compute one time step using the above equations.
4. Use a time loop to iterate through steps and record results.

See the `main.cpp` file in this repository for the full implementation.
