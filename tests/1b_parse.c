// 1h. Deduplicate cards array
#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "munit.h"

extern CARD_T *parse_card(char*);

int main(int argc, char **argv) {
	munit_assert_ptr(cards, ==, NULL);

	char *buf = strdup("64900,\"The Demon Seed\",1,\"<b>Questline:</b> Take 8\\ndamage on your turns.\\n<b>Reward:</b> <b>Lifesteal</b>. Deal $3\\ndamage to the enemy hero.\",,,\"SPELL\",\"WARLOCK\",\"LEGENDARY\"\n");
	CARD_T *card = parse_card(buf);
	munit_assert_size(total_cards, ==, 0);
	CARD_T **cards = realloc(NULL, sizeof(CARD_T*) * (total_cards + 1));
	cards[total_cards++] = card;

	munit_assert_uint(cards[0]->id, ==, 64900);
	munit_assert_string_equal(cards[0]->name, "The Demon Seed");
	munit_assert_string_equal(cards[0]->text, "\e[1mQuestline:\e[0m Take 8\ndamage on your turns.\n\e[1mReward:\e[0m \e[1mLifesteal\e[0m. Deal $3\ndamage to the enemy hero.");
	munit_assert_uint(cards[0]->cost, ==, 1);
	munit_assert_uint(cards[0]->attack, ==, 0);
	munit_assert_uint(cards[0]->health, ==, 0);
	munit_assert_int(cards[0]->type, ==, SPELL);
	munit_assert_int(cards[0]->class, ==, WARLOCK);
	munit_assert_int(cards[0]->rarity, ==, LEGENDARY);

	free(buf);
}

