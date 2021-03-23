/*
In the card game poker, a hand consists of five cards and are ranked, from lowest to highest, in the following way:

High Card: Highest value card.
One Pair: Two cards of the same value.
Two Pairs: Two different pairs.
Three of a Kind: Three cards of the same value.
Straight: All cards are consecutive values.
Flush: All cards of the same suit.
Full House: Three of a kind and a pair.
Four of a Kind: Four cards of the same value.
Straight Flush: All cards are consecutive values of same suit.
Royal Flush: Ten, Jack, Queen, King, Ace, in same suit.
The cards are valued in the order:
2, 3, 4, 5, 6, 7, 8, 9, 10, Jack, Queen, King, Ace.

If two players have the same ranked hands then the rank made up of the highest value wins; for example, a pair of eights beats a pair of fives (see example 1 below). But if two ranks tie, for example, both players have a pair of queens, then highest cards in each hand are compared (see example 4 below); if the highest cards tie then the next highest cards are compared, and so on.

Consider the following five hands dealt to two players:

Hand	 	Player 1	 	Player 2	 	Winner
1	 	5H 5C 6S 7S KD
Pair of Fives
 	2C 3S 8S 8D TD
Pair of Eights
 	Player 2
2	 	5D 8C 9S JS AC
Highest card Ace
 	2C 5C 7D 8S QH
Highest card Queen
 	Player 1
3	 	2D 9C AS AH AC
Three Aces
 	3D 6D 7D TD QD
Flush with Diamonds
 	Player 2
4	 	4D 6S 9H QH QC
Pair of Queens
Highest card Nine
 	3D 6D 7H QD QS
Pair of Queens
Highest card Seven
 	Player 1
5	 	2H 2D 4C 4D 4S
Full House
With Three Fours
 	3C 3D 3S 9S 9D
Full House
with Three Threes
 	Player 1
The file, poker.txt, contains one-thousand random hands dealt to two players. Each line of the file contains ten cards (separated by a single space): the first five are Player 1's cards and the last five are Player 2's cards. You can assume that all hands are valid (no invalid characters or repeated cards), each player's hand is in no specific order, and in each hand there is a clear winner.

How many hands does Player 1 win?

Author: https://github.com/ravikumark815

Answer: 376
*/

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HANDSIZE 5
#define FILENAME "p054_poker.txt"

/* all possible poker hand ranks, from lower to higher */
enum hands {
	HIGH_CARD,       /* 0 */
	PAIR,            /* 1 */
	TWO_PAIR,        /* 2 */
	THREE,           /* 3 */
	STRAIGHT,        /* 4 */
	FLUSH,           /* 5 */
	FULL_HOUSE,      /* 6 */
	FOUR,            /* 7 */
	STRAIGHT_FLUSH,  /* 8 */
	ROYAL_FLUSH      /* 9 */
};

enum suit {DIAMONDS, CLUBS, HEARTS, SPADES};

struct card {
	int8_t value;     /* goes from 2 to 14 (J=11, Q=12, K=13, A=14) */
	enum suit color;  /* one of the 4 possible suits */
};

/* parse_card: extract card value+suit from string `s` ("5C" -> {5, CLUBS}) */
struct card parse_card(char *s)
{
	int32_t len = strlen(s);
	struct card c;

	/* 10 is written as T, so there should be 2 char in the parsed string */
	if (len != 2)
	{
		printf("ERROR: can't parse %s\n", s);
		exit(1);
	}

	/* parse value */
	else
	{	/* 10 is represented as T, not 10 */
		if (isdigit(s[0]))
			c.value = s[0] - '0';
		else
			switch (s[0])
			{
				case 'T':
					c.value = 10;
					break;
				case 'J':
					c.value = 11;
					break;
				case 'Q':
					c.value = 12;
					break;
				case 'K':
					c.value = 13;
					break;
				case 'A':
					c.value = 14;
					break;
			}
	}

	/* parse suit */
	switch (s[len-1])
	{
		case 'D':
			c.color = DIAMONDS;
			break;
		case 'C':
			c.color = CLUBS;
			break;
		case 'H':
			c.color = HEARTS;
			break;
		case 'S':
			c.color = SPADES;
			break;
	}

	return c;
}

/* is_straight: return 1 if hand contains a straight (5 consecutive values)
 * given the number of occurrences of each value in hand */
int32_t is_straight(int32_t *count_value)
{
	int32_t i, j;

	/* A straight = 5 cards with successive value has a count of 1. Each
	 * time a count of 1 is found, check the count of successive value until
	 * a count != 1 is found. If starting index - stopping index is 5, it is
	 * a straight */
	for (i = 0; i < 11; ++i)
	{
		if (count_value[i] == 0)
			continue;
		for (j = i+1; j < 15 && count_value[j] == 1; ++j)
			;
		if (j-i == 5)
			return 1;
	}
	return 0;
}

/* is_flush: return 1 if the hand contains a flush (all cards have same suit) */
int32_t is_flush(int32_t *count_suit)
{
	return (count_suit[0] == HANDSIZE || count_suit[1] == HANDSIZE
	     || count_suit[2] == HANDSIZE || count_suit[3] == HANDSIZE);
}

