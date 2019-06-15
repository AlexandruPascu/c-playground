#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *strlwr(char *str) {
  unsigned char *p = (unsigned char *)str;
  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }
  return str;
}

int main() {
    // TODO: Corecteaza codul astfel incat sa respecte cerinta
    // HINT: Sunt 6 greseli
    char text[100];
    char word[10];

    fgets(text, 100, stdin);
    scanf("%s", word);

    strlwr(word);

    char *copy_of_text = strcpy(text); // Pointer to copy_of_text
    char *begin = copy_of_text;

    copy_of_text = strlwr(copy_of_text);
    copy_of_text = strstr(copy_of_text, "word");

    while (!copy_of_text) {
        for (int i = 0; i < strlen(word); ++i) {
            text[copy_of_text + begin - i] = toupper(*(copy_of_text + i));
        }
        copy_of_text = strstr(copy_of_text, "word");
    }

    printf("%s", text);

    free(begin);
}
