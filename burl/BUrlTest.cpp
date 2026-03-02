/*
 * Copyright 2026, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */


#include <Message.h>
#include <Path.h>
#include <Url.h>

#include <TestCase.h>
#include <TestSuiteAddon.h>
#include <TestUtils.h>


using CppUnit::TestFixture;


class BUrlTest : public TestFixture {
	CPPUNIT_TEST_SUITE(BUrlTest);

	// Constructor tests
	CPPUNIT_TEST(DefaultConstructor_HasNoFields);
	CPPUNIT_TEST(StringConstructor_ParsesAllComponents);
	CPPUNIT_TEST(StringConstructorWithEncode_EncodesSpecialChars);
	CPPUNIT_TEST(CopyConstructor_CopiesAllFields);
	CPPUNIT_TEST(RelativeUrlConstructor_ResolvesRelativePath);
	CPPUNIT_TEST(RelativeUrlConstructorWithDotDot_RemovesDotSegments);
	CPPUNIT_TEST(RelativeUrlConstructorWithAbsolutePath_UsesAbsolutePath);
	CPPUNIT_TEST(RelativeUrlConstructorWithProtocol_UsesRelativeUrl);
	CPPUNIT_TEST(RelativeUrlConstructorWithAuthority_UsesRelativeAuthority);
	CPPUNIT_TEST(RelativeUrlConstructorEmptyRelative_UsesBasePath);
	CPPUNIT_TEST(RelativeUrlConstructorWithQueryOnly_UsesBasePathWithNewQuery);
	CPPUNIT_TEST(RelativeUrlConstructorWithFragmentOnly_UsesBasePathAndQuery);
	CPPUNIT_TEST(RelativeUrlConstructorWithDotSegment_RemovesDotSegments);
	CPPUNIT_TEST(RelativeUrlConstructorMultipleDotDot_RemovesMultipleSegments);
	CPPUNIT_TEST(RelativeUrlConstructorEmptyBasePath_MergesCorrectly);
	CPPUNIT_TEST(RelativeUrlConstructorWithPathNoQuery_ClearsQuery);
	CPPUNIT_TEST(RelativeUrlConstructorWithAuthorityAndPath_UsesRelativeAuthority);
	CPPUNIT_TEST(RelativeUrlConstructorWithProtocolAndFragment_PreservesFragment);
	CPPUNIT_TEST(PathConstructor_CreatesFileUrl);
	CPPUNIT_TEST(ArchiveConstructor_RestoresFromMessage);

	// SetUrlString tests
	CPPUNIT_TEST(SetUrlStringWithEncode_EncodesUrl);
	CPPUNIT_TEST(SetUrlStringWithoutEncode_ParsesRaw);

	// Setter tests
	CPPUNIT_TEST(SetProtocol_SetsProtocol);
	CPPUNIT_TEST(SetProtocolEmpty_ClearsProtocol);
	CPPUNIT_TEST(SetUserName_SetsUserName);
	CPPUNIT_TEST(SetPassword_SetsPassword);
	CPPUNIT_TEST(SetHost_SetsHost);
	CPPUNIT_TEST(SetHostEmpty_ClearsHost);
	CPPUNIT_TEST(SetPort_SetsPort);
	CPPUNIT_TEST(SetPortZero_ClearsPort);
	CPPUNIT_TEST(SetPath_SetsAndNormalizesPath);
	CPPUNIT_TEST(SetPathWithDotSegments_RemovesDotSegments);
	CPPUNIT_TEST(SetPathWithDoubleDotSegments_RemovesParentDirectory);
	CPPUNIT_TEST(SetRequest_SetsRequest);
	CPPUNIT_TEST(SetFragment_SetsFragment);
	CPPUNIT_TEST(SetAuthority_ParsesUserPasswordHostPort);
	CPPUNIT_TEST(SetAuthorityWithIPv6_ParsesIPv6Host);
	CPPUNIT_TEST(SetAuthorityEmpty_SetsEmptyAuthority);

