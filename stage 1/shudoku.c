#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int *table;
int size;
int **conditions;
int *belongs;
int countConditions;
int pCell;
int *emptyRow;
int *emptyCol;
int totalEmpty;

int emptyRowCells(int row)
{
    int cnt = 0;
    for (int i = 0; i < size; i++)
    {
        if (*(table + row * size + i) == 0)
            cnt++;
    }
    return cnt;
}

int emptyColCells(int col)
{
    int cnt = 0;
    for (int i = 0; i < size; i++)
    {
        if (*(table + col + size * i) == 0)
            cnt++;
    }
    return cnt;
}

int emptyCells()
{
    int count = 0;
    for (int i = 0; i < size * size; i++)
    {
        if (table[i] == 0)
            count++;
    }
    return count;
}

int findBestCondition() //returns n>=0 for conditions int int**, -(2n+3) for row and -2(n+1) for col
{
    if (!emptyCells)
        return -1;
    int min = size + 1, tmp, index = -1, priority = 6;

    for (int i = 0; i < size; i++)
    {
        tmp = emptyRow[i];
        if ((tmp != 0) && (tmp < min))
            index = -1 * (2 * i + 3), min = tmp;
        tmp = emptyCol[i];
        if ((tmp != 0) && (tmp < min))
            index = -2 * (i + 1), min = tmp;
    }
    if (tmp == 1)
        return index;

    for (int i = 0; i < countConditions; i++)
    {
        tmp = *(*(conditions + i) + *(*(conditions + i) + 2) + 3);
        if (tmp != 0)
        {
            if (tmp < min)
            {
                min = tmp;
                index = i;
                priority = **(conditions + i);
            }
            else if (tmp == min)
            {
                if (priority > **(conditions + i))
                {
                    min = tmp;
                    index = i;
                    priority = **(conditions + i);
                }
            }
        }
    }
    return index;
}

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
    bool hasZero = false;
    int sum = 0;
    for (int i = 0; i < *(*(conditions + cnd) + 2); i++)
    {
        if (*(table + *(*(conditions + cnd) + i + 3)) == 0)
            hasZero = true;
        else
            sum += *(table + *(*(conditions + cnd) + i + 3));
    }

    if (sum > *(*(conditions + cnd) + 1))
        return false;
    if (hasZero || sum == *(*(conditions + cnd) + 1))
        return true;
    else
        return false;
}

bool checkProduct(int cnd)
{
    bool hasZero = false;
    int prod = 1;
    for (int i = 0; i < *(*(conditions + cnd) + 2); i++)
    {
        if (*(table + *(*(conditions + cnd) + i + 3)) == 0)
            hasZero = true;
        else if (*(*(conditions + cnd) + 1) % *(table + *(*(conditions + cnd) + i + 3)) != 0)
            return false;
        else
            prod *= *(table + *(*(conditions + cnd) + i + 3));
    }

    if (prod > *(*(conditions + cnd) + 1))
        return false;
    if (hasZero || prod == *(*(conditions + cnd) + 1))
        return true;
    return false;
}

bool checkMinus(int cnd)
{
    if ((*(table + *(*(conditions + cnd) + 3)) != 0) && ((size < *(table + *(*(conditions + cnd) + 3)) + *(*(conditions + cnd) + 1)) && (*(table + *(*(conditions + cnd) + 3)) <= *(*(conditions + cnd) + 1))))
        return false;
    if ((*(table + *(*(conditions + cnd) + 4)) != 0) && ((size < *(table + *(*(conditions + cnd) + 4)) + *(*(conditions + cnd) + 1)) && (*(table + *(*(conditions + cnd) + 4)) <= *(*(conditions + cnd) + 1))))
        return false;
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
    case 2:
        return checkPlus(cnd);
    case 3:
        return checkMinus(cnd);
    case 1:
        return checkProduct(cnd);
    case 4:
        return checkDivision(cnd);
    case 5:
        return checkMod(cnd);
    }
}

bool checkAll(int cell)
{
    return checkColumn(cell % size) && checkRow(cell / size) && checkConditions(cell);
}

