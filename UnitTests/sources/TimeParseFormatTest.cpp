
#include "Mona/TimeParser.h"
#include "TimeParseFormatTest.h"
#include "Mona/Logs.h"

using namespace Mona;
using namespace std;

string& TimeParseFormatTest::ToString(const string& fmt, int tzd /*= Time::UTC*/) {
	
	_out.clear();
	_time.toString(fmt, _out, tzd);
	
	return _out;
}

bool TimeParseFormatTest::IsParseOk(const char * fmt, const char * stDate, int year, int month,
	int day, int hour, int min, int sec, int msec, int microsec) {

	// Parsing
	int tzd = 0;
	bool bIsParseOk = TimeParser::Parse(fmt, stDate, _time, tzd);
	if (!bIsParseOk) {
		DEBUG("Error during parsing of date (", stDate, ")");
		return false;
	}

	struct tm tmdate;
	if (TimeParseFormatTest::VerifyParsing(stDate, year, month, day, hour, min, sec, msec, microsec, tmdate))
		return true;
	else {
		DEBUG((tmdate.tm_year + 1900),",",(tmdate.tm_mon + 1),",",tmdate.tm_mday,",",tmdate.tm_hour
			,",", tmdate.tm_min, ",", tmdate.tm_sec, ",", _time.millisec(), ",", _time.microsec(), " does not correspond to ", stDate, " (", fmt, ")");
		return false;
	}
}

bool TimeParseFormatTest::IsParseOk(const char * stDate, int year, int month, 
	int day, int hour, int min, int sec, int msec, int microsec) {

	// Parsing
	bool bIsParseOk = _time.fromString(stDate);
	if (!bIsParseOk) {
		DEBUG("Error during parsing of date (", stDate, ")");
		return false;
	}

	struct tm tmdate;
	if (TimeParseFormatTest::VerifyParsing(stDate, year, month, day, hour, min, sec, msec, microsec, tmdate))
		return true;
	else {
		DEBUG((tmdate.tm_year + 1900), ",", (tmdate.tm_mon + 1), ",", tmdate.tm_mday, ",", tmdate.tm_hour
			,",", tmdate.tm_min, ",", tmdate.tm_sec, ",", _time.millisec(), ",", _time.microsec(), " does not correspond to ", stDate);
		return false;
	}
}

bool TimeParseFormatTest::VerifyParsing(const char * stDate, int year, int month,
	int day, int hour, int min, int sec, int msec, int microsec, struct tm& tmdate) {

	// Conversion to struct tm
	_time.toGMT(tmdate);

	bool bIsParseOk = (tmdate.tm_year + 1900) == year;
	if (bIsParseOk) bIsParseOk = (tmdate.tm_mon + 1) == month;
	if (bIsParseOk) bIsParseOk = tmdate.tm_mday == day;
	if (bIsParseOk) bIsParseOk = tmdate.tm_hour == hour;
	if (bIsParseOk) bIsParseOk = tmdate.tm_min == min;
	if (bIsParseOk) bIsParseOk = tmdate.tm_sec == sec;
	if (bIsParseOk) bIsParseOk = _time.millisec() == msec;
	if (bIsParseOk) bIsParseOk = _time.microsec() == microsec;

	return bIsParseOk;
}

ADD_TEST(TimeParseFormatTest, TestISO8601) {

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00Z", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::ISO8601_FORMAT) == "2005-01-08T12:30:00Z");

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00+01:00", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::ISO8601_FORMAT, 3600) == "2005-01-08T12:30:00+01:00");

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00-01:00", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::ISO8601_FORMAT, -3600) == "2005-01-08T12:30:00-01:00");

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00", 2005, 1, 8, 12, 30, 0, 0, 0));
	
	EXPECT_TRUE(IsParseOk("2005-01-08", 2005, 1, 8, 0, 0, 0, 0, 0));
}