	// Getter tests
	CPPUNIT_TEST(UrlString_ReturnsCompleteUrl);
	CPPUNIT_TEST(Protocol_ReturnsProtocol);
	CPPUNIT_TEST(UserName_ReturnsUserName);
	CPPUNIT_TEST(Password_ReturnsPassword);
	CPPUNIT_TEST(UserInfo_ReturnsUserColonPassword);
	CPPUNIT_TEST(Host_ReturnsHost);
	CPPUNIT_TEST(Port_ReturnsPort);
	CPPUNIT_TEST(Authority_ReturnsFullAuthority);
	CPPUNIT_TEST(Path_ReturnsPath);
	CPPUNIT_TEST(Request_ReturnsRequest);
	CPPUNIT_TEST(Fragment_ReturnsFragment);

	// Has* tests
	CPPUNIT_TEST(HasProtocol_ReturnsTrueWhenSet);
	CPPUNIT_TEST(HasProtocol_ReturnsFalseWhenEmpty);
	CPPUNIT_TEST(HasAuthority_ReturnsTrueWhenHostSet);
	CPPUNIT_TEST(HasAuthority_ReturnsTrueWhenUserNameSet);
	CPPUNIT_TEST(HasAuthority_ReturnsFalseWhenEmpty);
	CPPUNIT_TEST(HasUserName_ReturnsTrueWhenSet);
	CPPUNIT_TEST(HasPassword_ReturnsTrueWhenSet);
	CPPUNIT_TEST(HasUserInfo_ReturnsTrueWhenUserNameOrPasswordSet);
	CPPUNIT_TEST(HasHost_ReturnsTrueWhenSet);
	CPPUNIT_TEST(HasPort_ReturnsTrueWhenNonZero);
	CPPUNIT_TEST(HasPath_ReturnsTrueWhenSet);
	CPPUNIT_TEST(HasRequest_ReturnsTrueWhenSet);
	CPPUNIT_TEST(HasRequest_ReturnsTrueWhenEmptyButPresent);
	CPPUNIT_TEST(HasFragment_ReturnsTrueWhenSet);

	// IsValid tests
	CPPUNIT_TEST(IsValid_ReturnsFalseWhenNoProtocol);
	CPPUNIT_TEST(IsValid_ReturnsFalseWhenInvalidProtocol);
	CPPUNIT_TEST(IsValid_ReturnsTrueForHttpWithHost);
	CPPUNIT_TEST(IsValid_ReturnsFalseForHttpWithoutHost);
	CPPUNIT_TEST(IsValid_ReturnsTrueForFileWithPath);
	CPPUNIT_TEST(IsValid_ReturnsFalseForFileWithoutPath);
	CPPUNIT_TEST(IsValid_ReturnsTrueForUnknownProtocol);

	// Static encoding/decoding tests
	CPPUNIT_TEST(UrlEncode_EncodesSpacesAsPercent20);
	CPPUNIT_TEST(UrlEncodeNonStrict_EncodesSpacesAsPlus);
	CPPUNIT_TEST(UrlEncodeDirectory_PreservesSlashes);
	CPPUNIT_TEST(UrlDecode_DecodesPercent20ToSpace);
	CPPUNIT_TEST(UrlDecodeNonStrict_DecodesPlusToSpace);

	// Operator tests
	CPPUNIT_TEST(AssignmentOperator_CopiesAllFields);
	CPPUNIT_TEST(StringAssignmentOperator_ParsesUrlWithEncode);
	CPPUNIT_TEST(CharPtrAssignmentOperator_ParsesUrlWithEncode);
	CPPUNIT_TEST(EqualityOperator_ReturnsTrueForSameUrl);
	CPPUNIT_TEST(EqualityOperator_ReturnsFalseForDifferentUrl);
	CPPUNIT_TEST(InequalityOperator_ReturnsTrueForDifferentUrl);
	CPPUNIT_TEST(CharPtrConversionOperator_ReturnsUrlString);

	// Archive tests
	CPPUNIT_TEST(Archive_StoresUrlInMessage);
	CPPUNIT_TEST(Instantiate_CreatesUrlFromMessage);
	CPPUNIT_TEST(InstantiateInvalid_ReturnsNull);

