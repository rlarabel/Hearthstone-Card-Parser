// 1h. Deduplicate cards array
#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "munit.h"

extern CARD_T *parse_card(char*);

int main(int argc, char **argv) {
	munit_assert_ptr(cards, ==, NULL);

	char *buf = strdup("38522,\"Eldritch Horror\",8,,6,10,\"MINION\",\"NEUTRAL\",\"COMMON\"\n");
	CARD_T *card = parse_card(buf);
	munit_assert_size(total_cards, ==, 0);
	CARD_T **cards = realloc(NULL, sizeof(CARD_T*) * (total_cards + 1));
	cards[total_cards++] = card;

	munit_assert_uint(cards[0]->id, ==, 38522);
	munit_assert_string_equal(cards[0]->name, "Eldritch Horror");
	munit_assert_uint(cards[0]->cost, ==, 8);
	munit_assert_string_equal(cards[0]->text, "");
	munit_assert_uint(cards[0]->attack, ==, 6);
	munit_assert_uint(cards[0]->health, ==, 10);
	munit_assert_int(cards[0]->type, ==, MINION);
	munit_assert_int(cards[0]->class, ==, NEUTRAL);
	munit_assert_int(cards[0]->rarity, ==, COMMON);

	free(buf);
}

