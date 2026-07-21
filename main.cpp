#include <iostream>
#include <iomanip>
#include <cmath>

// Celestial Calc — Step 1: Julian Day Calculation
// 
// Converts a Gregorian calendar date/time (UTC) to Julian Day Number (JD)
//
// Julian Day = total days elapsed since January 1, 4713 BC at 12:00 UTC
// Verification: 2000-01-01 12:00:00 UTC → JD 2451545.0 (J2000.0 epoch)

/**
 * Convert Gregorian date/time (UTC) to Julian Day.
 *
 * @param year   Calendar year
 * @param month  Month (1–12)
 * @param day    Day of month
 * @param hour   Hour, UTC (0–23)
 * @param min    Minute
 * @param sec    Second (may be fractional)
 * @return       Julian Day (fractional part encodes time of day)
 */
double toJulianDay(int year, int month, int day,
                   int hour, int min, double sec) {
    if (month <= 2) {
        year  -= 1;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + A / 4;

    double dayFrac = (hour + min / 60.0 + sec / 3600.0) / 24.0;

    return std::floor(365.25  * (year + 4716))
         + std::floor(30.6001 * (month + 1))
         + day + dayFrac + B - 1524.5;
}
/**
 * Julian Day → Greenwich Mean Sidereal Time (degrees, 0–360)
 * Formula: Meeus, Astronomical Algorithms, Ch. 12
 */
double toGMST(double jd) {
    double T = (jd - 2451545.0) / 36525.0; // Julian centuries since J2000.0
    double gmst = 280.46061837
                + 360.98564736629 * (jd - 2451545.0)
                + 0.000387933    * T * T
                - (T * T * T)   / 38710000.0;
    gmst = std::fmod(gmst, 360.0);
    if (gmst < 0.0) gmst += 360.0;
    return gmst;
}
 
/**
 * Julian Day + observer longitude → Local Sidereal Time (degrees, 0–360)
 * @param longitude  East positive, West negative (e.g. Hilo = -155.09)
 */
double toLST(double jd, double longitude) {
    double lst = toGMST(jd) + longitude;
    lst = std::fmod(lst, 360.0);
    if (lst < 0.0) lst += 360.0;
    return lst;
}
 
/** Convert decimal degrees to h m s string */
void printHms(double deg) {
    double h   = deg / 15.0;
    int hours  = static_cast<int>(h);
    double m   = (h - hours) * 60.0;
    int mins   = static_cast<int>(m);
    double sec = (m - mins) * 60.0;
    std::cout << std::setw(2) << std::setfill('0') << hours << "h "
              << std::setw(2) << std::setfill('0') << mins  << "m "
              << std::fixed << std::setprecision(1) << sec  << "s";
}
 
int main() {
    std::cout << std::fixed << std::setprecision(6);
 
    // --- Verification: J2000.0 ---
    double jd_j2000 = toJulianDay(2000, 1, 1, 12, 0, 0.0);
    double gmst_j2000 = toGMST(jd_j2000);
    std::cout << "[Verification] 2000-01-01 12:00:00 UTC\n";
    std::cout << "  JD        = " << jd_j2000  << "  ";
    std::cout << (std::abs(jd_j2000 - 2451545.0) < 1e-9 ? "[OK]" : "[FAIL]") << "\n";
    std::cout << "  GMST      = " << gmst_j2000 << "°  ";
    std::cout << (std::abs(gmst_j2000 - 280.46061837) < 0.0001 ? "[OK]" : "[FAIL]") << "\n\n";
 
    // --- Interactive input ---
    int    y, mo, d, h, mi;
    double s, lon;
 
    std::cout << "Enter date and time (UTC):\n";
    std::cout << "  Year      : "; std::cin >> y;
    std::cout << "  Month     : "; std::cin >> mo;
    std::cout << "  Day       : "; std::cin >> d;
    std::cout << "  Hour      : "; std::cin >> h;
    std::cout << "  Minute    : "; std::cin >> mi;
    std::cout << "  Second    : "; std::cin >> s;
    std::cout << "  Longitude : "; std::cin >> lon;
    std::cout << "  (East = positive, West = negative. Hilo HI = -155.09)\n\n";
 
    double jd   = toJulianDay(y, mo, d, h, mi, s);
    double gmst = toGMST(jd);
    double lst  = toLST(jd, lon);
 
    std::cout << "  Julian Day : " << jd   << "\n";
    std::cout << "  GMST       : " << gmst << "°\n";
    std::cout << "  LST        : " << lst  << "°  (";
    printHms(lst);
    std::cout << ")\n";
 
    return 0;
}