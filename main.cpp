#include <iostream>
#include <iomanip>
#include <cmath>

// =====================================================
// Celestial Calc — Step 3: Altitude / Azimuth
// =====================================================
// Full pipeline: date/time → JD → LST → Alt/Az
//
// Given a star's catalog position (RA, Dec) and the
// observer's location (lat, lon) and time, computes
// the altitude (angle above horizon) and azimuth
// (compass direction) needed to point a telescope.
//
// Verification (Sirius from Hilo HI, 2000-01-01 12:00 UTC):
//   RA = 101.287°, Dec = -16.716°
//   Lat = 19.70°,  Lon = -155.09°
//   Expected: Alt ≈ -37°  (below horizon at this time)
// =====================================================

const double PI = M_PI;
double toRad(double d) { return d * PI / 180.0; }
double toDeg(double r) { return r * 180.0 / PI; }

/** Gregorian date/time (UTC) → Julian Day */
double toJulianDay(int year, int month, int day,
                   int hour, int min, double sec) {
    if (month <= 2) { year -= 1; month += 12; }
    int A = year / 100, B = 2 - A + A / 4;
    double dayFrac = (hour + min / 60.0 + sec / 3600.0) / 24.0;
    return std::floor(365.25 * (year + 4716))
         + std::floor(30.6001 * (month + 1))
         + day + dayFrac + B - 1524.5;
}

/** Julian Day → GMST (degrees, 0–360) */
double toGMST(double jd) {
    double T = (jd - 2451545.0) / 36525.0;
    double g = 280.46061837 + 360.98564736629 * (jd - 2451545.0)
             + 0.000387933 * T * T - (T * T * T) / 38710000.0;
    return std::fmod(std::fmod(g, 360.0) + 360.0, 360.0);
}

/** Julian Day + longitude → LST (degrees, 0–360) */
double toLST(double jd, double longitude) {
    double lst = toGMST(jd) + longitude;
    return std::fmod(std::fmod(lst, 360.0) + 360.0, 360.0);
}

struct HorizCoord { double alt, az, H; };

/**
 * Equatorial → Horizontal coordinate transform
 * @param ra   Right Ascension (degrees)
 * @param dec  Declination (degrees)
 * @param lst  Local Sidereal Time (degrees)
 * @param lat  Observer latitude (degrees, N positive)
 * @return     { alt, az, H } all in degrees
 */
HorizCoord toAltAz(double ra, double dec, double lst, double lat) {
    double H   = std::fmod(std::fmod(lst - ra, 360.0) + 360.0, 360.0); // hour angle
    double h   = toRad(H), d = toRad(dec), L = toRad(lat);

    double sinAlt = std::sin(d) * std::sin(L) + std::cos(d) * std::cos(L) * std::cos(h);
    double alt    = toDeg(std::asin(sinAlt));

    double cosAlt = std::cos(toRad(alt));
    double cosAz  = (std::sin(d) - std::sin(toRad(alt)) * std::sin(L))
                  / (cosAlt * std::cos(L));
    cosAz = std::max(-1.0, std::min(1.0, cosAz)); // clamp for floating-point safety
    double az = toDeg(std::acos(cosAz));
    if (std::sin(h) > 0) az = 360.0 - az;         // correct quadrant

    return { alt, az, H };
}

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
    std::cout << std::fixed << std::setprecision(4);

    // --- Verification ---
    {
        double jd  = toJulianDay(2000, 1, 1, 12, 0, 0.0);
        double lst = toLST(jd, -155.09);
        auto   r   = toAltAz(101.287, -16.716, lst, 19.70);
        std::cout << "[Verification] Sirius from Hilo HI, 2000-01-01 12:00 UTC\n";
        std::cout << "  LST : "; printHms(lst); std::cout << "\n";
        std::cout << "  Alt : " << r.alt << "°\n";
        std::cout << "  Az  : " << r.az  << "°\n\n";
    }

    // --- Interactive input ---
    int y, mo, d, h, mi; double s, ra, dec, lat, lon;
    std::cout << "Enter date and time (UTC):\n";
    std::cout << "  Year       : "; std::cin >> y;
    std::cout << "  Month      : "; std::cin >> mo;
    std::cout << "  Day        : "; std::cin >> d;
    std::cout << "  Hour       : "; std::cin >> h;
    std::cout << "  Minute     : "; std::cin >> mi;
    std::cout << "  Second     : "; std::cin >> s;
    std::cout << "Enter target (catalog coordinates):\n";
    std::cout << "  RA  (deg)  : "; std::cin >> ra;
    std::cout << "  Dec (deg)  : "; std::cin >> dec;
    std::cout << "Enter observer location:\n";
    std::cout << "  Latitude   : "; std::cin >> lat;
    std::cout << "  Longitude  : "; std::cin >> lon;

    double jd  = toJulianDay(y, mo, d, h, mi, s);
    double lst = toLST(jd, lon);
    auto   r   = toAltAz(ra, dec, lst, lat);

    std::cout << "\n--- Results ---\n";
    std::cout << "  JD         : " << jd    << "\n";
    std::cout << "  LST        : "; printHms(lst); std::cout << "\n";
    std::cout << "  Hour Angle : " << r.H   << "°\n";
    std::cout << "  Altitude   : " << r.alt << "°" << (r.alt > 0 ? "  (above horizon)" : "  (below horizon)") << "\n";
    std::cout << "  Azimuth    : " << r.az  << "°\n";
    return 0;
}