/* is_three: return 1 if the hand contains three cards with the same value */
int32_t is_three(int32_t *count_value)
{
	int32_t i;
	for (i = 0; i < 15; ++i)
		if (count_value[i] == 3)
			return 1;
	return 0;
}

/* is_pair: return the number of pairs in hand (2 if TWO_PAIR, 1 if PAIR, 0 if
 * no pair) */
int32_t is_pair(int32_t *count_value)
{
	int32_t i, n;
	for (i = 0, n = 0; i < 15; ++i)
		if (count_value[i] == 2)
			++n;
	return n;
}

/* find_rank: find the rank of `hand` (PAIR, THREE, FOUR ... ROYAL_FLUSH) */
enum hands find_rank(struct card *hand)
{
	/* count occurrence of each value and suit in hand */
	int32_t i, count_value[15] = {0}, count_suit[4] = {0};
	for (i = 0; i < HANDSIZE; ++i)
	{
		++count_value[hand[i].value];
		++count_suit[hand[i].color];
	}

	/* test if ROYAL_FLUSH */
	if (count_value[10] == 1 && count_value[11] == 1 && count_value[12] == 1
	&&  count_value[13] == 1 && count_value[14] == 1
	&&  is_flush(count_suit))
		return ROYAL_FLUSH;

	/* test STRAIGHT_FLUSH or STRAIGHT */
	if (is_straight(count_value))
		return is_flush(count_suit) ? STRAIGHT_FLUSH : STRAIGHT;

	/* test FOUR (4 cards with same value). Either first or second card of
	 * hand has an occurrence of 4 */
	if (count_value[hand[0].value] == 4 || count_value[hand[1].value] == 4)
		return FOUR;

	/* test FULL_HOUSE (3 of a kind and a pair) or THREE */
	if (is_three(count_value))
		return is_pair(count_value) ? FULL_HOUSE : THREE;

	/* test FLUSH */
	if (is_flush(count_suit))
		return FLUSH;

	/* test if 1 or 2 pairs, or no pairs. Number of pairs match the rank
	 * value of hand (0 for HIGH_CARD, 1 for PAIR, 2 for TWO_PAIR */
	return is_pair(count_value);
}

/* handle_tie: return 1 (or 2) if hand_p1 (or hand_p2) is the winner. This is
 * the case where both hands have rank `̣̣rank` */
int32_t handle_tie(struct card *hand_p1, struct card *hand_p2, enum hands rank)
{
	/* count occurrence of each value in hands P1/P2 */
	int32_t count_value1[15] = {0}, count_value2[15] = {0};
	int32_t i, val1, val2;
	for (i = 0; i < HANDSIZE; ++i)
	{
		++count_value1[hand_p1[i].value];
		++count_value2[hand_p2[i].value];
	}

	/* both hands have a single pair, find value of pair and compare them */
	if (rank == 1)
	{
		for (i = 0; i < 15; ++i)
		{
			if (count_value1[i] == 2)
				val1 = i;
			if (count_value2[i] == 2)
				val2 = i;
		}

		if (val1 != val2)
			return val1 > val2 ? 1 : 2;
	}

	/* need to compare highest values to find winner */
	for (i = 14; i >= 0; --i)
	{
		if (count_value1[i] == 1 && count_value2[i] == 0)
			return 1;
		if (count_value2[i] == 1 && count_value1[i] == 0)
			return 2;
	}

	return 0;
}

/* given a list of poker hands for P1/P2, print the number of times P1 wins */
int32_t main(void)
{
	FILE *fp;
	struct card *hand_p1, *hand_p2;
	int32_t i, victory_p1, rank_p1, rank_p2;
	char buf[10];

	hand_p1 = calloc(HANDSIZE, sizeof *hand_p1);
	hand_p2 = calloc(HANDSIZE, sizeof *hand_p2);

	if ((fp = fopen(FILENAME, "r")) == NULL)
	{
		printf("ERROR: can't open %s\n", FILENAME);
		return 1;
	}

	i = victory_p1 = 0;
	while (fscanf(fp, "%s", buf) != EOF)
	{
		/* each line contains 2*HANDSIZE strings (card). First half is
		 * P1 hand, second half is P2 hand */
		if (i < HANDSIZE)
			hand_p1[i] = parse_card(buf);
		else
			hand_p2[i-HANDSIZE] = parse_card(buf);
		++i;

		/* if 2*HANDSIZE values are read, compare the 2 hands */
		if (i == 2*HANDSIZE)
		{
			rank_p1 = find_rank(hand_p1);
			rank_p2 = find_rank(hand_p2);

			if (rank_p1 > rank_p2)
				++victory_p1;
			else if (rank_p1 == rank_p2)
				if (handle_tie(hand_p1, hand_p2, rank_p1) == 1)
					++victory_p1;

			i = 0; /* reset i to read next hands for P1/P2 */
		}
	}

	printf("Problem 54: %d\n", victory_p1);
	return 0;
}