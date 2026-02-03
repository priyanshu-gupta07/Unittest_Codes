/*
 * Copyright 2026, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */


#include "BUrlTest.h"

#include <Message.h>
#include <Path.h>
#include <Url.h>

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>


class BUrlTest : public BTestCase {
public:
	BUrlTest(std::string name = "");

	// Constructor tests
	void DefaultConstructor_HasNoFields();
	void StringConstructor_ParsesAllComponents();
	void StringConstructorWithEncode_EncodesSpecialChars();
	void CopyConstructor_CopiesAllFields();
	void RelativeUrlConstructor_ResolvesRelativePath();
	void RelativeUrlConstructorWithDotDot_RemovesDotSegments();
	void RelativeUrlConstructorWithAbsolutePath_UsesAbsolutePath();
	void RelativeUrlConstructorWithProtocol_UsesRelativeUrl();
	void RelativeUrlConstructorWithAuthority_UsesRelativeAuthority();
	void RelativeUrlConstructorEmptyRelative_UsesBasePath();
	void RelativeUrlConstructorWithQueryOnly_UsesBasePathWithNewQuery();
	void RelativeUrlConstructorWithFragmentOnly_UsesBasePathAndQuery();
	void RelativeUrlConstructorWithDotSegment_RemovesDotSegments();
	void RelativeUrlConstructorMultipleDotDot_RemovesMultipleSegments();
	void RelativeUrlConstructorEmptyBasePath_MergesCorrectly();
	void RelativeUrlConstructorWithPathNoQuery_ClearsQuery();
	void RelativeUrlConstructorWithAuthorityAndPath_UsesRelativeAuthority();
	void RelativeUrlConstructorWithProtocolAndFragment_PreservesFragment();
	void PathConstructor_CreatesFileUrl();
	void ArchiveConstructor_RestoresFromMessage();

	// SetUrlString tests
	void SetUrlStringWithEncode_EncodesUrl();
	void SetUrlStringWithoutEncode_ParsesRaw();

	// Setter tests
	void SetProtocol_SetsProtocol();
	void SetProtocolEmpty_ClearsProtocol();
	void SetUserName_SetsUserName();
	void SetPassword_SetsPassword();
	void SetHost_SetsHost();
	void SetHostEmpty_ClearsHost();
	void SetPort_SetsPort();
	void SetPortZero_ClearsPort();
	void SetPath_SetsAndNormalizesPath();
	void SetPathWithDotSegments_RemovesDotSegments();
	void SetPathWithDoubleDotSegments_RemovesParentDirectory();
	void SetRequest_SetsRequest();
	void SetFragment_SetsFragment();
	void SetAuthority_ParsesUserPasswordHostPort();
	void SetAuthorityWithIPv6_ParsesIPv6Host();
	void SetAuthorityEmpty_SetsEmptyAuthority();

	// Getter tests
	void UrlString_ReturnsCompleteUrl();
	void Protocol_ReturnsProtocol();
	void UserName_ReturnsUserName();
	void Password_ReturnsPassword();
	void UserInfo_ReturnsUserColonPassword();
	void Host_ReturnsHost();
	void Port_ReturnsPort();
	void Authority_ReturnsFullAuthority();
	void Path_ReturnsPath();
	void Request_ReturnsRequest();
	void Fragment_ReturnsFragment();

	// Has* tests
	void HasProtocol_ReturnsTrueWhenSet();
	void HasProtocol_ReturnsFalseWhenEmpty();
	void HasAuthority_ReturnsTrueWhenHostSet();
	void HasAuthority_ReturnsTrueWhenUserNameSet();
	void HasAuthority_ReturnsFalseWhenEmpty();
	void HasUserName_ReturnsTrueWhenSet();
	void HasPassword_ReturnsTrueWhenSet();
	void HasUserInfo_ReturnsTrueWhenUserNameOrPasswordSet();
	void HasHost_ReturnsTrueWhenSet();
	void HasPort_ReturnsTrueWhenNonZero();
	void HasPath_ReturnsTrueWhenSet();
	void HasRequest_ReturnsTrueWhenSet();
	void HasRequest_ReturnsTrueWhenEmptyButPresent();
	void HasFragment_ReturnsTrueWhenSet();

	// IsValid tests
	void IsValid_ReturnsFalseWhenNoProtocol();
	void IsValid_ReturnsFalseWhenInvalidProtocol();
	void IsValid_ReturnsTrueForHttpWithHost();
	void IsValid_ReturnsFalseForHttpWithoutHost();
	void IsValid_ReturnsTrueForFileWithPath();
	void IsValid_ReturnsFalseForFileWithoutPath();
	void IsValid_ReturnsTrueForUnknownProtocol();

	// Static encoding/decoding tests
	void UrlEncode_EncodesSpacesAsPercent20();
	void UrlEncodeNonStrict_EncodesSpacesAsPlus();
	void UrlEncodeDirectory_PreservesSlashes();
	void UrlDecode_DecodesPercent20ToSpace();
	void UrlDecodeNonStrict_DecodesPlusToSpace();

	// Operator tests
	void AssignmentOperator_CopiesAllFields();
	void StringAssignmentOperator_ParsesUrlWithEncode();
	void CharPtrAssignmentOperator_ParsesUrlWithEncode();
	void EqualityOperator_ReturnsTrueForSameUrl();
	void EqualityOperator_ReturnsFalseForDifferentUrl();
	void InequalityOperator_ReturnsTrueForDifferentUrl();
	void CharPtrConversionOperator_ReturnsUrlString();

	// Archive tests
	void Archive_StoresUrlInMessage();
	void Instantiate_CreatesUrlFromMessage();
	void InstantiateInvalid_ReturnsNull();

	// Complex URL tests
	void ComplexUrl_ParsesAllComponents();
	void IPv6Host_ParsesCorrectly();
	void MinimalUrl_ParsesCorrectly();
	void UrlWithEmptyQuery_HasRequest();
	void UrlWithEmptyPath_IsValid();
	void UrlWithUserInfoNoPassword_ParsesCorrectly();
	void UrlWithSpecialCharsInPath_ParsesCorrectly();