ADD_TEST(TimeParseFormatTest, TestISO8601Frac) {
	
	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.1Z", 2005, 1, 8, 12, 30, 0, 100, 0));
	
	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.123+01:00", 2005, 1, 8, 12, 30, 0, 123, 0));
	
	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.12345-01:00", 2005, 1, 8, 12, 30, 0, 123, 450));
	
	EXPECT_TRUE(IsParseOk("2010-09-23T16:17:01.2817002+02:00", 2010, 9, 23, 16, 17, 1, 281, 700));
	
	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.123456", 2005, 1, 8, 12, 30, 0, 123, 456));

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.012034", 2005, 1, 8, 12, 30, 0, 12, 34));
	EXPECT_TRUE(ToString(Time::ISO8601_FRAC_FORMAT, 3600) == "2005-01-08T12:30:00.012034+01:00");
	EXPECT_TRUE(ToString(Time::ISO8601_FRAC_FORMAT, -3600) == "2005-01-08T12:30:00.012034-01:00");
}


ADD_TEST(TimeParseFormatTest, TestRFC822) {

	EXPECT_TRUE(IsParseOk("Sat, 8 Jan 05 12:30:00 GMT", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC822_FORMAT) == "Sat, 8 Jan 05 12:30:00 GMT");

	EXPECT_TRUE(IsParseOk("Sat, 8 Jan 05 12:30:00 +0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC822_FORMAT, 3600) == "Sat, 8 Jan 05 12:30:00 +0100");

	EXPECT_TRUE(IsParseOk("Sat, 8 Jan 05 12:30:00 -0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC822_FORMAT, -3600) == "Sat, 8 Jan 05 12:30:00 -0100");

	EXPECT_TRUE(IsParseOk("Tue, 18 Jan 05 12:30:00 CET", 2005, 1, 18, 12, 30, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("Wed, 12 Sep 73 02:01:12 CEST", 1973, 9, 12, 2, 1, 12, 0, 0));

	EXPECT_TRUE(!IsParseOk("12 Sep 73 02:01:12 CEST", 1973, 9, 12, 2, 1, 12, 0, 0));
	EXPECT_TRUE(IsParseOk(Time::RFC822_FORMAT.c_str() , "12 Sep 73 02:01:12 CEST", 1973, 9, 12, 2, 1, 12, 0, 0));
}


ADD_TEST(TimeParseFormatTest, TestRFC1123) {

	EXPECT_TRUE(IsParseOk("Sat, 8 Jan 2005 12:30:00 GMT", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC1123_FORMAT) == "Sat, 8 Jan 2005 12:30:00 GMT");

	EXPECT_TRUE(IsParseOk("Sat, 8 Jan 2005 12:30:00 +0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC1123_FORMAT, 3600) == "Sat, 8 Jan 2005 12:30:00 +0100");

	EXPECT_TRUE(IsParseOk("Sat, 8 Jan 2005 12:30:00 -0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC1123_FORMAT, -3600) == "Sat, 8 Jan 2005 12:30:00 -0100");

	EXPECT_TRUE(!IsParseOk("Sun, 20 Jul 1969 16:17:30 EDT", 1969, 7, 20, 16, 17, 30, 0, 0)); // Year invalid

	EXPECT_TRUE(!IsParseOk("Sun, 20 Jul 1969 16:17:30 GMT+01:00", 1969, 7, 20, 16, 17, 30, 0, 0));  // Year invalid
}


ADD_TEST(TimeParseFormatTest, TestHTTP) {

	EXPECT_TRUE(IsParseOk("Sat, 08 Jan 2005 12:30:00 GMT", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::HTTP_FORMAT) == "Sat, 08 Jan 2005 12:30:00 GMT");

	EXPECT_TRUE(IsParseOk("Sat, 08 Jan 2005 12:30:00 +0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::HTTP_FORMAT, 3600) == "Sat, 08 Jan 2005 12:30:00 +0100");

	EXPECT_TRUE(IsParseOk("Sat, 08 Jan 2005 12:30:00 -0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::HTTP_FORMAT, -3600) == "Sat, 08 Jan 2005 12:30:00 -0100");
}


ADD_TEST(TimeParseFormatTest, TestRFC850) {

	EXPECT_TRUE(IsParseOk("Saturday, 8-Jan-05 12:30:00 GMT", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC850_FORMAT) == "Saturday, 8-Jan-05 12:30:00 GMT");

	EXPECT_TRUE(IsParseOk("Saturday, 8-Jan-05 12:30:00 +0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC850_FORMAT, 3600) == "Saturday, 8-Jan-05 12:30:00 +0100");

	EXPECT_TRUE(IsParseOk("Saturday, 8-Jan-05 12:30:00 -0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC850_FORMAT, -3600) == "Saturday, 8-Jan-05 12:30:00 -0100");

	EXPECT_TRUE(IsParseOk("Wed, 12-Sep-73 02:01:12 CEST", 1973, 9, 12, 2, 1, 12, 0, 0));
}


ADD_TEST(TimeParseFormatTest, TestRFC1036) {

	EXPECT_TRUE(IsParseOk("Saturday, 8 Jan 05 12:30:00 GMT", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC1036_FORMAT) == "Saturday, 8 Jan 05 12:30:00 GMT");

	EXPECT_TRUE(IsParseOk("Saturday, 8 Jan 05 12:30:00 +0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC1036_FORMAT, 3600) == "Saturday, 8 Jan 05 12:30:00 +0100");

	EXPECT_TRUE(IsParseOk("Saturday, 8 Jan 05 12:30:00 -0100", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::RFC1036_FORMAT, -3600) == "Saturday, 8 Jan 05 12:30:00 -0100");
}


ADD_TEST(TimeParseFormatTest, TestASCTIME) {

	EXPECT_TRUE(IsParseOk("Sat Jan  8 12:30:00 2005", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::ASCTIME_FORMAT) == "Sat Jan  8 12:30:00 2005");
}


ADD_TEST(TimeParseFormatTest, TestSORTABLE) {

	EXPECT_TRUE(IsParseOk("2005-01-08 12:30:00", 2005, 1, 8, 12, 30, 0, 0, 0));
	EXPECT_TRUE(ToString(Time::SORTABLE_FORMAT) == "2005-01-08 12:30:00");
}


ADD_TEST(TimeParseFormatTest, TestCustom){

	EXPECT_TRUE(IsParseOk("%d-%b-%Y", "18-Jan-2005", 2005, 1, 18, 0, 0, 0, 0, 0));
	
	EXPECT_TRUE(IsParseOk("%m/%d/%y", "01/18/05", 2005, 1, 18, 0, 0, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("%h:%M %a", "12:30 am", 1970, 1, 1, 0, 30, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("%h:%M %a", "12:30 PM", 1970, 1, 1, 12, 30, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("%w/%B/%d/%Y/%h/%A/%M/%S/%F/%Z/%%",
		"Sat/January/08/2005/12/PM/30/00/250000/GMT/%", 2005, 1, 8, 12, 30, 0, 250, 0));
	EXPECT_TRUE(ToString("%w/%W/%b/%B/%d/%e/%f/%m/%n/%o/%y/%Y/%H/%h/%a/%A/%M/%S/%i/%c/%z/%Z/%%") == 
		"Sat/Saturday/Jan/January/08/8/ 8/01/1/ 1/05/2005/12/12/pm/PM/30/00/250/2/Z/GMT/%");
}


ADD_TEST(TimeParseFormatTest, TestGuess) {

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00Z", 2005, 1, 8, 12, 30, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("20050108T123000Z", 2005, 1, 8, 12, 30, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00+01:00", 2005, 1, 8, 12, 30, 0, 0, 0));

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.123456Z", 2005, 1, 8, 12, 30, 0, 123, 456));

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00,123456Z", 2005, 1, 8, 12, 30, 0, 123, 456));

	EXPECT_TRUE(IsParseOk("20050108T123000,123456Z", 2005, 1, 8, 12, 30, 0, 123, 456));

	EXPECT_TRUE(IsParseOk("20050108T123000.123+0200", 2005, 1, 8, 12, 30, 0, 123, 0));

	EXPECT_TRUE(IsParseOk("2005-01-08T12:30:00.123456-02:00", 2005, 1, 8, 12, 30, 0, 123, 456));
}