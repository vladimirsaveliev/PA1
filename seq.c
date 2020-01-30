#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char res[200];

/* this is to */
char tmp[100];

int total = 0;

void split(char *src, int len)
{
	int i;
	int n;

	if (*src == 0) {
			/* string res has space in the end, delete it */
		if (res[strlen(res) - 1] != ' ') {
			printf("Something wrong\n");
			exit(1);
		}
		res[strlen(res) - 1] = 0;

		/* output next variant */
		printf("%s\n", res);
		total++;
		return;
	}

	/* take beginning of string */
	for (i = 1; i <= strlen(src); i++) {		
		/* copy i chars to temporary buffer */
		strncpy(tmp, src, i);
		tmp[i] = 0;

		/* convert tmp to integer */
		n = atoi(tmp);
		/* check that it is multiple of 3 */
		if (n % 3 != 0)
			/* number is not multiple of 3, grab another char */
			continue;
		
		/* got number which is multiple of 3: copy it to result string */
		sprintf(res + len, "%s ", tmp);

		/* recursively repeat with rest of string */
		split(src + i, strlen(res));
	}
}

void validate_input(char *str)
{
	int len;
	int i;

	len = strlen(str);
	if (len < 2) {
		/* length of input must be at least 2: one number and '\n' */
		printf("Invalid input.\n");
		exit(1);
	}

	/* get rid of '\n' */
	if (str[len - 1] == '\n') {
		/* length of input is 0 */
		str[len - 1] = 0;
		len--;
	}
	if (len < 1 || len > 100) {
		printf("Invalid input.\n");
		exit(1);
	}
	/* check that all chars are digits */
	for (i = 0; i < len; i++) {
		if (!isdigit (str[i])) {
			printf("Invalid input.\n");
			exit(1);
		}
	}
}

int main(void)
{
	char input[102];

	if (fgets(input, 102, stdin) == NULL) {
		printf("Invalid input.\n");
		return(1);
	}

	validate_input(input);

	split(input, 0);
	printf("Total: %d\n", total);
	return 0;
}