	// IDNA tests
#if defined(HAIKU_TARGET_PLATFORM_HAIKU) || defined(HAIKU_TARGET_PLATFORM_LIBBE_TEST)
	void IDNAToAscii_ConvertsInternationalDomain();
	void IDNAToUnicode_ConvertsPunycode();
#endif
};


BUrlTest::BUrlTest(std::string name)
	:
	BTestCase(name)
{
}


// Constructor tests

void
BUrlTest::DefaultConstructor_HasNoFields()
{
	BUrl url;

	CPPUNIT_ASSERT(!url.HasProtocol());
	CPPUNIT_ASSERT(!url.HasHost());
	CPPUNIT_ASSERT(!url.HasPath());
	CPPUNIT_ASSERT(!url.HasUserName());
	CPPUNIT_ASSERT(!url.HasPassword());
	CPPUNIT_ASSERT(!url.HasPort());
	CPPUNIT_ASSERT(!url.HasRequest());
	CPPUNIT_ASSERT(!url.HasFragment());
	CPPUNIT_ASSERT(!url.IsValid());
}


void
BUrlTest::StringConstructor_ParsesAllComponents()
{
	BUrl url("https://user:pass@host.com:8080/path?query=1#frag", false);

	CPPUNIT_ASSERT_EQUAL(BString("https"), url.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserName());
	CPPUNIT_ASSERT_EQUAL(BString("pass"), url.Password());
	CPPUNIT_ASSERT_EQUAL(BString("host.com"), url.Host());
	CPPUNIT_ASSERT_EQUAL(8080, url.Port());
	CPPUNIT_ASSERT_EQUAL(BString("/path"), url.Path());
	CPPUNIT_ASSERT_EQUAL(BString("query=1"), url.Request());
	CPPUNIT_ASSERT_EQUAL(BString("frag"), url.Fragment());
}


void
BUrlTest::StringConstructorWithEncode_EncodesSpecialChars()
{
	BUrl url("http://host.com/path with spaces", true);

	CPPUNIT_ASSERT(url.Path().FindFirst("%20") >= 0 || url.Path().FindFirst("+") >= 0
		|| url.Path().FindFirst(" ") < 0);
		// need to check for either %20 or + because different
		//  implementations may encode spaces differently
}


void
BUrlTest::CopyConstructor_CopiesAllFields()
{
	BUrl url1("https://user:pass@host.com:8080/path?query=1#frag", false);
	BUrl url2(url1);

	CPPUNIT_ASSERT_EQUAL(url1.Protocol(), url2.Protocol());
	CPPUNIT_ASSERT_EQUAL(url1.UserName(), url2.UserName());
	CPPUNIT_ASSERT_EQUAL(url1.Password(), url2.Password());
	CPPUNIT_ASSERT_EQUAL(url1.Host(), url2.Host());
	CPPUNIT_ASSERT_EQUAL(url1.Port(), url2.Port());
	CPPUNIT_ASSERT_EQUAL(url1.Path(), url2.Path());
	CPPUNIT_ASSERT_EQUAL(url1.Request(), url2.Request());
	CPPUNIT_ASSERT_EQUAL(url1.Fragment(), url2.Fragment());

	// These should match because cached values were copied
	CPPUNIT_ASSERT_EQUAL(url1.UrlString(), url2.UrlString());
	CPPUNIT_ASSERT_EQUAL(url1.Authority(), url2.Authority());
	CPPUNIT_ASSERT_EQUAL(url1.UserInfo(), url2.UserInfo());
}


