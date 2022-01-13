#include <cstdint>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <codecvt>
#include <chrono>
#include <time.h>

template <int SECONDS>
using RoundDuration = std::chrono::duration<
        long, std::ratio_multiply<std::ratio<SECONDS>, std::chrono::seconds::period>>;

std::chrono::system_clock::duration nextTimeRounded(std::chrono::system_clock::time_point tp,
                                                    const std::chrono::seconds& interval)
{
    // auto tp = std::chrono::system_clock::now();
    constexpr auto timeFmt = "%T";

    auto tpNext = tp + interval;
    switch (interval.count())
    {
        case 300:
            tpNext = std::chrono::ceil<RoundDuration<300>>(tp);
            break;
        case 600:
            tpNext = std::chrono::ceil<RoundDuration<600>>(tp);
            break;
        case 900:
            tpNext = std::chrono::ceil<RoundDuration<900>>(tp);
            break;
        default:
            std::cout << "Unexpected interval: " << interval.count() << std::endl;
    };

    auto nextD = interval - (tp.time_since_epoch() % interval);

    auto lt = std::chrono::system_clock::to_time_t(tp);
    auto ltNext = std::chrono::system_clock::to_time_t(tpNext);
    std::cout << "Time: " << std::put_time(std::localtime(&lt), timeFmt) << " -> "
              << std::put_time(std::localtime(&ltNext), timeFmt) << " " << interval.count()
              << "s round."
              << " nextD: " << std::chrono::duration_cast<std::chrono::seconds>(nextD).count()
              << std::endl;

    return tpNext - tp;
}

std::string formatDuration(std::chrono::system_clock::duration d)
{
    auto h = std::chrono::duration_cast<std::chrono::hours>(d);
    auto m = std::chrono::duration_cast<std::chrono::minutes>(d);
    auto s = std::chrono::round<std::chrono::seconds>(d);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << h.count() << ":" << std::setfill('0') << std::setw(2)
       << (m - h).count() << ":" << std::setfill('0') << std::setw(2) << (s - m).count();
    return ss.str();
}

/** Set \a hour, \a minute, \a second for passed time_point.
 */
std::chrono::system_clock::time_point setTime(std::chrono::system_clock::time_point tp, int hour,
                                              int minute, int second)
{
    time_t tTp = std::chrono::system_clock::to_time_t(tp);
    auto tmTp = std::localtime(&tTp);
    tmTp->tm_hour = hour;
    tmTp->tm_min = minute;
    tmTp->tm_sec = second;
    return std::chrono::system_clock::from_time_t(std::mktime(tmTp));
}

void durations(std::chrono::system_clock::time_point tp, std::chrono::system_clock::duration d)
{
    constexpr auto timeFmt = "%FT%T";

    auto lt = std::chrono::system_clock::to_time_t(tp);

    std::cout << "TimePoint: " << tp.time_since_epoch().count()
              << " fmt: " << std::put_time(std::localtime(&lt), timeFmt) << std::endl
              << "duration: " << std::chrono::duration_cast<std::chrono::seconds>(d).count() << ", "
              << formatDuration(d) << std::endl;

    std::chrono::seconds dayInS = std::chrono::hours(24);

    auto midnight = setTime(tp, 0, 0, 0);

    // Duration cast for days yelds time at hour 01:00:00 instead of 00:00:00 and cannot find what's
    // I'm doing wrong.
    // using days =
    //         std::chrono::duration<int,
    //                               std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
    // using sys_days = std::chrono::time_point<std::chrono::system_clock, days>;

    // auto today = std::chrono::floor<days>(std::chrono::system_clock::now());
    // time_t tToday = std::chrono::system_clock::to_time_t(today);
    // std::cout << "today: " << std::put_time(std::localtime(&tToday), timeFmt) << std::endl;
    // std::cout << "days(1): " << days(1).count()
    //           << " hours: " << std::chrono::duration_cast<std::chrono::hours>(days(1)).count()
    //           << " minutes: " << std::chrono::duration_cast<std::chrono::minutes>(days(1)).count()
    //           << " seconds: " << std::chrono::duration_cast<std::chrono::seconds>(days(1)).count()
    //           << std::endl;

    auto durationSinceMidnight = (tp - midnight);
    auto durationTillMidnight = dayInS - durationSinceMidnight;

    auto nextD = d - (durationSinceMidnight % d);

    auto tpNext = tp + nextD;

    auto nextEnd = tpNext - midnight;
    auto day1  = std::chrono::hours(24);
    if (nextEnd > day1)
    {
        time_t tNext = std::chrono::system_clock::to_time_t(tpNext);
        std::cout << "tpNext: " << tpNext.time_since_epoch().count() << " "
                  << std::put_time(std::localtime(&tNext), timeFmt) << std::endl
                  << "pass_midnight: " << std::chrono::round<std::chrono::seconds>(nextEnd).count()
                  << std::endl;
        tpNext = midnight + std::chrono::hours(24);
    }

    auto ltD = std::chrono::system_clock::to_time_t(tpNext);

    std::cout << "  from_midnight: " << durationSinceMidnight.count()
              << " t: " << formatDuration(durationSinceMidnight) << ", hours: "
              << std::chrono::duration_cast<std::chrono::hours>(durationSinceMidnight).count()
              << ", minutes: "
              << std::chrono::duration_cast<std::chrono::minutes>(durationSinceMidnight).count()
              << ", seconds: "
              << std::chrono::round<std::chrono::seconds>(durationSinceMidnight).count()
              << std::endl
              << "  next_midnight: " << durationTillMidnight.count()
              << " t: " << formatDuration(durationTillMidnight) << ", hours: "
              << std::chrono::duration_cast<std::chrono::hours>(durationTillMidnight).count()
              << ", minutes: "
              << std::chrono::duration_cast<std::chrono::minutes>(durationTillMidnight).count()
              << ", seconds: "
              << std::chrono::round<std::chrono::seconds>(durationTillMidnight).count() << std::endl
              << "  nextD: " << nextD.count() << " t: " << formatDuration(nextD)
              << ", hours: " << std::chrono::duration_cast<std::chrono::hours>(nextD).count()
              << ", minutes: " << std::chrono::duration_cast<std::chrono::minutes>(nextD).count()
              << ", seconds: " << std::chrono::round<std::chrono::seconds>(nextD).count()
              << std::endl
              << "Time: " << std::put_time(std::localtime(&lt), timeFmt) << " -> "
              << std::put_time(std::localtime(&ltD), timeFmt)
              << " seconds: " << std::chrono::round<std::chrono::seconds>(d).count() << "."
              << std::endl
              << "----------------------------------------" << std::endl;
}



