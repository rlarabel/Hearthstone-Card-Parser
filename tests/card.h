#ifndef __CARD_H__
#define __CARD_H__

typedef enum {
	HERO,
	MINION,
	SPELL,
	WEAPON,
} Type;

typedef enum {
	DEMONHUNTER,
	DRUID,
	HUNTER,
	MAGE,
	NEUTRAL,
	PALADIN,
	PRIEST,
	ROGUE,
	SHAMAN,
	WARLOCK,
	WARRIOR,
} Class;

typedef enum {
	FREE,
	COMMON,
	EPIC,
	RARE,
	LEGENDARY,
} Rarity;

typedef struct {
	unsigned id;
	char *name;
	unsigned cost;
	Type type;
	Class class;
	Rarity rarity;
	char *text;
	unsigned attack;
	unsigned health;
} CARD_T;

extern CARD_T **cards;
extern size_t total_cards;
#endif

