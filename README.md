<<<<<<< HEAD
# celestial-calc
celestial location calculation tool
=======
# Celestial Calc 🔭

An astronomical position calculator for telescope control systems — computing the **altitude and azimuth** of celestial objects from any date, time, and observer location. Implemented in both **C++** and **JavaScript** using identical algorithms.

## Live Demo

> *(Paste your GitHub Pages URL here after deploying)*

---

## Background

Automatic telescope tracking requires the control system to continuously compute a target object's position in the sky — expressed as azimuth (compass direction) and altitude (angle above horizon). This project builds that computation engine step by step, with telescope control as the final application.

---

## Development Roadmap

| Step | Description | Status |
|------|-------------|--------|
| 1 | **Julian Day (JD)** — universal time reference for astronomical calculations | ✅ Complete |
| 2 | **Local Sidereal Time (LST)** — which right ascension is on the meridian | 🔲 Upcoming |
| 3 | **Coordinate Transform** — equatorial (RA/Dec) → horizontal (Az/Alt) | 🔲 Upcoming |
| 4 | **Stellar Verification** — cross-check computed positions against Stellarium | 🔲 Upcoming |
| 5 | **Planetary Positions** — derive planet locations from orbital elements | 🔲 Upcoming |

---

## Step 1: Julian Day

Gregorian calendars are inconvenient for computation (variable month lengths, leap years, etc.). Astronomy uses **Julian Day (JD)** instead — the total number of days elapsed since January 1, 4713 BC at 12:00 UTC. Every calculation in this tool derives from JD.

**Known reference point (J2000.0 epoch):**
```
2000 Jan 1, 12:00:00 UTC  →  JD 2451545.0
```

### Build & Run (C++)

```bash
g++ -o celestial main.cpp
./celestial
```

### Algorithm (shared between C++ and JavaScript)

```cpp
double toJulianDay(int year, int month, int day,
                   int hour, int min, double sec) {
    if (month <= 2) { year -= 1; month += 12; }
    int A = year / 100;
    int B = 2 - A + A / 4;
    double dayFrac = (hour + min / 60.0 + sec / 3600.0) / 24.0;
    return floor(365.25  * (year + 4716))
         + floor(30.6001 * (month + 1))
         + day + dayFrac + B - 1524.5;
}
```

*Reference: Jean Meeus, Astronomical Algorithms, 2nd ed., Chapter 7.*

---

## Tech Stack

| | |
|-|-|
| Languages | C++, HTML, CSS, JavaScript |
| Reference | Jean Meeus, *Astronomical Algorithms* (2nd ed.) |
| Dev environment | VS Code / g++ |

---

## Author

University of Hawaiʻi at Hilo — Computer Science (Astronomy Minor)  
Student Assistant, [PISCES](https://www.pisces.hawaii.edu/) — Pacific International Space Center for Exploration Systems
>>>>>>> fddc6dc6a9bd0aed3357d7b6fd69b2fd76318219
