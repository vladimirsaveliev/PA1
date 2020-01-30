#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct item {
	char *ean;
	int price;
};

/* dynamic array of struct item */
struct item *all_items = NULL;
int cur_size = 0;
int max_size = 0;

int compare(const void *p1, const void *p2)
{
	const struct item *i1, *i2;

	i1 = (const struct item *)p1;
	i2 = (const struct item *)p2;

	return strcmp(i1->ean, i2->ean);
}

void add_item(char *eancode, int price)
{
	if (cur_size == max_size) {
		/* expand dynamic array */
		all_items = realloc(all_items, (max_size + 100) * sizeof(struct item));
		if (all_items == NULL) {
			printf("no memory");
			exit(1);
		}
		max_size += 100;
	}
	all_items[cur_size].ean = eancode;
	all_items[cur_size].price = price;
	cur_size++;

	/* sort array to speedup search */
	qsort(all_items, cur_size, sizeof(struct item), compare);
}

/* expect: EAN => price */
void validate_price_list(char *str)
{
	char *p;
	char *ean;
	char *p2;
	int price;

	p = strstr(str, "=>");
	if (p == NULL) {
		printf("Invalid input.\n");
		exit(1);
	}

	ean = strndup(str, p - str);
	if (ean == NULL) {
		printf("no memory");
		exit(1);
	}
	/* remove tailing spaces */
	p2 = ean + strlen(ean) - 1;
	while (*p2 == ' ') {
		*p2 = 0;
		p2--;
	}

	/* step over "=>" */
	p += 2;
	
	/* skip spaces */
	while (*p == ' ')
		p++;
	
	/* check that from p to end there are only digits */
	p2 = p;
	while(*p2) {
		if (!isdigit(*p2)) {
			printf("Invalid input\n");
			exit(1);
		}
		p2++;
	}
	price = atoi(p);
	

	/* ok, ean is EAN in allocated string, price is price */

	/* save in array */
	add_item(ean, price);
}

/* expect: EAN (all digits) */
/* return price */
int validate_bill_record(char *str)
{
	int i;
	int len;
	struct item *item;
	struct item key;

	len = strlen(str);
	if (len < 5 || len > 100) {
		printf("Invalid input.\n");
		exit(1);
	}
	for (i = 0; i < len; i++) {
		if (!isdigit(str[i])) {
			printf("Invalid input.\n");
			exit(1);
		}
	}	

	/* look for item in array */
	key.ean = str;
	item = bsearch(&key, all_items, cur_size, sizeof(struct item), compare);
	if (item) {
		return item->price;
	}
	/*	printf("%s: not found\n", key.ean)*/

	/* debug code */
/*	for (i = 0; i < cur_size; i++) {
		printf("%s: %d\n", all_items[i].ean, all_items[i].price);
		}*/
	return 0;
}

int main(void)
{
	/* assume that this is enough for "EAN(5-100 char) => price" */
	char input[150];
	int mode = 1; /* price list mode */
	int len;
	int i;
	int billtotal;

	printf("Price list:\n");
	while (fgets(input, 150, stdin)) {
		/* get rid of '\n' in end */
		len = strlen(input);
		if (input[len - 1] == '\n') {
			input[len - 1] = 0;
			len--;
		}

		if (!input[0]) {
			/* empty line: this is ether price list or
			 * current bill is over */
			mode++;

			if (mode > 2) {
				/* was in bill mode */
				printf("Total: %d\n", billtotal);
			}
			billtotal = 0;
			printf("Bill:\n");
			continue;
		}

		if (mode == 1) {
			/* price list */
			validate_price_list(input);
			continue;
		}

		billtotal += validate_bill_record(input);
		printf("%s\n", input);
	}

	/* for last bill */
	printf("Total: %d\n", billtotal);


	/* debug code */
/*
	for (i = 0; i < cur_size; i++) {
		printf("%s: %d\n", all_items[i].ean, all_items[i].price);
		}*/

	/* free memory */
	for (i = 0; i < cur_size; i++) {
		free(all_items[i].ean);
	}
	free(all_items);

	return 0;
}
