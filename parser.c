#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card.h"

#define DUPE -1
#define NO_DUPE -2

#define BOLD "\e[1m"
#define ITALIC "\e[3m"
#define END "\e[0m"

int dupe_check(unsigned, char*);
char *fix_text(char*);
void free_card(CARD_T*);
CARD_T *parse_card(char*);
void print_card(CARD_T*);

int find_compar(const void *a, const void *b) {
	const char** aptr = (const char**) a;
	CARD_T** bptr = (CARD_T**) b;
	return strcmp(*aptr, (*bptr)->name);
}

int sort_compar(const void *a, const void *b) {
	CARD_T** aptr = (CARD_T**) a;
	CARD_T** bptr = (CARD_T**) b;
	return strcmp((*aptr)->name, (*bptr)->name);
}

CARD_T **cards = NULL;
size_t total_cards = 0;


int main(int argc, char **argv) {
	// Opens a Given User File 
	FILE *infile = fopen(argv[1], "r");
	if (infile == NULL) return -2;
	
	// Reads File Line By Line
	char *line = NULL;
	CARD_T *c = NULL;
	size_t n = 0;
	// Dumps first line of csv before parsing
	getline(&line, &n, infile);
	ssize_t result = getline(&line, &n, infile);
	while(result > 0) {
		c = parse_card(line);
		if(c != NULL) 
		{
			// Add parsed card to cards array and sort
			cards = realloc(cards, sizeof(cards)*(total_cards+1));
			cards[total_cards++] = c;

		}
		
		result = getline(&line, &n, infile);
	}

	fclose(infile);
	free(line);
	
	qsort(cards, total_cards, sizeof(CARD_T*), sort_compar);
	
	for(int i=0;i<total_cards; i++){
		print_card(cards[i]);
		free_card(cards[i]);
	}
	
	free(cards);
	
	return 0;
}


int dupe_check(unsigned id, char *name) {
	CARD_T** ptr = NULL;
	
	// Find any duplicates of new card in cards
	ptr = lfind(&name, cards, &total_cards, sizeof(CARD_T*), find_compar);
	if (ptr != NULL) {
		// Update id if card is lower
		if(ptr[0]->id > id) {
			// return index
			return (ptr - cards);
		} else {
			return DUPE;
		}
	} else {
		// Do nothing and return NO_DUPE
		return NO_DUPE;
	}
	
}


char *fix_text(char *text) {
	//int dest_len; 
	int fixed = 0;
	char* dest, *src;
	
	while(fixed == 0){
		if (strstr(text, "\"\"") != NULL) {
			// Moves past first quote
			dest = strstr(text, "\"\"") + 1;
			// Rewrites Memory over second quote
			memmove(dest, &dest[1], strlen(dest)); 
			text = realloc(text, sizeof(text)*strlen(text));
			
		} else if (strstr(text, "\\n") != NULL){
			// Find and rewites backslash char as a newline
			dest = (strstr(text, "\\n"));
			memmove(dest, "\n", 1); 
			// Rewrites Memory over 'n' char
			dest = dest + 1;
			memmove(dest, &dest[1], strlen(dest)); 
			text = realloc(text, sizeof(text)*strlen(text));
			
		} else if (strstr(text, "</b>") != NULL){
			// Replacement text
			src = "\e[0m";
			dest = strstr(text, "</b>");
			memmove(dest, src, strlen(src));
			
		} else if (strstr(text, "</i>") != NULL){
			//Same logic as above
			src = "\e[0m";
			dest = strstr(text, "</i>");
			memmove(dest, src, strlen(src));
			
		}else if (strstr(text, "<b>") != NULL){
			// Make space in memory 
			text = realloc(text, sizeof(text)*(strlen(text)+1));
			// Rewrite <b> as <<b> in string
			src = strstr(text, "<b>");
			dest = src + 1;
			memmove(dest, src, strlen(src)+1);
			// Replace text
			dest = src; 
			src = "\e[1m";
			memmove(dest, src, strlen(src)); 


		} else if (strstr(text, "<i>") != NULL){
			// Same Logic as above;
			text = realloc(text, sizeof(text)*(strlen(text)+1));
			src = strstr(text, "<i>");
			dest = src + 1;
			memmove(dest, src, strlen(src)+1);
			dest = src;
			src = "\e[3m";
			memmove(dest, src, strlen(src));
			 
		} else {
			fixed = 1;
		}
	}
	return text;
}