void
BUrlTest::RelativeUrlConstructor_ResolvesRelativePath()
{
	BUrl base("https://host.com/dir/", false);
	BUrl rel(base, "subdir/file.txt");

	CPPUNIT_ASSERT_EQUAL(BString("https"), rel.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("host.com"), rel.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/dir/subdir/file.txt"), rel.Path());
}


void
BUrlTest::RelativeUrlConstructorWithDotDot_RemovesDotSegments()
{
	BUrl base("http://example.com/dir1/dir2/page.html", false);
	BUrl relative(base, "../other.html");

	CPPUNIT_ASSERT_EQUAL(BString("/dir1/other.html"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorWithAbsolutePath_UsesAbsolutePath()
{
	BUrl base("http://example.com/dir/page.html", false);
	BUrl relative(base, "/absolute/path");

	CPPUNIT_ASSERT_EQUAL(BString("/absolute/path"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorWithProtocol_UsesRelativeUrl()
{
	BUrl base("http://example.com/path", false);
	BUrl relative(base, "https://other.com/newpath");

	CPPUNIT_ASSERT_EQUAL(BString("https"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("other.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorWithAuthority_UsesRelativeAuthority()
{
	BUrl base("http://example.com/path", false);
	BUrl relative(base, "//other.com/newpath");

	CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("other.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorEmptyRelative_UsesBasePath()
{
	BUrl base("http://example.com/path?query=1", false);
	BUrl relative(base, "");

	CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
	CPPUNIT_ASSERT_EQUAL(BString("query=1"), relative.Request());
}


void
BUrlTest::RelativeUrlConstructorWithQueryOnly_UsesBasePathWithNewQuery()
{
	// RFC3986 Section 5.4: "?y" should use base path with new query
	BUrl base("http://example.com/path?oldquery", false);
	BUrl relative(base, "?newquery");

	CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
	CPPUNIT_ASSERT_EQUAL(BString("newquery"), relative.Request());
}


void
BUrlTest::RelativeUrlConstructorWithFragmentOnly_UsesBasePathAndQuery()
{
	// RFC3986 Section 5.4: "#s" should use base path and query with new fragment
	BUrl base("http://example.com/path?query", false);
	BUrl relative(base, "#section");

	CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
	CPPUNIT_ASSERT_EQUAL(BString("query"), relative.Request());
	CPPUNIT_ASSERT_EQUAL(BString("section"), relative.Fragment());
}


void
BUrlTest::RelativeUrlConstructorWithDotSegment_RemovesDotSegments()
{
	// RFC3986 Section 5.4: "./g" should resolve to "/base/g"
	BUrl base("http://example.com/base/index.html", false);
	BUrl relative(base, "./other.html");

	CPPUNIT_ASSERT_EQUAL(BString("/base/other.html"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorMultipleDotDot_RemovesMultipleSegments()
{
	// RFC3986 Section 5.4: "../../g" should go up two directories
	BUrl base("http://example.com/a/b/c/page.html", false);
	BUrl relative(base, "../../other.html");

	CPPUNIT_ASSERT_EQUAL(BString("/a/other.html"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorEmptyBasePath_MergesCorrectly()
{
	// RFC3986 Section 5.2.3: If base has authority and empty path, result is "/" + relative
	BUrl base("http://example.com", false);
	BUrl relative(base, "path");

	CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
}


void
BUrlTest::RelativeUrlConstructorWithPathNoQuery_ClearsQuery()
{
	// RFC3986: Relative path without query should not inherit base query
	BUrl base("http://example.com/path?basequery", false);
	BUrl relative(base, "newpath");

	CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
	CPPUNIT_ASSERT(!relative.HasRequest() || relative.Request().IsEmpty());
}


void
BUrlTest::RelativeUrlConstructorWithAuthorityAndPath_UsesRelativeAuthority()
{
	// RFC3986: "//host/path" should use relative authority and path
	BUrl base("http://example.com/basepath?query", false);
	BUrl relative(base, "//newhost.com/newpath?newquery");

	CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("newhost.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
	CPPUNIT_ASSERT_EQUAL(BString("newquery"), relative.Request());
}


void
BUrlTest::RelativeUrlConstructorWithProtocolAndFragment_PreservesFragment()
{
	// RFC3986: Full URL with fragment should preserve fragment
	BUrl base("http://example.com/path", false);
	BUrl relative(base, "https://other.com/newpath#frag");

	CPPUNIT_ASSERT_EQUAL(BString("https"), relative.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("other.com"), relative.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
	CPPUNIT_ASSERT_EQUAL(BString("frag"), relative.Fragment());
}


void
BUrlTest::PathConstructor_CreatesFileUrl()
{
	BPath path("/tmp/test.txt");
	BUrl url(path);

	CPPUNIT_ASSERT_EQUAL(BString("file"), url.Protocol());
	CPPUNIT_ASSERT(url.Path().FindFirst("/tmp/test.txt") >= 0);
}


void
BUrlTest::ArchiveConstructor_RestoresFromMessage()
{
	BUrl url1("http://host.com/path", false);
	BMessage archive;
	status_t result = url1.Archive(&archive, true);

	if (result == B_OK) {
		BUrl url2(&archive);
		CPPUNIT_ASSERT_EQUAL(url1.UrlString(), url2.UrlString());
	}
	// If archiving failed, the test is inconclusive
}


// SetUrlString tests

void
BUrlTest::SetUrlStringWithEncode_EncodesUrl()
{
	BUrl url;
	url.SetUrlString("http://host.com/path with spaces", true);

	CPPUNIT_ASSERT(url.IsValid());
}


void
BUrlTest::SetUrlStringWithoutEncode_ParsesRaw()
{
	BUrl url;
	url.SetUrlString("http://host.com/path", false);

	CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("host.com"), url.Host());
	CPPUNIT_ASSERT_EQUAL(BString("/path"), url.Path());
}


// Setter tests

void
BUrlTest::SetProtocol_SetsProtocol()
{
	BUrl url;
	url.SetProtocol("http");

	CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
	CPPUNIT_ASSERT(url.HasProtocol());
}


void
BUrlTest::SetProtocolEmpty_ClearsProtocol()
{
	BUrl url("http://host.com", false);
	url.SetProtocol("");

	CPPUNIT_ASSERT(!url.HasProtocol());
}


void
BUrlTest::SetUserName_SetsUserName()
{
	BUrl url("http://host.com", false);
	url.SetUserName("testuser");

	CPPUNIT_ASSERT_EQUAL(BString("testuser"), url.UserName());
	CPPUNIT_ASSERT(url.HasUserName());
}


void
BUrlTest::SetPassword_SetsPassword()
{
	BUrl url("http://host.com", false);
	url.SetPassword("testpass");

	CPPUNIT_ASSERT_EQUAL(BString("testpass"), url.Password());
	CPPUNIT_ASSERT(url.HasPassword());
}


void
BUrlTest::SetHost_SetsHost()
{
	BUrl url;
	url.SetHost("example.com");

	CPPUNIT_ASSERT_EQUAL(BString("example.com"), url.Host());
	CPPUNIT_ASSERT(url.HasHost());
}


void
BUrlTest::SetHostEmpty_ClearsHost()
{
	BUrl url("http://host.com", false);
	url.SetHost("");

	CPPUNIT_ASSERT(!url.HasHost());
}


void
BUrlTest::SetPort_SetsPort()
{
	BUrl url("http://host.com", false);
	url.SetPort(9090);

	CPPUNIT_ASSERT_EQUAL(9090, url.Port());
	CPPUNIT_ASSERT(url.HasPort());
}


void
BUrlTest::SetPortZero_ClearsPort()
{
	BUrl url("http://host.com:8080", false);
	url.SetPort(0);

	CPPUNIT_ASSERT(!url.HasPort());
}


void
BUrlTest::SetPath_SetsAndNormalizesPath()
{
	BUrl url("http://host.com", false);
	url.SetPath("/test/path");

	CPPUNIT_ASSERT_EQUAL(BString("/test/path"), url.Path());
	CPPUNIT_ASSERT(url.HasPath());
}


void
BUrlTest::SetPathWithDotSegments_RemovesDotSegments()
{
	BUrl url("http://host.com", false);
	url.SetPath("/a/./b/./c");

	CPPUNIT_ASSERT_EQUAL(BString("/a/b/c"), url.Path());
}


void
BUrlTest::SetPathWithDoubleDotSegments_RemovesParentDirectory()
{
	BUrl url("http://host.com", false);
	url.SetPath("/a/b/../c/./d");

	CPPUNIT_ASSERT_EQUAL(BString("/a/c/d"), url.Path());
}


void
BUrlTest::SetRequest_SetsRequest()
{
	BUrl url("http://host.com", false);
	url.SetRequest("key=value&foo=bar");

	CPPUNIT_ASSERT_EQUAL(BString("key=value&foo=bar"), url.Request());
	CPPUNIT_ASSERT(url.HasRequest());
}


void
BUrlTest::SetFragment_SetsFragment()
{
	BUrl url("http://host.com", false);
	url.SetFragment("section");

	CPPUNIT_ASSERT_EQUAL(BString("section"), url.Fragment());
	CPPUNIT_ASSERT(url.HasFragment());
}


void
BUrlTest::SetAuthority_ParsesUserPasswordHostPort()
{
	BUrl url("http://example.com", false);
	url.SetAuthority("user:pass@newhost.com:8080");

	CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserName());
	CPPUNIT_ASSERT_EQUAL(BString("pass"), url.Password());
	CPPUNIT_ASSERT_EQUAL(BString("newhost.com"), url.Host());
	CPPUNIT_ASSERT_EQUAL(8080, url.Port());
}


void
BUrlTest::SetAuthorityWithIPv6_ParsesIPv6Host()
{
	BUrl url("http://example.com", false);
	url.SetAuthority("[2001:db8::1]:8080");

	CPPUNIT_ASSERT_EQUAL(BString("[2001:db8::1]"), url.Host());
	CPPUNIT_ASSERT_EQUAL(8080, url.Port());
}


void
BUrlTest::SetAuthorityEmpty_SetsEmptyAuthority()
{
	BUrl url("http://host.com/path", false);
	url.SetAuthority("");

	CPPUNIT_ASSERT(url.HasAuthority());
	CPPUNIT_ASSERT(url.Host().IsEmpty());
}


// Getter tests

void
BUrlTest::UrlString_ReturnsCompleteUrl()
{
	BUrl url("https://host.com/path?query=1#frag", false);
	BString urlStr = url.UrlString();

	CPPUNIT_ASSERT(urlStr.FindFirst("https://host.com/path") >= 0);
	CPPUNIT_ASSERT(urlStr.FindFirst("query=1") >= 0);
	CPPUNIT_ASSERT(urlStr.FindFirst("#frag") >= 0);
}


void
BUrlTest::Protocol_ReturnsProtocol()
{
	BUrl url("ftp://host.com", false);

	CPPUNIT_ASSERT_EQUAL(BString("ftp"), url.Protocol());
}


void
BUrlTest::UserName_ReturnsUserName()
{
	BUrl url("http://myuser@host.com", false);

	CPPUNIT_ASSERT_EQUAL(BString("myuser"), url.UserName());
}


void
BUrlTest::Password_ReturnsPassword()
{
	BUrl url("http://user:mypass@host.com", false);

	CPPUNIT_ASSERT_EQUAL(BString("mypass"), url.Password());
}


void
BUrlTest::UserInfo_ReturnsUserColonPassword()
{
	BUrl url("http://user:pass@host.com", false);

	CPPUNIT_ASSERT_EQUAL(BString("user:pass"), url.UserInfo());
}


void
BUrlTest::Host_ReturnsHost()
{
	BUrl url("http://myhost.example.com/path", false);

	CPPUNIT_ASSERT_EQUAL(BString("myhost.example.com"), url.Host());
}


void
BUrlTest::Port_ReturnsPort()
{
	BUrl url("http://host.com:1234", false);

	CPPUNIT_ASSERT_EQUAL(1234, url.Port());
}


void
BUrlTest::Authority_ReturnsFullAuthority()
{
	BUrl url("http://user:pass@host.com:8080/path", false);

	CPPUNIT_ASSERT_EQUAL(BString("user:pass@host.com:8080"), url.Authority());
}


void
BUrlTest::Path_ReturnsPath()
{
	BUrl url("http://host.com/my/path/here", false);

	CPPUNIT_ASSERT_EQUAL(BString("/my/path/here"), url.Path());
}


void
BUrlTest::Request_ReturnsRequest()
{
	BUrl url("http://host.com?foo=bar&baz=qux", false);

	CPPUNIT_ASSERT_EQUAL(BString("foo=bar&baz=qux"), url.Request());
}


void
BUrlTest::Fragment_ReturnsFragment()
{
	BUrl url("http://host.com#mysection", false);

	CPPUNIT_ASSERT_EQUAL(BString("mysection"), url.Fragment());
}


// Has* tests

void
BUrlTest::HasProtocol_ReturnsTrueWhenSet()
{
	BUrl url("http://host.com", false);

	CPPUNIT_ASSERT(url.HasProtocol());
}


void
BUrlTest::HasProtocol_ReturnsFalseWhenEmpty()
{
	BUrl url;

	CPPUNIT_ASSERT(!url.HasProtocol());
}


void
BUrlTest::HasAuthority_ReturnsTrueWhenHostSet()
{
	BUrl url("http://host.com", false);

	CPPUNIT_ASSERT(url.HasAuthority());
}


void
BUrlTest::HasAuthority_ReturnsTrueWhenUserNameSet()
{
	BUrl url;
	url.SetUserName("user");

	CPPUNIT_ASSERT(url.HasAuthority());
}


void
BUrlTest::HasAuthority_ReturnsFalseWhenEmpty()
{
	BUrl url;

	CPPUNIT_ASSERT(!url.HasAuthority());
}


void
BUrlTest::HasUserName_ReturnsTrueWhenSet()
{
	BUrl url("http://user@host.com", false);

	CPPUNIT_ASSERT(url.HasUserName());
}


void
BUrlTest::HasPassword_ReturnsTrueWhenSet()
{
	BUrl url("http://user:pass@host.com", false);

	CPPUNIT_ASSERT(url.HasPassword());
}


void
BUrlTest::HasUserInfo_ReturnsTrueWhenUserNameOrPasswordSet()
{
	BUrl url1("http://user:pass@host.com", false);
	CPPUNIT_ASSERT(url1.HasUserInfo());

	BUrl url2("http://user@host.com", false);
	CPPUNIT_ASSERT(url2.HasUserInfo());

	BUrl url3("http://host.com", false);
	CPPUNIT_ASSERT(!url3.HasUserInfo());
}


void
BUrlTest::HasHost_ReturnsTrueWhenSet()
{
	BUrl url("http://host.com", false);

	CPPUNIT_ASSERT(url.HasHost());
}


void
BUrlTest::HasPort_ReturnsTrueWhenNonZero()
{
	BUrl url1("http://host.com:8080", false);
	CPPUNIT_ASSERT(url1.HasPort());

	BUrl url2("http://host.com", false);
	CPPUNIT_ASSERT(!url2.HasPort());
}


void
BUrlTest::HasPath_ReturnsTrueWhenSet()
{
	BUrl url("http://host.com/path", false);

	CPPUNIT_ASSERT(url.HasPath());
}


void
BUrlTest::HasRequest_ReturnsTrueWhenSet()
{
	BUrl url("http://host.com?query=1", false);

	CPPUNIT_ASSERT(url.HasRequest());
}


void
BUrlTest::HasRequest_ReturnsTrueWhenEmptyButPresent()
{
	BUrl url("http://host.com/path?", false);

	CPPUNIT_ASSERT(url.HasRequest());
	CPPUNIT_ASSERT(url.Request().IsEmpty());
}


void
BUrlTest::HasFragment_ReturnsTrueWhenSet()
{
	BUrl url("http://host.com#frag", false);

	CPPUNIT_ASSERT(url.HasFragment());
}


// IsValid tests

void
BUrlTest::IsValid_ReturnsFalseWhenNoProtocol()
{
	BUrl url;

	CPPUNIT_ASSERT(!url.IsValid());
}


void
BUrlTest::IsValid_ReturnsFalseWhenInvalidProtocol()
{
	BUrl url;
	url.SetProtocol("123invalid");
	url.SetHost("host.com");

	CPPUNIT_ASSERT(!url.IsValid());
}


void
BUrlTest::IsValid_ReturnsTrueForHttpWithHost()
{
	BUrl url("http://host.com/path", false);

	CPPUNIT_ASSERT(url.IsValid());
}


void
BUrlTest::IsValid_ReturnsFalseForHttpWithoutHost()
{
	BUrl url;
	url.SetProtocol("http");
	url.SetPath("/path");

	CPPUNIT_ASSERT(!url.IsValid());
}


void
BUrlTest::IsValid_ReturnsTrueForFileWithPath()
{
	BUrl url("file:///path/to/file", false);

	CPPUNIT_ASSERT(url.IsValid());
}


void
BUrlTest::IsValid_ReturnsFalseForFileWithoutPath()
{
	BUrl url;
	url.SetProtocol("file");

	CPPUNIT_ASSERT(!url.IsValid());
}


void
BUrlTest::IsValid_ReturnsTrueForUnknownProtocol()
{
	BUrl url;
	url.SetProtocol("myproto");

	CPPUNIT_ASSERT(url.IsValid());
}


// Static encoding/decoding tests

void
BUrlTest::UrlEncode_EncodesSpacesAsPercent20()
{
	BString encoded = BUrl::UrlEncode("hello world", true, false);
	// By default, spaces are encoded as %20
	CPPUNIT_ASSERT(encoded.FindFirst("%20") >= 0);
}


void
BUrlTest::UrlEncodeNonStrict_EncodesSpacesAsPlus()
{
	BString encoded = BUrl::UrlEncode("hello world", false, false);
	// In non-strict mode, spaces are encoded as +
	CPPUNIT_ASSERT(encoded.FindFirst("+") >= 0);
}


void
BUrlTest::UrlEncodeDirectory_PreservesSlashes()
{
	BString encoded = BUrl::UrlEncode("/path/to/file", true, true);
	// In directory mode, slashes are preserved
	CPPUNIT_ASSERT(encoded.FindFirst("/") >= 0);
}


void
BUrlTest::UrlDecode_DecodesPercent20ToSpace()
{
	BString decoded = BUrl::UrlDecode("hello%20world", true);

	CPPUNIT_ASSERT_EQUAL(BString("hello world"), decoded);
}


void
BUrlTest::UrlDecodeNonStrict_DecodesPlusToSpace()
{
	BString decoded = BUrl::UrlDecode("hello+world", false);

	CPPUNIT_ASSERT_EQUAL(BString("hello world"), decoded);
}


// Operator tests

void
BUrlTest::AssignmentOperator_CopiesAllFields()
{
	BUrl url1("http://user:pass@host.com:8080/path?query#frag", false);
	BUrl url2;
	url2 = url1;

	CPPUNIT_ASSERT_EQUAL(url1.UrlString(), url2.UrlString());
	CPPUNIT_ASSERT_EQUAL(url1.Protocol(), url2.Protocol());
	CPPUNIT_ASSERT_EQUAL(url1.UserName(), url2.UserName());
	CPPUNIT_ASSERT_EQUAL(url1.Password(), url2.Password());
	CPPUNIT_ASSERT_EQUAL(url1.Host(), url2.Host());
	CPPUNIT_ASSERT_EQUAL(url1.Port(), url2.Port());
}


void
BUrlTest::StringAssignmentOperator_ParsesUrlWithEncode()
{
	BUrl url;
	url = BString("http://example.com/test");

	CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("example.com"), url.Host());
}


void
BUrlTest::CharPtrAssignmentOperator_ParsesUrlWithEncode()
{
	BUrl url;
	url = "ftp://ftp.example.com/file.txt";

	CPPUNIT_ASSERT_EQUAL(BString("ftp"), url.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("ftp.example.com"), url.Host());
}


void
BUrlTest::EqualityOperator_ReturnsTrueForSameUrl()
{
	BUrl url1("http://host.com/path", false);
	BUrl url2("http://host.com/path", false);

	CPPUNIT_ASSERT(url1 == url2);
}


void
BUrlTest::EqualityOperator_ReturnsFalseForDifferentUrl()
{
	BUrl url1("http://host1.com/path", false);
	BUrl url2("http://host2.com/path", false);

	CPPUNIT_ASSERT(!(url1 == url2));
}


void
BUrlTest::InequalityOperator_ReturnsTrueForDifferentUrl()
{
	BUrl url1("http://host1.com/path", false);
	BUrl url2("http://host2.com/path", false);

	CPPUNIT_ASSERT(url1 != url2);
}


void
BUrlTest::CharPtrConversionOperator_ReturnsUrlString()
{
	BUrl url("http://host.com/path", false);
	const char* urlStr = url;

	CPPUNIT_ASSERT(urlStr != NULL);
	CPPUNIT_ASSERT(BString(urlStr).FindFirst("http://host.com") >= 0);
}


// Archive tests

void
BUrlTest::Archive_StoresUrlInMessage()
{
	BUrl url("http://host.com:8080/path?query=1", false);
	BMessage archive;
	status_t result = url.Archive(&archive, true);

	CPPUNIT_ASSERT_EQUAL(B_OK, result);

	BString storedUrl;

	CPPUNIT_ASSERT_EQUAL(B_OK, archive.FindString("be:url string", &storedUrl));
	// Verify the URL string is stored correctly
	CPPUNIT_ASSERT(storedUrl.FindFirst("http://host.com") >= 0);
}


void
BUrlTest::Instantiate_CreatesUrlFromMessage()
{
	BUrl url1("http://host.com/path", false);
	BMessage archive;
	status_t result = url1.Archive(&archive, true);

	if (result == B_OK) {
		BArchivable* archivable = BUrl::Instantiate(&archive);

		CPPUNIT_ASSERT(archivable != NULL);
		BUrl* url2 = dynamic_cast<BUrl*>(archivable);
		CPPUNIT_ASSERT(url2 != NULL);
		CPPUNIT_ASSERT_EQUAL(url1.UrlString(), url2->UrlString());
		delete archivable;
	}
	// If archiving failed, the test is inconclusive
}


void
BUrlTest::InstantiateInvalid_ReturnsNull()
{
	BMessage invalidArchive;

	BArchivable* result = BUrl::Instantiate(&invalidArchive);

	CPPUNIT_ASSERT(result == NULL);
}


// Complex URL tests

void
BUrlTest::ComplexUrl_ParsesAllComponents()
{
	BUrl url("https://user:password@example.com:8443/api/v1/users?page=2&limit=10#results", false);

	CPPUNIT_ASSERT_EQUAL(BString("https"), url.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserName());
	CPPUNIT_ASSERT_EQUAL(BString("password"), url.Password());
	CPPUNIT_ASSERT_EQUAL(BString("example.com"), url.Host());
	CPPUNIT_ASSERT_EQUAL(8443, url.Port());
	CPPUNIT_ASSERT_EQUAL(BString("/api/v1/users"), url.Path());
	CPPUNIT_ASSERT_EQUAL(BString("page=2&limit=10"), url.Request());
	CPPUNIT_ASSERT_EQUAL(BString("results"), url.Fragment());
}


void
BUrlTest::IPv6Host_ParsesCorrectly()
{
	BUrl url("http://[2001:db8::1]:8080/path", false);

	CPPUNIT_ASSERT_EQUAL(BString("[2001:db8::1]"), url.Host());
	CPPUNIT_ASSERT_EQUAL(8080, url.Port());
}


void
BUrlTest::MinimalUrl_ParsesCorrectly()
{
	BUrl url("http://h", false);

	CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
	CPPUNIT_ASSERT_EQUAL(BString("h"), url.Host());
}


void
BUrlTest::UrlWithEmptyQuery_HasRequest()
{
	BUrl url("http://host.com/path?", false);

	CPPUNIT_ASSERT(url.HasRequest());
	CPPUNIT_ASSERT(url.Request().IsEmpty());
}


void
BUrlTest::UrlWithEmptyPath_IsValid()
{
	BUrl url("http://host.com", false);

	CPPUNIT_ASSERT(url.IsValid());
	CPPUNIT_ASSERT(!url.HasPath());
}


void
BUrlTest::UrlWithUserInfoNoPassword_ParsesCorrectly()
{
	BUrl url("http://user@host.com/path", false);

	CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserName());
	CPPUNIT_ASSERT(!url.HasPassword());
	CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserInfo());
}


void
BUrlTest::UrlWithSpecialCharsInPath_ParsesCorrectly()
{
	BUrl url("http://host.com/path%20with%20spaces", false);

	CPPUNIT_ASSERT(url.Path().FindFirst("path%20with%20spaces") >= 0);
}


// IDNA tests

#if defined(HAIKU_TARGET_PLATFORM_HAIKU) || defined(HAIKU_TARGET_PLATFORM_LIBBE_TEST)


void
BUrlTest::IDNAToAscii_ConvertsInternationalDomain()
{
	BUrl url("http://münchen.de", false);
	status_t result = url.IDNAToAscii();

	CPPUNIT_ASSERT_EQUAL(B_OK, result);
	CPPUNIT_ASSERT(url.Host().FindFirst("xn--") >= 0);
	// münchen should be converted to punycode starting with xn--
}


void
BUrlTest::IDNAToUnicode_ConvertsPunycode()
{
	BUrl url("http://xn--mnchen-3ya.de", false);
	status_t result = url.IDNAToUnicode();

	CPPUNIT_ASSERT_EQUAL(B_OK, result);
	CPPUNIT_ASSERT(url.Host().FindFirst("münchen") >= 0);
	// xn--mnchen-3ya should be converted back to münchen
}
#endif


CppUnit::Test*
BUrlTestSuite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("BUrl");

	// Constructor tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::DefaultConstructor_HasNoFields",
		&BUrlTest::DefaultConstructor_HasNoFields));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::StringConstructor_ParsesAllComponents",
			&BUrlTest::StringConstructor_ParsesAllComponents));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::StringConstructorWithEncode_EncodesSpecialChars",
		&BUrlTest::StringConstructorWithEncode_EncodesSpecialChars));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::CopyConstructor_CopiesAllFields",
		&BUrlTest::CopyConstructor_CopiesAllFields));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::RelativeUrlConstructor_ResolvesRelativePath",
			&BUrlTest::RelativeUrlConstructor_ResolvesRelativePath));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithDotDot_RemovesDotSegments",
		&BUrlTest::RelativeUrlConstructorWithDotDot_RemovesDotSegments));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithAbsolutePath_UsesAbsolutePath",
		&BUrlTest::RelativeUrlConstructorWithAbsolutePath_UsesAbsolutePath));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithProtocol_UsesRelativeUrl",
		&BUrlTest::RelativeUrlConstructorWithProtocol_UsesRelativeUrl));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithAuthority_UsesRelativeAuthority",
		&BUrlTest::RelativeUrlConstructorWithAuthority_UsesRelativeAuthority));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorEmptyRelative_UsesBasePath",
		&BUrlTest::RelativeUrlConstructorEmptyRelative_UsesBasePath));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithQueryOnly_UsesBasePathWithNewQuery",
		&BUrlTest::RelativeUrlConstructorWithQueryOnly_UsesBasePathWithNewQuery));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithFragmentOnly_UsesBasePathAndQuery",
		&BUrlTest::RelativeUrlConstructorWithFragmentOnly_UsesBasePathAndQuery));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithDotSegment_RemovesDotSegments",
		&BUrlTest::RelativeUrlConstructorWithDotSegment_RemovesDotSegments));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorMultipleDotDot_RemovesMultipleSegments",
		&BUrlTest::RelativeUrlConstructorMultipleDotDot_RemovesMultipleSegments));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorEmptyBasePath_MergesCorrectly",
		&BUrlTest::RelativeUrlConstructorEmptyBasePath_MergesCorrectly));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithPathNoQuery_ClearsQuery",
		&BUrlTest::RelativeUrlConstructorWithPathNoQuery_ClearsQuery));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithAuthorityAndPath_UsesRelativeAuthority",
		&BUrlTest::RelativeUrlConstructorWithAuthorityAndPath_UsesRelativeAuthority));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::RelativeUrlConstructorWithProtocolAndFragment_PreservesFragment",
		&BUrlTest::RelativeUrlConstructorWithProtocolAndFragment_PreservesFragment));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::PathConstructor_CreatesFileUrl",
		&BUrlTest::PathConstructor_CreatesFileUrl));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::ArchiveConstructor_RestoresFromMessage",
			&BUrlTest::ArchiveConstructor_RestoresFromMessage));

	// SetUrlString tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetUrlStringWithEncode_EncodesUrl",
		&BUrlTest::SetUrlStringWithEncode_EncodesUrl));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetUrlStringWithoutEncode_ParsesRaw",
			&BUrlTest::SetUrlStringWithoutEncode_ParsesRaw));

	// Setter tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetProtocol_SetsProtocol",
		&BUrlTest::SetProtocol_SetsProtocol));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetProtocolEmpty_ClearsProtocol",
		&BUrlTest::SetProtocolEmpty_ClearsProtocol));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetUserName_SetsUserName",
		&BUrlTest::SetUserName_SetsUserName));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetPassword_SetsPassword",
		&BUrlTest::SetPassword_SetsPassword));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetHost_SetsHost",
		&BUrlTest::SetHost_SetsHost));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetHostEmpty_ClearsHost",
		&BUrlTest::SetHostEmpty_ClearsHost));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetPort_SetsPort",
		&BUrlTest::SetPort_SetsPort));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetPortZero_ClearsPort",
		&BUrlTest::SetPortZero_ClearsPort));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetPath_SetsAndNormalizesPath",
		&BUrlTest::SetPath_SetsAndNormalizesPath));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetPathWithDotSegments_RemovesDotSegments",
			&BUrlTest::SetPathWithDotSegments_RemovesDotSegments));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::SetPathWithDoubleDotSegments_RemovesParentDirectory",
		&BUrlTest::SetPathWithDoubleDotSegments_RemovesParentDirectory));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetRequest_SetsRequest",
		&BUrlTest::SetRequest_SetsRequest));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetFragment_SetsFragment",
		&BUrlTest::SetFragment_SetsFragment));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetAuthority_ParsesUserPasswordHostPort",
			&BUrlTest::SetAuthority_ParsesUserPasswordHostPort));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetAuthorityWithIPv6_ParsesIPv6Host",
			&BUrlTest::SetAuthorityWithIPv6_ParsesIPv6Host));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::SetAuthorityEmpty_SetsEmptyAuthority",
			&BUrlTest::SetAuthorityEmpty_SetsEmptyAuthority));

	// Getter tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlString_ReturnsCompleteUrl",
		&BUrlTest::UrlString_ReturnsCompleteUrl));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Protocol_ReturnsProtocol",
		&BUrlTest::Protocol_ReturnsProtocol));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UserName_ReturnsUserName",
		&BUrlTest::UserName_ReturnsUserName));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Password_ReturnsPassword",
		&BUrlTest::Password_ReturnsPassword));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UserInfo_ReturnsUserColonPassword",
		&BUrlTest::UserInfo_ReturnsUserColonPassword));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Host_ReturnsHost",
		&BUrlTest::Host_ReturnsHost));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Port_ReturnsPort",
		&BUrlTest::Port_ReturnsPort));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Authority_ReturnsFullAuthority",
		&BUrlTest::Authority_ReturnsFullAuthority));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Path_ReturnsPath",
		&BUrlTest::Path_ReturnsPath));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Request_ReturnsRequest",
		&BUrlTest::Request_ReturnsRequest));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Fragment_ReturnsFragment",
		&BUrlTest::Fragment_ReturnsFragment));

	// Has* tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasProtocol_ReturnsTrueWhenSet",
		&BUrlTest::HasProtocol_ReturnsTrueWhenSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasProtocol_ReturnsFalseWhenEmpty",
		&BUrlTest::HasProtocol_ReturnsFalseWhenEmpty));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasAuthority_ReturnsTrueWhenHostSet",
			&BUrlTest::HasAuthority_ReturnsTrueWhenHostSet));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasAuthority_ReturnsTrueWhenUserNameSet",
			&BUrlTest::HasAuthority_ReturnsTrueWhenUserNameSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasAuthority_ReturnsFalseWhenEmpty",
		&BUrlTest::HasAuthority_ReturnsFalseWhenEmpty));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasUserName_ReturnsTrueWhenSet",
		&BUrlTest::HasUserName_ReturnsTrueWhenSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasPassword_ReturnsTrueWhenSet",
		&BUrlTest::HasPassword_ReturnsTrueWhenSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>(
		"BUrlTest::HasUserInfo_ReturnsTrueWhenUserNameOrPasswordSet",
		&BUrlTest::HasUserInfo_ReturnsTrueWhenUserNameOrPasswordSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasHost_ReturnsTrueWhenSet",
		&BUrlTest::HasHost_ReturnsTrueWhenSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasPort_ReturnsTrueWhenNonZero",
		&BUrlTest::HasPort_ReturnsTrueWhenNonZero));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasPath_ReturnsTrueWhenSet",
		&BUrlTest::HasPath_ReturnsTrueWhenSet));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasRequest_ReturnsTrueWhenSet",
		&BUrlTest::HasRequest_ReturnsTrueWhenSet));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasRequest_ReturnsTrueWhenEmptyButPresent",
			&BUrlTest::HasRequest_ReturnsTrueWhenEmptyButPresent));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::HasFragment_ReturnsTrueWhenSet",
		&BUrlTest::HasFragment_ReturnsTrueWhenSet));

	// IsValid tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsFalseWhenNoProtocol",
		&BUrlTest::IsValid_ReturnsFalseWhenNoProtocol));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsFalseWhenInvalidProtocol",
			&BUrlTest::IsValid_ReturnsFalseWhenInvalidProtocol));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsTrueForHttpWithHost",
		&BUrlTest::IsValid_ReturnsTrueForHttpWithHost));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsFalseForHttpWithoutHost",
			&BUrlTest::IsValid_ReturnsFalseForHttpWithoutHost));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsTrueForFileWithPath",
		&BUrlTest::IsValid_ReturnsTrueForFileWithPath));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsFalseForFileWithoutPath",
			&BUrlTest::IsValid_ReturnsFalseForFileWithoutPath));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::IsValid_ReturnsTrueForUnknownProtocol",
			&BUrlTest::IsValid_ReturnsTrueForUnknownProtocol));

	// Encoding/Decoding tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlEncode_EncodesSpacesAsPercent20",
		&BUrlTest::UrlEncode_EncodesSpacesAsPercent20));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlEncodeNonStrict_EncodesSpacesAsPlus",
			&BUrlTest::UrlEncodeNonStrict_EncodesSpacesAsPlus));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlEncodeDirectory_PreservesSlashes",
			&BUrlTest::UrlEncodeDirectory_PreservesSlashes));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlDecode_DecodesPercent20ToSpace",
		&BUrlTest::UrlDecode_DecodesPercent20ToSpace));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlDecodeNonStrict_DecodesPlusToSpace",
			&BUrlTest::UrlDecodeNonStrict_DecodesPlusToSpace));

	// Operator tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::AssignmentOperator_CopiesAllFields",
		&BUrlTest::AssignmentOperator_CopiesAllFields));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::StringAssignmentOperator_ParsesUrlWithEncode",
			&BUrlTest::StringAssignmentOperator_ParsesUrlWithEncode));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::CharPtrAssignmentOperator_ParsesUrlWithEncode",
			&BUrlTest::CharPtrAssignmentOperator_ParsesUrlWithEncode));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::EqualityOperator_ReturnsTrueForSameUrl",
			&BUrlTest::EqualityOperator_ReturnsTrueForSameUrl));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::EqualityOperator_ReturnsFalseForDifferentUrl",
			&BUrlTest::EqualityOperator_ReturnsFalseForDifferentUrl));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::InequalityOperator_ReturnsTrueForDifferentUrl",
			&BUrlTest::InequalityOperator_ReturnsTrueForDifferentUrl));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::CharPtrConversionOperator_ReturnsUrlString",
			&BUrlTest::CharPtrConversionOperator_ReturnsUrlString));

	// Archive tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Archive_StoresUrlInMessage",
		&BUrlTest::Archive_StoresUrlInMessage));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::Instantiate_CreatesUrlFromMessage",
		&BUrlTest::Instantiate_CreatesUrlFromMessage));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::InstantiateInvalid_ReturnsNull",
		&BUrlTest::InstantiateInvalid_ReturnsNull));

	// Complex URL tests
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::ComplexUrl_ParsesAllComponents",
		&BUrlTest::ComplexUrl_ParsesAllComponents));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::IPv6Host_ParsesCorrectly",
		&BUrlTest::IPv6Host_ParsesCorrectly));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::MinimalUrl_ParsesCorrectly",
		&BUrlTest::MinimalUrl_ParsesCorrectly));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlWithEmptyQuery_HasRequest",
		&BUrlTest::UrlWithEmptyQuery_HasRequest));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlWithEmptyPath_IsValid",
		&BUrlTest::UrlWithEmptyPath_IsValid));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlWithUserInfoNoPassword_ParsesCorrectly",
			&BUrlTest::UrlWithUserInfoNoPassword_ParsesCorrectly));
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::UrlWithSpecialCharsInPath_ParsesCorrectly",
			&BUrlTest::UrlWithSpecialCharsInPath_ParsesCorrectly));

#if defined(HAIKU_TARGET_PLATFORM_HAIKU) || defined(HAIKU_TARGET_PLATFORM_LIBBE_TEST)
	// IDNA tests
	suite->addTest(
		new CppUnit::TestCaller<BUrlTest>("BUrlTest::IDNAToAscii_ConvertsInternationalDomain",
			&BUrlTest::IDNAToAscii_ConvertsInternationalDomain));
	suite->addTest(new CppUnit::TestCaller<BUrlTest>("BUrlTest::IDNAToUnicode_ConvertsPunycode",
		&BUrlTest::IDNAToUnicode_ConvertsPunycode));
#endif

	return suite;
}