	// Complex URL tests
	CPPUNIT_TEST(ComplexUrl_ParsesAllComponents);
	CPPUNIT_TEST(IPv6Host_ParsesCorrectly);
	CPPUNIT_TEST(MinimalUrl_ParsesCorrectly);
	CPPUNIT_TEST(UrlWithEmptyQuery_HasRequest);
	CPPUNIT_TEST(UrlWithEmptyPath_IsValid);
	CPPUNIT_TEST(UrlWithUserInfoNoPassword_ParsesCorrectly);
	CPPUNIT_TEST(UrlWithSpecialCharsInPath_ParsesCorrectly);

#if defined(HAIKU_TARGET_PLATFORM_HAIKU) || defined(HAIKU_TARGET_PLATFORM_LIBBE_TEST)
	// IDNA tests
	CPPUNIT_TEST(IDNAToAscii_ConvertsInternationalDomain);
	CPPUNIT_TEST(IDNAToUnicode_ConvertsPunycode);
#endif

	CPPUNIT_TEST_SUITE_END();

public:
	// Constructor tests
	void DefaultConstructor_HasNoFields() {
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

	void StringConstructor_ParsesAllComponents() {
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

	void StringConstructorWithEncode_EncodesSpecialChars() {
		BUrl url("http://host.com/path with spaces", true);

		CPPUNIT_ASSERT(url.Path().FindFirst("%20") >= 0 || url.Path().FindFirst("+") >= 0
			|| url.Path().FindFirst(" ") < 0);
	}

	void CopyConstructor_CopiesAllFields() {
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
		CPPUNIT_ASSERT_EQUAL(url1.UrlString(), url2.UrlString());
		CPPUNIT_ASSERT_EQUAL(url1.Authority(), url2.Authority());
		CPPUNIT_ASSERT_EQUAL(url1.UserInfo(), url2.UserInfo());
	}

	void RelativeUrlConstructor_ResolvesRelativePath() {
		BUrl base("https://host.com/dir/", false);
		BUrl rel(base, BString("subdir/file.txt"));

		CPPUNIT_ASSERT_EQUAL(BString("https"), rel.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("host.com"), rel.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/dir/subdir/file.txt"), rel.Path());
	}

	void RelativeUrlConstructorWithDotDot_RemovesDotSegments() {
		BUrl base("http://example.com/dir1/dir2/page.html", false);
		BUrl relative(base, BString("../other.html"));

		CPPUNIT_ASSERT_EQUAL(BString("/dir1/other.html"), relative.Path());
	}

	void RelativeUrlConstructorWithAbsolutePath_UsesAbsolutePath() {
		BUrl base("http://example.com/dir/page.html", false);
		BUrl relative(base, BString("/absolute/path"));

		CPPUNIT_ASSERT_EQUAL(BString("/absolute/path"), relative.Path());
	}

	void RelativeUrlConstructorWithProtocol_UsesRelativeUrl() {
		BUrl base("http://example.com/path", false);
		BUrl relative(base, BString("https://other.com/newpath"));

		CPPUNIT_ASSERT_EQUAL(BString("https"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("other.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
	}

	void RelativeUrlConstructorWithAuthority_UsesRelativeAuthority() {
		BUrl base("http://example.com/path", false);
		BUrl relative(base, BString("//other.com/newpath"));

		CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("other.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
	}

	void RelativeUrlConstructorEmptyRelative_UsesBasePath() {
		BUrl base("http://example.com/path?query=1", false);
		BUrl relative(base, BString(""));

		CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
		CPPUNIT_ASSERT_EQUAL(BString("query=1"), relative.Request());
	}

	void RelativeUrlConstructorWithQueryOnly_UsesBasePathWithNewQuery() {
		BUrl base("http://example.com/path?oldquery", false);
		BUrl relative(base, BString("?newquery"));

		CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
		CPPUNIT_ASSERT_EQUAL(BString("newquery"), relative.Request());
	}

	void RelativeUrlConstructorWithFragmentOnly_UsesBasePathAndQuery() {
		BUrl base("http://example.com/path?query", false);
		BUrl relative(base, BString("#section"));

		CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
		CPPUNIT_ASSERT_EQUAL(BString("query"), relative.Request());
		CPPUNIT_ASSERT_EQUAL(BString("section"), relative.Fragment());
	}

	void RelativeUrlConstructorWithDotSegment_RemovesDotSegments() {
		BUrl base("http://example.com/base/index.html", false);
		BUrl relative(base, BString("./other.html"));

		CPPUNIT_ASSERT_EQUAL(BString("/base/other.html"), relative.Path());
	}

	void RelativeUrlConstructorMultipleDotDot_RemovesMultipleSegments() {
		BUrl base("http://example.com/a/b/c/page.html", false);
		BUrl relative(base, BString("../../other.html"));

		CPPUNIT_ASSERT_EQUAL(BString("/a/other.html"), relative.Path());
	}

	void RelativeUrlConstructorEmptyBasePath_MergesCorrectly() {
		BUrl base("http://example.com", false);
		BUrl relative(base, BString("path"));

		CPPUNIT_ASSERT_EQUAL(BString("/path"), relative.Path());
	}

	void RelativeUrlConstructorWithPathNoQuery_ClearsQuery() {
		BUrl base("http://example.com/path?basequery", false);
		BUrl relative(base, BString("newpath"));

		CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("example.com"), relative.Host());
		CPPUNIT_ASSERT(!relative.HasRequest() || relative.Request().IsEmpty());
	}

	void RelativeUrlConstructorWithAuthorityAndPath_UsesRelativeAuthority() {
		BUrl base("http://example.com/basepath?query", false);
		BUrl relative(base, BString("//newhost.com/newpath?newquery"));

		CPPUNIT_ASSERT_EQUAL(BString("http"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("newhost.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
		CPPUNIT_ASSERT_EQUAL(BString("newquery"), relative.Request());
	}

	void RelativeUrlConstructorWithProtocolAndFragment_PreservesFragment() {
		BUrl base("http://example.com/path", false);
		BUrl relative(base, BString("https://other.com/newpath#frag"));

		CPPUNIT_ASSERT_EQUAL(BString("https"), relative.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("other.com"), relative.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/newpath"), relative.Path());
		CPPUNIT_ASSERT_EQUAL(BString("frag"), relative.Fragment());
	}

	void PathConstructor_CreatesFileUrl() {
		BPath path("/tmp/test.txt");
		BUrl url(path);

		CPPUNIT_ASSERT_EQUAL(BString("file"), url.Protocol());
		CPPUNIT_ASSERT(url.Path().FindFirst("/tmp/test.txt") >= 0);
	}

	void ArchiveConstructor_RestoresFromMessage() {
		BUrl url1("http://host.com/path", false);
		BMessage archive;
		status_t result = url1.Archive(&archive, true);

		if (result == B_OK) {
			BUrl url2(&archive);
			CPPUNIT_ASSERT_EQUAL(url1.UrlString(), url2.UrlString());
		}
	}

	// SetUrlString tests
	void SetUrlStringWithEncode_EncodesUrl() {
		BUrl url;
		url.SetUrlString("http://host.com/path with spaces", true);

		CPPUNIT_ASSERT(url.IsValid());
	}

	void SetUrlStringWithoutEncode_ParsesRaw() {
		BUrl url;
		url.SetUrlString("http://host.com/path", false);

		CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("host.com"), url.Host());
		CPPUNIT_ASSERT_EQUAL(BString("/path"), url.Path());
	}

	// Setter tests
	void SetProtocol_SetsProtocol() {
		BUrl url;
		url.SetProtocol("http");

		CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
		CPPUNIT_ASSERT(url.HasProtocol());
	}

	void SetProtocolEmpty_ClearsProtocol() {
		BUrl url("http://host.com", false);
		url.SetProtocol("");

		CPPUNIT_ASSERT(!url.HasProtocol());
	}

	void SetUserName_SetsUserName() {
		BUrl url("http://host.com", false);
		url.SetUserName("testuser");

		CPPUNIT_ASSERT_EQUAL(BString("testuser"), url.UserName());
		CPPUNIT_ASSERT(url.HasUserName());
	}

	void SetPassword_SetsPassword() {
		BUrl url("http://host.com", false);
		url.SetPassword("testpass");

		CPPUNIT_ASSERT_EQUAL(BString("testpass"), url.Password());
		CPPUNIT_ASSERT(url.HasPassword());
	}

	void SetHost_SetsHost() {
		BUrl url;
		url.SetHost("example.com");

		CPPUNIT_ASSERT_EQUAL(BString("example.com"), url.Host());
		CPPUNIT_ASSERT(url.HasHost());
	}

	void SetHostEmpty_ClearsHost() {
		BUrl url("http://host.com", false);
		url.SetHost("");

		CPPUNIT_ASSERT(!url.HasHost());
	}

	void SetPort_SetsPort() {
		BUrl url("http://host.com", false);
		url.SetPort(9090);

		CPPUNIT_ASSERT_EQUAL(9090, url.Port());
		CPPUNIT_ASSERT(url.HasPort());
	}

	void SetPortZero_ClearsPort() {
		BUrl url("http://host.com:8080", false);
		url.SetPort(0);

		CPPUNIT_ASSERT(!url.HasPort());
	}

	void SetPath_SetsAndNormalizesPath() {
		BUrl url("http://host.com", false);
		url.SetPath("/test/path");

		CPPUNIT_ASSERT_EQUAL(BString("/test/path"), url.Path());
		CPPUNIT_ASSERT(url.HasPath());
	}

	void SetPathWithDotSegments_RemovesDotSegments() {
		BUrl url("http://host.com", false);
		url.SetPath("/a/./b/./c");

		CPPUNIT_ASSERT_EQUAL(BString("/a/b/c"), url.Path());
	}

	void SetPathWithDoubleDotSegments_RemovesParentDirectory() {
		BUrl url("http://host.com", false);
		url.SetPath("/a/b/../c/./d");

		CPPUNIT_ASSERT_EQUAL(BString("/a/c/d"), url.Path());
	}

	void SetRequest_SetsRequest() {
		BUrl url("http://host.com", false);
		url.SetRequest("key=value&foo=bar");

		CPPUNIT_ASSERT_EQUAL(BString("key=value&foo=bar"), url.Request());
		CPPUNIT_ASSERT(url.HasRequest());
	}

	void SetFragment_SetsFragment() {
		BUrl url("http://host.com", false);
		url.SetFragment("section");

		CPPUNIT_ASSERT_EQUAL(BString("section"), url.Fragment());
		CPPUNIT_ASSERT(url.HasFragment());
	}

	void SetAuthority_ParsesUserPasswordHostPort() {
		BUrl url("http://example.com", false);
		url.SetAuthority("user:pass@newhost.com:8080");

		CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserName());
		CPPUNIT_ASSERT_EQUAL(BString("pass"), url.Password());
		CPPUNIT_ASSERT_EQUAL(BString("newhost.com"), url.Host());
		CPPUNIT_ASSERT_EQUAL(8080, url.Port());
	}

	void SetAuthorityWithIPv6_ParsesIPv6Host() {
		BUrl url("http://example.com", false);
		url.SetAuthority("[2001:db8::1]:8080");

		CPPUNIT_ASSERT_EQUAL(BString("[2001:db8::1]"), url.Host());
		CPPUNIT_ASSERT_EQUAL(8080, url.Port());
	}

	void SetAuthorityEmpty_SetsEmptyAuthority() {
		BUrl url("http://host.com/path", false);
		url.SetAuthority("");

		CPPUNIT_ASSERT(url.HasAuthority());
		CPPUNIT_ASSERT(url.Host().IsEmpty());
	}

	// Getter tests
	void UrlString_ReturnsCompleteUrl() {
		BUrl url("https://host.com/path?query=1#frag", false);
		BString urlStr = url.UrlString();

		CPPUNIT_ASSERT(urlStr.FindFirst("https://host.com/path") >= 0);
		CPPUNIT_ASSERT(urlStr.FindFirst("query=1") >= 0);
		CPPUNIT_ASSERT(urlStr.FindFirst("#frag") >= 0);
	}

	void Protocol_ReturnsProtocol() {
		BUrl url("ftp://host.com", false);

		CPPUNIT_ASSERT_EQUAL(BString("ftp"), url.Protocol());
	}

	void UserName_ReturnsUserName() {
		BUrl url("http://myuser@host.com", false);

		CPPUNIT_ASSERT_EQUAL(BString("myuser"), url.UserName());
	}

	void Password_ReturnsPassword() {
		BUrl url("http://user:mypass@host.com", false);

		CPPUNIT_ASSERT_EQUAL(BString("mypass"), url.Password());
	}

	void UserInfo_ReturnsUserColonPassword() {
		BUrl url("http://user:pass@host.com", false);

		CPPUNIT_ASSERT_EQUAL(BString("user:pass"), url.UserInfo());
	}

	void Host_ReturnsHost() {
		BUrl url("http://myhost.example.com/path", false);

		CPPUNIT_ASSERT_EQUAL(BString("myhost.example.com"), url.Host());
	}

	void Port_ReturnsPort() {
		BUrl url("http://host.com:1234", false);

		CPPUNIT_ASSERT_EQUAL(1234, url.Port());
	}

	void Authority_ReturnsFullAuthority() {
		BUrl url("http://user:pass@host.com:8080/path", false);

		CPPUNIT_ASSERT_EQUAL(BString("user:pass@host.com:8080"), url.Authority());
	}

	void Path_ReturnsPath() {
		BUrl url("http://host.com/my/path/here", false);

		CPPUNIT_ASSERT_EQUAL(BString("/my/path/here"), url.Path());
	}

	void Request_ReturnsRequest() {
		BUrl url("http://host.com?foo=bar&baz=qux", false);

		CPPUNIT_ASSERT_EQUAL(BString("foo=bar&baz=qux"), url.Request());
	}

	void Fragment_ReturnsFragment() {
		BUrl url("http://host.com#mysection", false);

		CPPUNIT_ASSERT_EQUAL(BString("mysection"), url.Fragment());
	}

	// Has* tests
	void HasProtocol_ReturnsTrueWhenSet() {
		BUrl url("http://host.com", false);

		CPPUNIT_ASSERT(url.HasProtocol());
	}

	void HasProtocol_ReturnsFalseWhenEmpty() {
		BUrl url;

		CPPUNIT_ASSERT(!url.HasProtocol());
	}

	void HasAuthority_ReturnsTrueWhenHostSet() {
		BUrl url("http://host.com", false);

		CPPUNIT_ASSERT(url.HasAuthority());
	}

	void HasAuthority_ReturnsTrueWhenUserNameSet() {
		BUrl url;
		url.SetUserName("user");

		CPPUNIT_ASSERT(url.HasAuthority());
	}

	void HasAuthority_ReturnsFalseWhenEmpty() {
		BUrl url;

		CPPUNIT_ASSERT(!url.HasAuthority());
	}

	void HasUserName_ReturnsTrueWhenSet() {
		BUrl url("http://user@host.com", false);

		CPPUNIT_ASSERT(url.HasUserName());
	}

	void HasPassword_ReturnsTrueWhenSet() {
		BUrl url("http://user:pass@host.com", false);

		CPPUNIT_ASSERT(url.HasPassword());
	}

	void HasUserInfo_ReturnsTrueWhenUserNameOrPasswordSet() {
		BUrl url1("http://user:pass@host.com", false);
		CPPUNIT_ASSERT(url1.HasUserInfo());

		BUrl url2("http://user@host.com", false);
		CPPUNIT_ASSERT(url2.HasUserInfo());

		BUrl url3("http://host.com", false);
		CPPUNIT_ASSERT(!url3.HasUserInfo());
	}

	void HasHost_ReturnsTrueWhenSet() {
		BUrl url("http://host.com", false);

		CPPUNIT_ASSERT(url.HasHost());
	}

	void HasPort_ReturnsTrueWhenNonZero() {
		BUrl url1("http://host.com:8080", false);
		CPPUNIT_ASSERT(url1.HasPort());

		BUrl url2("http://host.com", false);
		CPPUNIT_ASSERT(!url2.HasPort());
	}

	void HasPath_ReturnsTrueWhenSet() {
		BUrl url("http://host.com/path", false);

		CPPUNIT_ASSERT(url.HasPath());
	}

	void HasRequest_ReturnsTrueWhenSet() {
		BUrl url("http://host.com?query=1", false);

		CPPUNIT_ASSERT(url.HasRequest());
	}

	void HasRequest_ReturnsTrueWhenEmptyButPresent() {
		BUrl url("http://host.com/path?", false);

		CPPUNIT_ASSERT(url.HasRequest());
		CPPUNIT_ASSERT(url.Request().IsEmpty());
	}

	void HasFragment_ReturnsTrueWhenSet() {
		BUrl url("http://host.com#frag", false);

		CPPUNIT_ASSERT(url.HasFragment());
	}

	// IsValid tests
	void IsValid_ReturnsFalseWhenNoProtocol() {
		BUrl url;

		CPPUNIT_ASSERT(!url.IsValid());
	}

	void IsValid_ReturnsFalseWhenInvalidProtocol() {
		BUrl url;
		url.SetProtocol("123invalid");
		url.SetHost("host.com");

		CPPUNIT_ASSERT(!url.IsValid());
	}

	void IsValid_ReturnsTrueForHttpWithHost() {
		BUrl url("http://host.com/path", false);

		CPPUNIT_ASSERT(url.IsValid());
	}

	void IsValid_ReturnsFalseForHttpWithoutHost() {
		BUrl url;
		url.SetProtocol("http");
		url.SetPath("/path");

		CPPUNIT_ASSERT(!url.IsValid());
	}

	void IsValid_ReturnsTrueForFileWithPath() {
		BUrl url("file:///path/to/file", false);

		CPPUNIT_ASSERT(url.IsValid());
	}

	void IsValid_ReturnsFalseForFileWithoutPath() {
		BUrl url;
		url.SetProtocol("file");

		CPPUNIT_ASSERT(!url.IsValid());
	}

	void IsValid_ReturnsTrueForUnknownProtocol() {
		BUrl url;
		url.SetProtocol("myproto");

		CPPUNIT_ASSERT(url.IsValid());
	}

	// Static encoding/decoding tests
	void UrlEncode_EncodesSpacesAsPercent20() {
		BString encoded = BUrl::UrlEncode("hello world", true, false);
		CPPUNIT_ASSERT(encoded.FindFirst("%20") >= 0);
	}

	void UrlEncodeNonStrict_EncodesSpacesAsPlus() {
		BString encoded = BUrl::UrlEncode("hello world", false, false);
		CPPUNIT_ASSERT(encoded.FindFirst("+") >= 0);
	}

	void UrlEncodeDirectory_PreservesSlashes() {
		BString encoded = BUrl::UrlEncode("/path/to/file", true, true);
		CPPUNIT_ASSERT(encoded.FindFirst("/") >= 0);
	}

	void UrlDecode_DecodesPercent20ToSpace() {
		BString decoded = BUrl::UrlDecode("hello%20world", true);

		CPPUNIT_ASSERT_EQUAL(BString("hello world"), decoded);
	}

	void UrlDecodeNonStrict_DecodesPlusToSpace() {
		BString decoded = BUrl::UrlDecode("hello+world", false);

		CPPUNIT_ASSERT_EQUAL(BString("hello world"), decoded);
	}

	// Operator tests
	void AssignmentOperator_CopiesAllFields() {
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

	void StringAssignmentOperator_ParsesUrlWithEncode() {
		BUrl url;
		url = BString("http://example.com/test");

		CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("example.com"), url.Host());
	}

	void CharPtrAssignmentOperator_ParsesUrlWithEncode() {
		BUrl url;
		url = "ftp://ftp.example.com/file.txt";

		CPPUNIT_ASSERT_EQUAL(BString("ftp"), url.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("ftp.example.com"), url.Host());
	}

	void EqualityOperator_ReturnsTrueForSameUrl() {
		BUrl url1("http://host.com/path", false);
		BUrl url2("http://host.com/path", false);

		CPPUNIT_ASSERT(url1 == url2);
	}

	void EqualityOperator_ReturnsFalseForDifferentUrl() {
		BUrl url1("http://host1.com/path", false);
		BUrl url2("http://host2.com/path", false);

		CPPUNIT_ASSERT(!(url1 == url2));
	}

	void InequalityOperator_ReturnsTrueForDifferentUrl() {
		BUrl url1("http://host1.com/path", false);
		BUrl url2("http://host2.com/path", false);

		CPPUNIT_ASSERT(url1 != url2);
	}

	void CharPtrConversionOperator_ReturnsUrlString() {
		BUrl url("http://host.com/path", false);
		const char* urlStr = url;

		CPPUNIT_ASSERT(urlStr != NULL);
		CPPUNIT_ASSERT(BString(urlStr).FindFirst("http://host.com") >= 0);
	}

	// Archive tests
	void Archive_StoresUrlInMessage() {
		BUrl url("http://host.com:8080/path?query=1", false);
		BMessage archive;
		status_t result = url.Archive(&archive, true);

		CPPUNIT_ASSERT_EQUAL(B_OK, result);

		BString storedUrl;
		CPPUNIT_ASSERT_EQUAL(B_OK, archive.FindString("be:url string", &storedUrl));
		CPPUNIT_ASSERT(storedUrl.FindFirst("http://host.com") >= 0);
	}

	void Instantiate_CreatesUrlFromMessage() {
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
	}

	void InstantiateInvalid_ReturnsNull() {
		BMessage invalidArchive;

		BArchivable* result = BUrl::Instantiate(&invalidArchive);

		CPPUNIT_ASSERT(result == NULL);
	}

	// Complex URL tests
	void ComplexUrl_ParsesAllComponents() {
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

	void IPv6Host_ParsesCorrectly() {
		BUrl url("http://[2001:db8::1]:8080/path", false);

		CPPUNIT_ASSERT_EQUAL(BString("[2001:db8::1]"), url.Host());
		CPPUNIT_ASSERT_EQUAL(8080, url.Port());
	}

	void MinimalUrl_ParsesCorrectly() {
		BUrl url("http://h", false);

		CPPUNIT_ASSERT_EQUAL(BString("http"), url.Protocol());
		CPPUNIT_ASSERT_EQUAL(BString("h"), url.Host());
	}

	void UrlWithEmptyQuery_HasRequest() {
		BUrl url("http://host.com/path?", false);

		CPPUNIT_ASSERT(url.HasRequest());
		CPPUNIT_ASSERT(url.Request().IsEmpty());
	}

	void UrlWithEmptyPath_IsValid() {
		BUrl url("http://host.com", false);

		CPPUNIT_ASSERT(url.IsValid());
		CPPUNIT_ASSERT(!url.HasPath());
	}

	void UrlWithUserInfoNoPassword_ParsesCorrectly() {
		BUrl url("http://user@host.com/path", false);

		CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserName());
		CPPUNIT_ASSERT(!url.HasPassword());
		CPPUNIT_ASSERT_EQUAL(BString("user"), url.UserInfo());
	}

	void UrlWithSpecialCharsInPath_ParsesCorrectly() {
		BUrl url("http://host.com/path%20with%20spaces", false);

		CPPUNIT_ASSERT(url.Path().FindFirst("path%20with%20spaces") >= 0);
	}

#if defined(HAIKU_TARGET_PLATFORM_HAIKU) || defined(HAIKU_TARGET_PLATFORM_LIBBE_TEST)
	// IDNA tests
	void IDNAToAscii_ConvertsInternationalDomain() {
		BUrl url("http://münchen.de", false);
		status_t result = url.IDNAToAscii();

		CPPUNIT_ASSERT_EQUAL(B_OK, result);
		CPPUNIT_ASSERT(url.Host().FindFirst("xn--") >= 0);
	}

	void IDNAToUnicode_ConvertsPunycode() {
		BUrl url("http://xn--mnchen-3ya.de", false);
		status_t result = url.IDNAToUnicode();

		CPPUNIT_ASSERT_EQUAL(B_OK, result);
		CPPUNIT_ASSERT(url.Host().FindFirst("münchen") >= 0);
	}
#endif
};


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BUrlTest, getTestSuiteName());

