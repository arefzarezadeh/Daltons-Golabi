#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int *table;
int size;
int **conditions;
int *belongs;

bool checkColumn(int col)
{
    for (int i = 0; i < size; i++)
    {
        if (*(table + col + size * i) != 0)
        {
            for (int j = i + 1;  j < size; j++)
            {
                if (*(table + col + size * i) == *(table + col + size * j))
                    return false;
            }
        }
    }
    return true;
}

bool checkRow(int row)
{
    for (int i = 0; i < size; i++)
    {
        if ((*(table + row * size + i) != 0))
        {
            for (int j = i + 1;  j < size; j++)
            {
                if (*(table + row * size + i) == *(table + row * size + j))
                    return false;
            }
        }
    }
    return true;
}

bool checkPlus(int cnd)
{
    int sum = 0;
    for (int i = 0; i < *(*(conditions + cnd) + 2); i++)
    {
        if (*(table + *(*(conditions + cnd) + i + 3)) == 0)
            return true;
        sum += *(table + *(*(conditions + cnd) + i + 3));
    }

    if (sum == *(*(conditions + cnd) + 1))
        return true;
    else
        return false;
}

bool checkProduct(int cnd)
{
    int prod = 1;
    for (int i = 0; i < *(*(conditions + cnd) + 2); i++)
    {
        if (*(table + *(*(conditions + cnd) + i + 3)) == 0)
            return true;
        prod *= *(table + *(*(conditions + cnd) + i + 3));
    }

    if (prod == *(*(conditions + cnd) + 1))
        return true;
    else
        return false;
}

bool checkMinus(int cnd)
{
    if ((*(table + *(*(conditions + cnd) + 3)) == 0) || (*(table + *(*(conditions + cnd) + 4)) == 0))
        return true;
    if (*(table + *(*(conditions + cnd) + 3)) - *(table + *(*(conditions + cnd) + 4)) == *(*(conditions + cnd) + 1))
        return true;
    if (*(table + *(*(conditions + cnd) + 4)) - *(table + *(*(conditions + cnd) + 3)) == *(*(conditions + cnd) + 1))
        return true;
    return false;
}

bool checkDivision(int cnd)
{
    if ((*(table + *(*(conditions + cnd) + 3)) == 0) || (*(table + *(*(conditions + cnd) + 4)) == 0))
        return true;
    if (*(table + *(*(conditions + cnd) + 3)) / *(table + *(*(conditions + cnd) + 4)) == *(*(conditions + cnd) + 1))
        return true;
    if (*(table + *(*(conditions + cnd) + 4)) / *(table + *(*(conditions + cnd) + 3)) == *(*(conditions + cnd) + 1))
        return true;
    return false;
}

bool checkMod(int cnd)
{
    int big, small;
    if ((*(table + *(*(conditions + cnd) + 3)) == 0) || (*(table + *(*(conditions + cnd) + 4)) == 0))
        return true;
    if (*(table + *(*(conditions + cnd) + 3)) > *(table + *(*(conditions + cnd) + 4)))
    {
        big = *(table + *(*(conditions + cnd) + 3));
        small = *(table + *(*(conditions + cnd) + 4));
    }
    else
    {
        small = *(table + *(*(conditions + cnd) + 3));
        big = *(table + *(*(conditions + cnd) + 4));
    }
    if (big % small == *(*(conditions + cnd) + 1))
        return true;
    return false;
}

bool checkConditions(int cell)
{
    int cnd = *(belongs + cell);
    switch (**(conditions + cnd))
    {
    case 1:
        return checkPlus(cnd);
    case 2:
        return checkMinus(cnd);
    case 3:
        return checkProduct(cnd);
    case 4:
        return checkDivision(cnd);
    case 5:
        return checkMod(cnd);
    }
}

bool checkAll(int cell)
{
    return checkConditions(cell) && checkColumn(cell % size) && checkRow(cell / size);
}

bool solve(int cell)
{
    if (cell == size * size)
        return true;

    if (*(table + cell) != 0)
        return solve(cell + 1);
    for (int i = 1; i <= size; i++)
    {
        *(table + cell) = i;
        if (checkAll(cell))
        {
            if (solve(cell + 1))
                return true;
        }
    }

    *(table + cell) = 0;
    return false;
}

int main()
{
    int k, length;
    char operatorChar;
    scanf("%d%d", &size, &k);
    table = (int *) calloc(size * size, sizeof(int));
    belongs = (int *) malloc(size * size * sizeof(int));
    conditions = (int **) malloc(k * sizeof(int*));

    for (int i = 0; i < k; i++)
    {
        scanf("%d", &length);
        *(conditions + i) = (int *) malloc((length + 3) * sizeof(int));
        *(*(conditions + i) + 2) = length;
        for (int j = 3; j < length + 3; j++)
        {
            scanf("%d", &(*(*(conditions + i) + j)));
            *(belongs + *(*(conditions + i) + j)) = i;
        }
        getchar();
        operatorChar = getchar();
        switch (operatorChar)
        {
        case '=':
            **(conditions + i) = 0;
            break;
        case '+':
            **(conditions + i) = 1;
            break;
        case '-':
            **(conditions + i) = 2;
            break;
        case '*':
            **(conditions + i) = 3;
            break;
        case '/':
            **(conditions + i) = 4;
            break;
        case '%':
            **(conditions + i) = 5;
            break;
        default:
            **(conditions + i) = -1;
            break;
        }
        scanf("%d", &(*(*(conditions + i) + 1)));
        if (**(conditions + i) == 0)
        {
            *(table + *(*(conditions + i) + 3)) = *(*(conditions + i) + 1);
        }
    }


    solve(0);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", *(table + i * size + j));
        }
        printf("\n");
    }

    return 17;
}
