#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 256

typedef char Item;
#include "Stack.h"

int isBalanced(const char *str, int length)
{
  Stack *stack = createStack(); // create a stack to store opening parentheses
  int i;

  for (i = 0; i < length; i++) {
    char c = str[i];

    // check if the current character is an opening parenthesis
    if (c == '(' || c == '{' || c == '[') {

      push(stack, c); // push the opening parenthesis onto the stack
    }
    else if (c == ')' || c == '}' || c == ']') {

      if (isStackEmpty(stack)) {

        // if stack is empty, there is no matching opening parenthesis.
        destroyStack(stack);
        return 0;
      }

      char topChar = top(stack);
      if ((c == ')' && topChar == '(') || (c == '}' && topChar == '{') || (c == ']' && topChar == '['))
      {
        // If the closing parenthesis matches the opening parenthesis at the top of the stack
        // pop the opening parenthesis from the stack.
        pop(stack);
      }
      else
      {

        // If the closing parenthesis does not match the opening parenthesis at the top of the stack
        // the string is not balanced.
        destroyStack(stack);
        return 0;
      }
    }
  }

  // If the stack is empty, the string is balanced.
  int result = isStackEmpty(stack);
  destroyStack(stack);
  return result;
}

int main()
{
  int len;
  char buffer[MAX_INPUT_LEN];
  FILE *inputFile = fopen("input-parantheses.txt", "r");
  if (inputFile == NULL)
    return 1;

  while (fgets(buffer, MAX_INPUT_LEN, inputFile) != NULL)
  {
    buffer[strcspn(buffer, "\r\n")] = 0;
    len = strlen(buffer);
    if (len == 0)
      break;

    if (isBalanced(buffer, len))
      printf("%s ---> is balanced.\n", buffer);
    else
      printf("%s ---> not balanced.\n", buffer);
  }

  fclose(inputFile);

  return 0;
}
