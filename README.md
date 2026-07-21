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
| 2 | **Local Sidereal Time (LST)** — which right ascension is on the meridian | ✅ Complete |
| 3 | **Coordinate Transform** — equatorial (RA/Dec) → horizontal (Az/Alt) | ✅ Complete |
| 4 | **Stellar Verification** — cross-check computed positions against Stellarium | 🔲 Upcoming |
| 5 | **Planetary Positions** — derive planet locations from orbital elements | 🔲 Upcoming |

---

## Algorithm Pipeline

```
Date / Time (UTC)
      ↓
Julian Day (JD)          — Step 1
      ↓
Greenwich Mean Sidereal Time (GMST)
      ↓
Local Sidereal Time (LST)  — Step 2
      ↓
Hour Angle  H = LST − RA
      ↓
Altitude / Azimuth         — Step 3
```

---

## Step 1: Julian Day

```cpp
double toJulianDay(int year, int month, int day,
                   int hour, int min, double sec) {
    if (month <= 2) { year -= 1; month += 12; }
    int A = year / 100, B = 2 - A + A / 4;
    double dayFrac = (hour + min / 60.0 + sec / 3600.0) / 24.0;
    return floor(365.25 * (year + 4716))
         + floor(30.6001 * (month + 1))
         + day + dayFrac + B - 1524.5;
}
// Verification: 2000-01-01 12:00 UTC → JD 2451545.0
```

## Step 2: Local Sidereal Time

```cpp
double toGMST(double jd) {
    double T = (jd - 2451545.0) / 36525.0;
    double g = 280.46061837 + 360.98564736629 * (jd - 2451545.0)
             + 0.000387933 * T * T - (T * T * T) / 38710000.0;
    return fmod(fmod(g, 360.0) + 360.0, 360.0);
}
double toLST(double jd, double longitude) {
    return fmod(fmod(toGMST(jd) + longitude, 360.0) + 360.0, 360.0);
}
// Verification: JD 2451545.0 → GMST ≈ 280.4606°
```

## Step 3: Altitude / Azimuth

```cpp
HorizCoord toAltAz(double ra, double dec, double lst, double lat) {
    double H   = fmod(fmod(lst - ra, 360.0) + 360.0, 360.0);
    double sinAlt = sin(dec)*sin(lat) + cos(dec)*cos(lat)*cos(H);
    double alt    = asin(sinAlt);
    double cosAz  = (sin(dec) - sin(alt)*sin(lat)) / (cos(alt)*cos(lat));
    double az     = acos(clamp(cosAz, -1, 1));
    if (sin(H) > 0) az = 2*PI - az;  // quadrant correction
    return { alt, az, H };            // all in degrees after conversion
}
```

---

## Build & Run (C++)

```bash
g++ -o celestial main.cpp
./celestial
```

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