// Frees both fields and then the card itself
void free_card(CARD_T *card) {
	free(card->name);
	free(card->text);
	free(card);
}


CARD_T *parse_card(char *line) {
	char* temp_str;
	int i = 0;
	CARD_T* card = malloc(sizeof(CARD_T));
	int dup_check;
	
	// Strips new line character at end of line
	line = strsep(&line, "\n");
	
	// Parses One Entry
	while(line != NULL) {
		// Logic to properly set card fields
		switch(i) {
			// Id field
			case 0:
				card->id = atoi(strsep(&line, ","));
				break;
				
			// Name fields
			case 1:
				if (line[0] == '"') {
				 	// Strips first quote 
				 	strsep(&line, "\"");
				 	// Strips closing quote and comma
				 	card->name = strdup(strsep(&line, "\""));
				 	strsep(&line, ",");
				 	
				
				} // Handles cases for empty value for name
				else {
					printf("Error: Could not identify keys for the Name field for: %u\n", card->id);
					strsep(&line, ",");
					free(card);
					return NULL;
				}
				
				
				// If duplicated stop parsing and return null  
				dup_check = dupe_check(card->id, card->name);
				if(dup_check == DUPE){
					free(card->name);
					free(card); 
					return NULL;
					
				} else if (dup_check != NO_DUPE) {
					free_card(cards[dup_check]);

					// Rewites cards w/o higher id dup
					if((total_cards-1) > dup_check)
						memmove(&cards[dup_check], &cards[dup_check+1], (total_cards-dup_check-1)*sizeof(CARD_T**));
					--total_cards;			
				}

				break;
				
			// Cost fields
			case 2:
				card->cost = atoi(strsep(&line, ","));
				break;
				
			// Text fields
			case 3:
				if (line[0] == '"') {
				 	// Strips first quote 
				 	strsep(&line, "\"");
				 	temp_str = line;
				 	// Goes to end of user defined string (in most cases)
				 	line = strstr(line, "\",");
				 	// Strips closing quote and comma
				 	strsep(&line, "\"");
				 	strsep(&line, ",");
				 	card->text = strdup(temp_str);
				 	card->text = fix_text(card->text);
				 	
				
				} // Handles cases for empty values
				else {
					strsep(&line, ",");
					card->text = strdup("");
				}

				break;
				
			// Attack feilds
			case 4:
				card->attack = atoi(strsep(&line, ","));;
				break;
			
			// Health field
			case 5:
				card->health = atoi(strsep(&line, ","));;
				break;
			// Type fields
			case 6:
				if (line[0] == '"') {
				 	// Strips first quote 
				 	strsep(&line, "\"");
				 	// Strips closing quote and comma
				 	temp_str = strsep(&line, "\"");
				 	strsep(&line, ",");
				 	
				 	if (strcmp(temp_str, "HERO") == 0){
						card->type = 0;
					} else if (strcmp(temp_str, "MINION") == 0) {
						card->type = 1;
					} else if (strcmp(temp_str, "SPELL") == 0) {
						card->type = 2;
					} else if (strcmp(temp_str, "WEAPON") == 0) {
						card->type = 3;
					} else {
						printf("Error: Could not identify keys for the Type field for: %u, %s\n", card->id, card->name);
						free_card(card);
						return NULL;
				}
				 	
				
				} // Handles cases for empty value
				else {
					printf("Error: no Type field given for: %u, %s\n", card->id, card->name);
					strsep(&line, ",");
					free_card(card);
					return NULL;
				}
				
				
				break;
				
			// Class Field
			case 7:
				if (line[0] == '"') {
				 	// Strips first quote 
				 	strsep(&line, "\"");
				 	// Strips closing quote and comma
				 	temp_str = strsep(&line, "\"");
				 	strsep(&line, ",");
				 	
					if (strcmp(temp_str, "DEMONHUNTER") == 0) {
						card->class = 0;
					} else if (strcmp(temp_str, "DRUID") == 0) {
						card->class = 1;	
					} else if (strcmp(temp_str, "HUNTER") == 0) {
						card->class = 2;
					} else if (strcmp(temp_str, "MAGE") == 0) {
						card->class = 3;
					} else if (strcmp(temp_str, "NEUTRAL") == 0) {
						card->class = 4;
					} else if (strcmp(temp_str, "PALADIN") == 0) {
						card->class = 5;
					} else if (strcmp(temp_str, "PRIEST") == 0) {
						card->class = 6;
					} else if (strcmp(temp_str, "ROGUE") == 0) {
						card->class = 7;
					} else if (strcmp(temp_str, "SHAMAN") == 0) {
						card->class = 8;
					} else if (strcmp(temp_str, "WARLOCK") == 0) {
						card->class = 9;
					} else if (strcmp(temp_str, "WARRIOR") == 0) {
						card->class = 10;
					} else {
						printf("Error: Could not identify keys for the Class field for: %u, %s\n", card->id, card->name);
						free_card(card);
						return NULL;
					}
				
				 	
				
				} // Handles cases for empty value
				else {
					printf("Error: no Class field given for: %u, %s\n", card->id, card->name);
					strsep(&line, ",");
					free_card(card);
					return NULL;
				}
				

				break;
				
			// Rarity field
			case 8:
				if (line[0] == '"') {
				 	// Strips first quote 
				 	strsep(&line, "\"");
				 	// Strips closing quote
				 	temp_str = strsep(&line, "\"");
				 	strsep(&line, ",");
					 	
					if (strcmp(temp_str, "FREE") == 0){
						card->rarity = 0;
					} else if (strcmp(temp_str, "COMMON") == 0) {
						card->rarity = 1;
					} else if (strcmp(temp_str, "EPIC") == 0) {
						card->rarity = 2;
					} else if (strcmp(temp_str, "RARE") == 0) {
						card->rarity = 3;
					} else if (strcmp(temp_str, "LEGENDARY") == 0) {
						card->rarity = 4;
					} else {
						printf("Error: Could not identify keys for the Rarity field for: %u, %s\n", card->id, card->name);
						free_card(card);
						return NULL;
					}
				
				 	
				
				} // Handles cases for empty value 
				else {
					printf("Error: no Rarity field given for: %u, %s\n", card->id, card->name);
					strsep(&line, ",");
					free_card(card);
					return NULL;
				}

				break;
			
			// Unknown field
			default:
				printf("Error: Could not identify all fields\n");
				free_card(card);
				return NULL;
		} 
		
		if(line != NULL && strlen(line) == 0) line = NULL;
		++i;
	}
	
	return card;
}


void print_card(CARD_T *card) {
	printf("%-29s %2d\n", card->name, card->cost);
	unsigned length = 15 - strlen(class_str[card->class]);
	unsigned remainder = length % 2;
	unsigned margins = length / 2;
	unsigned left = 0;
	unsigned right = 0;
	if (remainder) {
		left = margins + 2;
		right = margins - 1;
	} else {
		left = margins + 1;
		right = margins - 1;
	}
	printf("%-6s %*s%s%*s %9s\n", type_str[card->type], left, "", class_str[card->class], right, "", rarity_str[card->rarity]);
	printf("--------------------------------\n");
	printf("%s\n", card->text);
	printf("--------------------------------\n");
	printf("%-16d%16d\n\n", card->attack, card->health);
}

