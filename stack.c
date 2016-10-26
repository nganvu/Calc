#include <stdio.h>
#include <stdlib.h>
#include "/c/cs223/hw4/stack.h"

void StackInit(stackT *stackP, int maxSize)
{
  stackP->contents = calloc(maxSize, sizeof(Token));
  stackP->maxSize = maxSize;
  stackP->top = 0;
  stackP->count = 0;
}

void StackDestroy(stackT *stackP)
{
  for (int i = 0; i < stackP->count; i++)
  {
    free(stackP->contents[i]->str);
    free(stackP->contents[i]);
  }
  free(stackP->contents);
  free(stackP);
}

void StackPush(stackT *stackP, Token element)
{
  if (StackIsFull(stackP))
  {
    fprintf(stderr, "Stack is full");
    return;
  }
  stackP->contents[stackP->count] = element;
  stackP->count++;
  stackP->top = stackP->count - 1;
}

Token StackPop(stackT *stackP)
{
  stackP->count--;
  stackP->top--;
  return stackP->contents[stackP->count];
}

bool StackIsEmpty(stackT *stackP)
{
  if (stackP->count == 0)
    return true;
  else
    return false;
}

bool StackIsFull(stackT *stackP)
{
  if (stackP->count == stackP->maxSize)
    return true;
  else
    return false;
}

int StackCount(stackT *stackP)
{
  return (stackP->count);
}

void StackPrint(stackT *stackP)
{
  if (!StackIsEmpty(stackP))
  {
    for (int i = stackP->top; i >= 0; i--)
    {
      printf("[%d %s %.1lf] ",
        (stackP->contents[i])->type, (stackP->contents[i])->str, (stackP->contents[i])->value);
    }
  }
  putchar('\n');
}

Token StackTop(stackT *stackP)
{
  return (stackP->contents[stackP->top]);
}