bool solve()
{
    int cell, cnd = findBestCondition();
    //printf("cnd:%d\n", cnd);
    if (cnd == -1)
    {
        for (int i = 0; i < size * size; i++)
        {
            if (!checkAll(i))
                return false;
        }
        return true;
    }
    if (cnd > -1)
    {
        for (int i = 0; i < *(*(conditions + cnd) + 2); i++)
        {
            if (table[*(*(conditions + cnd) + 3 + i)] == 0)
            {
                cell = *(*(conditions + cnd) + 3 + i);
            }
        }
    }
    else
    {
        cnd *= -1;
        int rc = cnd / 2 - 1;

        if (cnd % 2 == 1)
        {
            for (int i = 0; i < size; i++)
            {
                if (table[rc * size + i] == 0)
                {
                    cell = rc * size + i;
                    if (pCell != cell)
                        break;
                }
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                if (table[rc + size * i] == 0)
                {
                    cell = rc + size * i;
                    if (pCell != cell)
                        break;
                }
            }
        }
    }


    //printf("cell:%d\n", cell);


    /*if (*(table + cell) != 0)
        return solve(cell + 1);*/
    *(*(conditions + *(belongs + cell)) + *(*(conditions + *(belongs + cell)) + 2) + 3) -= 1;
    emptyCol[cell % size] -= 1;
    emptyRow[cell / size] -= 1;
    totalEmpty--;
    for (int i = 1; i <= size; i++)
    {
        *(table + cell) = i;
        if (checkAll(cell))
        {
            if (solve())
                return true;
        }
    }

    *(table + cell) = 0;
    *(*(conditions + *(belongs + cell)) + *(*(conditions + *(belongs + cell)) + 2) + 3) += 1;
    emptyCol[cell % size] += 1;
    emptyRow[cell / size] += 1;
    totalEmpty++;
    return false;
}

int main()
{
    int k, length;
    char operatorChar;
    scanf("%d%d", &size, &k);
    countConditions = k;
    table = (int *) calloc(size * size, sizeof(int));
    belongs = (int *) malloc(size * size * sizeof(int));
    conditions = (int **) malloc(k * sizeof(int*));
    emptyCol = (int *) malloc(size * size * sizeof(int));
    emptyRow = (int *) malloc(size * size * sizeof(int));

    for (int i = 0; i < k; i++)
    {
        scanf("%d", &length);
        *(conditions + i) = (int *) malloc((length + 4) * sizeof(int));
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
            **(conditions + i) = 2;
            break;
        case '-':
            **(conditions + i) = 3;
            break;
        case '*':
            **(conditions + i) = 1;
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
        *(*(conditions + i) + length + 3) = *(*(conditions + i) + 2);
        if (**(conditions + i) == 0)
        {
            *(table + *(*(conditions + i) + 3)) = *(*(conditions + i) + 1);
            *(*(conditions + i) + length + 3) = 0;
        }
    }

    for (int i = 0; i < size; i++)
    {
        emptyCol[i] = emptyColCells(i);
        emptyRow[i] = emptyRowCells(i);
    }
    totalEmpty = emptyCells();
    /*table[0] = 0;
    table[1] = 2;
    table[2] = 0;
    table[3] = 1;*/

    //printf("solve(0):%d\n", solve());
    /*printf("checkRow(0):%d\n", checkRow(0));
    printf("checkRow(1):%d\n", checkRow(1));
    printf("checkCol(0):%d\n", checkColumn(0));
    printf("checkCol(1):%d\n", checkColumn(1));
    printf("checkConditions(1):%d\n", checkConditions(1));
    printf("checkConditions(0):%d\n", checkConditions(0));
    printf("checkConditions(1):%d\n", checkConditions(1));
    printf("checkConditions(2):%d\n", checkConditions(2));
    printf("checkConditions(3):%d\n", checkConditions(3));
    printf("emptyRowCells(0):%d\n", emptyRowCells(0));
    printf("emptyRowCells(1):%d\n", emptyRowCells(1));
    printf("emptyRowCells(2):%d\n", emptyRowCells(2));
    printf("best:%d\n", findBestCondition());*/
    solve();
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
