#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int	match(const char *pattern, const char *text)
{
	if (*pattern == '\0' && *text == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *text == '\0')
		return (0);
	if (*pattern == *text)
        return match(pattern+1, text+1);
	if (*pattern == '*')
		return (match(pattern + 1, text) || match(pattern, text + 1));
	return (0);
}

int main() {
	const char *pattern = "g*pand*i";
	const char *text = "gaaapandbbbbb";

	if (match(pattern, text))
		printf("Pattern matches the text.\n");
	else
		printf("Pattern does not match the text.\n");
	return 0;
}

