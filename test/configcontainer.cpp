#include "catch.hpp"

#include <configcontainer.h>
#include <configparser.h>
#include <keymap.h>

using namespace newsboat;

TEST_CASE("ConfigContainer parses test config without exceptions",
          "[configcontainer]")
{
	configcontainer cfg;
	configparser cfgparser;
	cfg.register_commands(cfgparser);
	keymap k(KM_NEWSBOAT);
	cfgparser.register_handler("macro", &k);

	REQUIRE_NOTHROW(cfgparser.parse("data/test-config.txt"));

	SECTION("bool value") {
		REQUIRE(cfg.get_configvalue("show-read-feeds") == "no");
		REQUIRE_FALSE(cfg.get_configvalue_as_bool("show-read-feeds"));
	}

	SECTION("string value") {
		REQUIRE(cfg.get_configvalue("browser") == "firefox");
	}

	SECTION("integer value") {
		REQUIRE(cfg.get_configvalue("max-items") == "100");
		REQUIRE(cfg.get_configvalue_as_int("max-items") == 100);
	}

	SECTION("Tilde got expanded into path to user's home directory") {
		std::string cachefilecomp = ::getenv("HOME");
		cachefilecomp.append("/foo");
		REQUIRE(cfg.get_configvalue("cache-file") == cachefilecomp);
	}
}

TEST_CASE("ConfigContainer parses test config correctly, even if there's no \\n at the end line.",
          "[configcontainer]")
{
	configcontainer cfg;
	configparser cfgparser;
	cfg.register_commands(cfgparser);

	REQUIRE_NOTHROW(cfgparser.parse("data/test-config-without-newline-at-the-end.txt"));

	SECTION("first line") {
		REQUIRE(cfg.get_configvalue("browser") == "firefox");
	}

	SECTION("last line") {
		REQUIRE(cfg.get_configvalue("download-path") == "whatever");
	}
}
