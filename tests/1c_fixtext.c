// 1h. Deduplicate cards array
#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "munit.h"

extern char *fix_text(char*);

int main(int argc, char **argv) {
// 1b. Replace Newline

	char* html_input = strdup("\\n");
	munit_assert_string_equal("\n", fix_text((char*)html_input));

	// 1c. Replace quotes

	html_input = strdup("\"\"hey\"\"");
	munit_assert_string_equal("\"hey\"", fix_text((char*)html_input));

	// 1d. Replace bold

	html_input = strdup("<b>Hello</b> I'm looking for <b>boldness!</b>");
	munit_assert_string_equal("\e[1mHello\e[0m I'm looking for \e[1mboldness!\e[0m", fix_text((char*)html_input));
		
	// 1e. Replace italic

	html_input = strdup("<i>Hello</i> I'm looking for <i>italicness!</i>");
	munit_assert_string_equal("\e[3mHello\e[0m I'm looking for \e[3mitalicness!\e[0m", fix_text((char*)html_input));
		
	// 1f. Mixed string replace

	html_input = strdup("<b>Hello</b>\\n I'm looking for \"\"<i>italicness!</i>\"\"");
	munit_assert_string_equal("\e[1mHello\e[0m\n I'm looking for \"\e[3mitalicness!\e[0m\"", fix_text((char*)html_input));
		
	// 1g. More mixed string replace

	html_input = strdup("<b><i>Hello</i></b>\\n\\n I'm looking\\n for \"\"<i>italicness!</i>\"\"");
	munit_assert_string_equal("\e[1m\e[3mHello\e[0m\e[0m\n\n I'm looking\n for \"\e[3mitalicness!\e[0m\"", fix_text((char*)html_input));

	html_input = strdup("<b>Battlecry:</b> <b>Discover</b>\\nand cast 2 Improved\\n<b>Secrets</b>.");
	munit_assert_string_equal("\e[1mBattlecry:\e[0m \e[1mDiscover\e[0m\nand cast 2 Improved\n\e[1mSecrets\e[0m.", fix_text(html_input));

}

