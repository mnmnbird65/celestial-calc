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

int main() {
    std::cout << std::fixed << std::setprecision(6);

    // --- Verification against known epoch ---
    double jd_j2000 = toJulianDay(2000, 1, 1, 12, 0, 0.0);
    std::cout << "[Verification] 2000-01-01 12:00:00 UTC\n";
    std::cout << "  JD       = " << jd_j2000 << "\n";
    std::cout << "  Expected : 2451545.000000  ";
    std::cout << (std::abs(jd_j2000 - 2451545.0) < 1e-9
                  ? "[OK]" : "[FAIL]") << "\n\n";

    // input
    int y; // year
    int mo; // month
    int d; // day
    int h; // hour 
    int mi; // minute
    double s; // second

    std::cout << "Enter date and time (UTC):\n";
    std::cout << "  Year   : "; std::cin >> y;
    std::cout << "  Month  : "; std::cin >> mo;
    std::cout << "  Day    : "; std::cin >> d;
    std::cout << "  Hour   : "; std::cin >> h;
    std::cout << "  Minute : "; std::cin >> mi;
    std::cout << "  Second : "; std::cin >> s;

    std::cout << "\nJulian Day = " << toJulianDay(y, mo, d, h, mi, s) << "\n";
    return 0;
}
