#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include "/c/cs223/hw4/stack.h"

int main (int argc, char** argv)
{
  if ((argc > 2) || (argc == 2 && (strcmp(argv[1], "-debug") != 0)))
  {
    fprintf(stderr, "Usage: Calc [-debug]\n");
    return -1;
  }
  bool debug = false;
  if (argc == 2)
    debug = true;
  char *buffer = NULL;
  unsigned long len;

  while (getline(&buffer, &len, stdin) != -1)
  {
    printf("Input: %s", buffer);
    buffer[strlen(buffer) - 1] = '\0';

    char* token;
    const char space[2] = " ";
    token = strtok(buffer, space);

    stackT* operatorStack = malloc(sizeof(stackT));
    assert(operatorStack);
    StackInit(operatorStack, 512);
    if (debug)
      printf("Stack: size: %d :\n", operatorStack->count);

    stackT* outputStack = malloc(sizeof(stackT));
    assert(outputStack);
    StackInit(outputStack, 1024);

    int parenCount = 0;

    while (token != NULL)
    {
      double tokenValue = strtod(token, NULL);
      Token element = malloc(sizeof(struct token));
      assert(element);
      element->str = malloc((strlen(token) + 1) * sizeof(char));
      assert(element->str);

      if (tokenValue > 0)
        element->type = NUM;
      else if (token[0] == '0')
        element->type = NUM;
      else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
        element->type = OP1;
      else if (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
        element->type = OP2;
      else if (strcmp(token, "(") == 0)
        element->type = LPAR;
      else if (strcmp(token, ")") == 0)
        element->type = RPAR;
      else
      {
        fprintf(stderr, "Fatal Error. Bad token: %s\n", token);
        return -1;
      }
      element->value = tokenValue;
      strcpy(element->str, token);
      if (debug)
        printf("Token:%s: type: %d value: %.2lf\n",
          element->str, element->type, element->value);

      if (element->type == NUM)
        StackPush(outputStack, element);
      else //operators
      {
        if (element->type == OP1)
        {
          while (!StackIsEmpty(operatorStack) && (operatorStack->contents[operatorStack->top])->type != LPAR)
          {
            StackPush(outputStack, operatorStack->contents[operatorStack->top]);
            StackPop(operatorStack);
          }
          StackPush(operatorStack, element);
        }
        else if (element->type == OP2)
        {
          if (!StackIsEmpty(operatorStack) && (operatorStack->contents[operatorStack->top])->type == OP2)
          {
            StackPush(outputStack, operatorStack->contents[operatorStack->top]);
            StackPop(operatorStack);
            StackPush(operatorStack, element);
          }
          else
            StackPush(operatorStack, element);
        }
        else if (element->type == LPAR)
        {
          parenCount++;
          StackPush(operatorStack, element);
        }
        else if (element->type == RPAR)
        {
          parenCount--;
          while ((operatorStack->contents[operatorStack->top])->type != LPAR)
          {
            StackPush(outputStack, operatorStack->contents[operatorStack->top]);
            StackPop(operatorStack);
          }
          StackPop(operatorStack);
        }
      }
      if (debug)
      {
        printf("Stack: size: %d :", operatorStack->count);
        StackPrint(operatorStack);
      }
      token = strtok(NULL, space);
    }

    if (parenCount != 0)
    {
      fprintf(stderr, "Error: Mismatched parentheses\n");
      StackDestroy(outputStack);
      return -1;
    }

    while (!StackIsEmpty(operatorStack))
    {
      StackPush(outputStack, operatorStack->contents[operatorStack->top]);
      StackPop(operatorStack);
    }
    StackDestroy(operatorStack);

    if (debug)
      printf("OUTPUT:\n");

    stackT* evaluateStack = malloc(sizeof(stackT));
    assert(evaluateStack);
    StackInit(evaluateStack, 512);

    for (int i = 0; i < outputStack->count; i++)
    {
      //printf("content: %s\n", outputStack->contents[i]->str);
      if (debug)
      {
        printf("Stack: size: %d :", evaluateStack->count);
        StackPrint(evaluateStack);
        printf("Token:%s: type: %d value: %.2lf\n",
        outputStack->contents[i]->str,
        outputStack->contents[i]->type,
        outputStack->contents[i]->value);
      }

      if (outputStack->contents[i]->type == NUM)
        StackPush(evaluateStack, outputStack->contents[i]);
      else
      {
        Token firstNum = evaluateStack->contents[evaluateStack->top - 1];
        Token secondNum = evaluateStack->contents[evaluateStack->top];
        char* operator = outputStack->contents[i]->str;

        Token element = malloc(sizeof(struct token));
        assert(element);
        element->str = malloc((strlen(secondNum->str) + 1) * sizeof(char));
        assert(element->str);

        element->type = NUM;
        strcpy(element->str, secondNum->str);

        if (strcmp(operator, "+") == 0)
          element->value = firstNum->value + secondNum->value;
        else if (strcmp(operator, "-") == 0)
          element->value = firstNum->value - secondNum->value;
        else if (strcmp(operator, "*") == 0)
          element->value = firstNum->value * secondNum->value;
        else if (strcmp(operator, "/") == 0)
          element->value = firstNum->value / secondNum->value;
        StackPop(evaluateStack);
        StackPop(evaluateStack);
        StackPush(evaluateStack, element);
      }
    }
    StackDestroy(outputStack);
    printf("Result: %.2f\n", evaluateStack->contents[0]->value);
    StackDestroy(evaluateStack);
  }
}
