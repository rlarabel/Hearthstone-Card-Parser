// 1h. Deduplicate cards array
#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "munit.h"

int dupe_check(unsigned, char*);

int main(int argc, char **argv) {
	CARD_T a = {1, "Loki" };
	CARD_T b = {2, "Thor" };
	CARD_T c = {3, "hello" };
	
	cards = malloc(sizeof(CARD_T*) * 3);
	cards[0] = &a;
	cards[1] = &b;
	cards[2] = &c;
	
	total_cards = 3;
	
	if (dupe_check(10, "hello") != -1) {
	    printf("failed to return dedup == -1 got `%d`\n", dupe_check(10, "hello"));
	    return MUNIT_FAIL;
	}
	if (dupe_check(1, "hello") != 2) {
	    printf("failed to return dedup == 2 got `%d`\n", dupe_check(1, "hello"));
	    return MUNIT_FAIL;
	}
	if (dupe_check(5, "Unique") != -2) {
	    printf("failed to return dedup == -2 got `%d`\n", dupe_check(5, "Unique"));
	    return MUNIT_FAIL;
	}
}