int main()
{
    // std::time_t t = std::time(nullptr);
    std::wbuffer_convert<std::codecvt_utf8<wchar_t>> conv(std::cout.rdbuf());
    std::wostream out(&conv);
    // out.imbue(std::locale("pl_PL.utf8"));
    // out.imbue(std::locale("C"));
    // // this I/O manipulator std::put_time uses std::time_put<wchar_t>
    // constexpr auto timeFmt = L"%FT%T%z";
    // out << L"\nlocaltime: " << std::put_time(std::localtime(&t), timeFmt) << '\n';
    // out << L"\ngmtime: " << std::put_time(std::gmtime(&t), timeFmt) << '\n';

    // out << "DUPA BLADA" << std::endl;

    // using TP = std::chrono::system_clock::time_point;
    // using SC = TP::clock;
    // TP tp;
    // auto tpNow = SC::now();

    // auto d4s{std::chrono::seconds(4)};

    // out << "D: " << d4s.count() << std::endl;

    // out << "TP: " << std::chrono::system_clock::to_time_t(tp)
    //     << " +4s: " << std::chrono::system_clock::to_time_t(tp + d4s) << std::endl;

    // out << "TP NOW: " << std::chrono::system_clock::to_time_t(tpNow)
    //     << " +4s: " << std::chrono::system_clock::to_time_t(tpNow + d4s) << std::endl;

    // using quarter = std::chrono::duration<
    //         long, std::ratio_multiply<std::ratio<15>, std::chrono::minutes::period>>;

    // auto minutes15 = quarter(1);

    // constexpr auto hourFmt = L"%T";
    // auto lt = SC::to_time_t(tpNow + minutes15);
    // out << "TP 15 minutes: " << std::chrono::duration_cast<quarter>(minutes15).count() << " -> "
    //     << std::put_time(std::localtime(&lt), hourFmt) << " quarters." << std::endl;

    // out << "TP NOW: " << std::chrono::duration_cast<quarter>(tpNow.time_since_epoch()).count()
    //     << " quarters." << std::endl;

    // auto interval = std::chrono::minutes(15);
    // auto roundQ = TP(std::chrono::duration_cast<quarter>(tpNow.time_since_epoch()));
    // lt = SC::to_time_t(roundQ);
    // auto ltNext{SC::to_time_t(roundQ + interval)};
    // out << "TP NOW: " << roundQ.time_since_epoch().count() << " -> "
    //     << TP(roundQ + interval).time_since_epoch().count() << " : "
    //     << std::put_time(std::localtime(&lt), hourFmt) << " -> "
    //     << std::put_time(std::localtime(&ltNext), hourFmt) << " 15 m round." << std::endl;

    // using DCastType = std::chrono::seconds;

    // out << "------------------------------" << std::endl;
    // auto dNext =
    //         nextTimeRounded(tpNow, std::chrono::duration_cast<std::chrono::seconds>(quarter(1)));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // out << "------------------------------" << std::endl;
    // dNext = nextTimeRounded(tpNow, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // out << "------------------------------" << std::endl;
    // dNext = nextTimeRounded(tpNow, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tpNow, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // auto tTime = TP(std::chrono::hours(10) + std::chrono::minutes(2));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(5));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(8));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(10));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(11));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(15));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(33));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(44));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(56));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<DCastType>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    // tTime = TP(std::chrono::hours(10) + std::chrono::minutes(56) + std::chrono::seconds(33));
    // lt = SC::to_time_t(tTime);
    // out << "Test Time: " << std::put_time(std::localtime(&lt), hourFmt) << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(300));
    // out << " Next: " << std::chrono::duration_cast<std::chrono::seconds>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(600));
    // out << " Next: " << std::chrono::duration_cast<std::chrono::seconds>(dNext).count()
    //     << std::endl;

    // dNext = nextTimeRounded(tTime, std::chrono::seconds(900));
    // out << " Next: " << std::chrono::duration_cast<std::chrono::seconds>(dNext).count()
    //     << std::endl;
    // out << "------------------------------" << std::endl;

    using MeasTime = std::chrono::system_clock::time_point;
    using MeasInterval = MeasTime::duration;

    MeasTime timeNow{MeasTime::clock::now()};
    MeasInterval inte{std::chrono::seconds{300}};
    MeasInterval nextMeas {inte - timeNow.time_since_epoch() % inte};

    out << "timeNow:" << timeNow.time_since_epoch().count() << ", interval:" << inte.count()
        << "(seconds:" << std::chrono::duration_cast<std::chrono::seconds>(inte).count() << ")"
        << ", next:" << nextMeas.count()
        << "(seconds:" << std::chrono::duration_cast<std::chrono::seconds>(nextMeas).count() << ")"
        << std::endl;

#if 0
    auto us100 = std::chrono::microseconds{100};
    auto us499k = std::chrono::microseconds{499'000};
    auto us500k = std::chrono::microseconds{500'000};
    auto us501k = std::chrono::microseconds{501'000};
    auto us999k = std::chrono::microseconds{999'000};
    auto us1M = std::chrono::microseconds{1'000'000};
    auto s1 = std::chrono::seconds{1};
    auto s0 = std::chrono::seconds{0};

    out << "us100: " << us100.count()
        << " in s: " << std::chrono::duration_cast<std::chrono::seconds>(us100).count()
        << std::endl;
    out << "us499k: " << us499k.count()
        << " in s: " << std::chrono::duration_cast<std::chrono::seconds>(us499k).count()
        << std::endl;
    out << "us500k: " << us500k.count()
        << " in s: " << std::chrono::duration_cast<std::chrono::seconds>(us500k).count()
        << std::endl;
    out << "us501k: " << us501k.count()
        << " in s: " << std::chrono::duration_cast<std::chrono::seconds>(us501k).count()
        << std::endl;

    out << "us999k: " << us999k.count()
        << " in s: " << std::chrono::duration_cast<std::chrono::seconds>(us999k).count()
        << std::endl;

    out << "us1M: " << us1M.count()
        << " in s: " << std::chrono::duration_cast<std::chrono::seconds>(us1M).count()
        << std::endl;

    out << "s1: " << s1.count()
        << " in us: " << std::chrono::duration_cast<std::chrono::microseconds>(s1).count()
        << std::endl;

    out << "s0: " << s0.count() << std::endl;

    out << "us100 == s1: " << std::boolalpha << (us100 == s1) << std::endl;
    out << "us100 == s0: " << std::boolalpha << (us100 == s0) << std::endl;

    out << "<as s>(us100) == s0: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us100) == s0) << std::endl;

    out << "<as s>(us499k) == s0: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us499k) == s0) << std::endl;

    out << "<as s>(us499k) == s1: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us499k) == s1) << std::endl;

    out << "<as s>(us500k) == s0: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us500k) == s0) << std::endl;

    out << "<as s>(us500k) == s1: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us500k) == s1) << std::endl;

    out << "<as s>(us501k) == s0: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us501k) == s0) << std::endl;

    out << "<as s>(us501k) == s1: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us501k) == s1) << std::endl;

    out << "<as s>(us999k) == s0: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us999k) == s0) << std::endl;

    out << "<as s>(us999k) == s1: " << std::boolalpha
        << (std::chrono::duration_cast<std::chrono::seconds>(us999k) == s1) << std::endl;

    out << "us100 == us1M: " << std::boolalpha << (us100 == us1M) << std::endl;
    out << "us1M == s1: " << std::boolalpha << (us1M == s1) << std::endl;
    out << "s0 == s1: " << std::boolalpha << (s0 == s1) << std::endl;
#endif // 0

    auto const minute = std::chrono::minutes(1);

    out << "========================================" << std::endl;
    durations(timeNow, std::chrono::seconds(300));
    durations(timeNow, std::chrono::seconds(600));
    durations(timeNow, std::chrono::seconds(900));
    durations(timeNow, std::chrono::seconds(1800));
    durations(timeNow, std::chrono::hours(1));
    durations(timeNow, std::chrono::hours(2));
    durations(timeNow, std::chrono::hours(3));
    durations(timeNow, std::chrono::hours(4));
    durations(timeNow, std::chrono::hours(6));

    out << "____________________________________________________________" << std::endl;
    std::chrono::system_clock::duration testDuration = std::chrono::hours(8);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 8, 0, 1), testDuration);
    durations(setTime(timeNow, 16, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    out << "____________________________________________________________" << std::endl;

    durations(timeNow, std::chrono::hours(12));
    durations(timeNow, std::chrono::hours(24));

    out << "____________________________________________________________" << std::endl;

    durations(timeNow, std::chrono::hours(5));
    durations(timeNow, std::chrono::hours(7));

    out << "____________________________________________________________" << std::endl;
    testDuration = std::chrono::hours(9);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 9, 0, 1), testDuration);
    durations(setTime(timeNow, 18, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 4, 0, 0), testDuration);

    out << "____________________________________________________________" << std::endl;
    testDuration = std::chrono::hours(10);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 10, 0, 1), testDuration);
    durations(setTime(timeNow, 20, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 6, 0, 0), testDuration);

    out << "____________________________________________________________" << std::endl;
    testDuration = std::chrono::hours(11);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 11, 0, 1), testDuration);
    durations(setTime(timeNow, 22, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 9, 0, 0), testDuration);

    out << "____________________________________________________________" << std::endl;
    durations(timeNow, std::chrono::hours(13));
    durations(timeNow, std::chrono::hours(14));
    durations(timeNow, std::chrono::hours(15));
    durations(timeNow, std::chrono::hours(16));
    durations(timeNow, std::chrono::hours(17));
    out << "____________________________________________________________" << std::endl;
    testDuration = std::chrono::hours(18);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 18, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 12, 0, 0), testDuration);
    out << "____________________________________________________________" << std::endl;
    testDuration = std::chrono::hours(19);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 19, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 3, 59, 59), testDuration);
    durations(setTime(timeNow, 4, 0, 0), testDuration);
    durations(setTime(timeNow, 4, 0, 1), testDuration);
    out << "____________________________________________________________" << std::endl;
    durations(timeNow, std::chrono::hours(20));
    durations(timeNow, std::chrono::hours(21));
    durations(timeNow, std::chrono::hours(22));
    durations(timeNow, std::chrono::hours(23));

    out << "____________________________________________________________" << std::endl;
    testDuration = std::chrono::minutes(90);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 19, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 3, 59, 59), testDuration);
    durations(setTime(timeNow, 4, 0, 0), testDuration);
    durations(setTime(timeNow, 22, 29, 59), testDuration);
    durations(setTime(timeNow, 22, 30, 0), testDuration);
    durations(setTime(timeNow, 22, 30, 1), testDuration);
    out << "____________________________________________________________" << std::endl;

    testDuration = std::chrono::minutes(100);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 19, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 3, 59, 59), testDuration);
    durations(setTime(timeNow, 4, 0, 0), testDuration);
    durations(setTime(timeNow, 23, 19, 59), testDuration);
    durations(setTime(timeNow, 23, 20, 0), testDuration);
    durations(setTime(timeNow, 23, 21, 1), testDuration);
    out << "____________________________________________________________" << std::endl;

    testDuration = std::chrono::seconds(5555);
    durations(timeNow, testDuration);
    durations(setTime(timeNow, 0, 0, 0), testDuration);
    durations(setTime(timeNow, 0, 0, 1), testDuration);
    durations(setTime(timeNow, 19, 0, 1), testDuration);
    durations(setTime(timeNow, 23, 59, 59), testDuration);
    durations(setTime(timeNow, 3, 59, 59), testDuration);
    durations(setTime(timeNow, 4, 0, 0), testDuration);
    durations(setTime(timeNow, 22, 20, 21), testDuration);
    durations(setTime(timeNow, 23, 8, 40), testDuration);
    durations(setTime(timeNow, 23, 8, 45), testDuration);
    durations(setTime(timeNow, 23, 8, 50), testDuration);
    out << "____________________________________________________________" << std::endl;

    durations(timeNow, minute);
}
