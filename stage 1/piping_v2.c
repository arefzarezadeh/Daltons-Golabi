#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define debug(x) printf("%s:%d\n", #x, x)

int n;
int *graph;
int *type;
int *direction;
bool *connected;
int maxLayer = 0; //<---------------------------
int *pgraph;

printGraph(int *graph)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            //printf("%d ", (direction[i * n + j] - initial_direction[i][j] + 4) % 4);
            printf("%2d ", *(graph + i * n + j));
        printf("\n");
    }
}

//--------directions---------
int up(int cell)
{
    if (cell / n == 0)
        return cell - n + n * n;
    return cell - n;
}
int right(int cell)
{
    if (cell % n == n - 1)
        return cell - n + 1;
    return cell + 1;
}
int down(int cell)
{
    if (cell / n == n - 1)
        return cell % n;
    return cell + n;
}
int left(int cell)
{
    if (cell % n == 0)
        return cell + n - 1;
    return cell - 1;
}


bool compareGraphs(int *g1, int *g2)
{
    for (int i = 0; i < n * n; i++)
    {
        if (*(g1 + i) != *(g2 + i))
            return false;
    }
    return true;
}

void setConnection(int cell)
{
    int k = *(direction + cell);
    switch (*(type + cell))
    {
    case 0:
        *(connected + cell * 4 + k % 4) = false;
        *(connected + cell * 4 + (k + 1) % 4) = true;
        *(connected + cell * 4 + (k + 2) % 4) = true;
        *(connected + cell * 4 + (k + 3) % 4) = true;
        break;
    case 1:
        *(connected + cell * 4 + k % 4) = true;
        *(connected + cell * 4 + (k + 1) % 4) = false;
        *(connected + cell * 4 + (k + 2) % 4) = false;
        *(connected + cell * 4 + (k + 3) % 4) = false;
        break;
    case 2:
        *(connected + cell * 4 + k % 4) = true;
        *(connected + cell * 4 + (k + 1) % 4) = true;
        *(connected + cell * 4 + (k + 2) % 4) = false;
        *(connected + cell * 4 + (k + 3) % 4) = false;
        break;
    case 3:
        *(connected + cell * 4 + k % 4) = true;
        *(connected + cell * 4 + (k + 1) % 4) = false;
        *(connected + cell * 4 + (k + 2) % 4) = true;
        *(connected + cell * 4 + (k + 3) % 4) = false;
        break;
    case 4:
        *(connected + cell * 4 + k % 4) = false;
        *(connected + cell * 4 + (k + 1) % 4) = true;
        *(connected + cell * 4 + (k + 2) % 4) = true;
        *(connected + cell * 4 + (k + 3) % 4) = true;
        break;
    }
}

bool isConnected(int cell1, int cell2)
{
    if (cell1 < cell2)
    {
        int tmp = cell2;
        cell2 = cell1;
        cell1 = tmp;
    }
    if (down(cell2) == cell1)
        return *(connected + 4 * cell1    ) && *(connected + 4 * cell2 + 2);
    if (right(cell2) == cell1)
        return *(connected + 4 * cell1 + 3) && *(connected + 4 * cell2 + 1);
    if (left(cell2) == cell1)
        return *(connected + 4 * cell1 + 1) && *(connected + 4 * cell2 + 3);
    if (up(cell2) == cell1)
        return *(connected + 4 * cell1 + 2) && *(connected + 4 * cell2    );
    return false;
}


bool checkLayer(int layer)
{
    int connected_to_last_layer = 0;
    int count = 0;
    for (int i = 0; i < n * n; i++)
    {
        if (graph[i] == layer)
        {
            count++;
            //checks if it is connected to only one cell from the previous layer
            if (((*(graph + up(i)) == layer - 1) && isConnected(i, up(i)))
                || ((*(graph + right(i)) == layer - 1) && isConnected(i, right(i)))
                || ((*(graph + down(i)) == layer - 1) && isConnected(i, down(i)))
                || ((*(graph + left(i)) == layer - 1) && isConnected(i, left(i))))
                connected_to_last_layer++;
            //checks if it is connected to a layer before the last layer
            if (((*(graph + up(i)) < layer - 1) && (*(graph + up(i)) != -1) && isConnected(i, up(i)))
                || ((*(graph + right(i)) < layer - 1) && (*(graph + right(i)) != -1) && isConnected(i, right(i)))
                || ((*(graph + down(i)) < layer - 1) && (*(graph + down(i)) != -1) && isConnected(i, down(i)))
                || ((*(graph + left(i)) < layer - 1) && (*(graph + left(i)) != -1) && isConnected(i, left(i))))
                return false;
            //checks if it is connected to the same layer
            if (((*(graph + up(i)) == layer) && isConnected(i, up(i)))
                || ((*(graph + right(i)) == layer) && isConnected(i, right(i)))
                || ((*(graph + down(i)) == layer) && isConnected(i, down(i)))
                || ((*(graph + left(i)) == layer) && isConnected(i, left(i))))
                return false;
        }
    }
    if (connected_to_last_layer == count)
        return true;
    return false;
}


bool checkAll()
{
    for (int i = 1; i <= maxLayer; i++)
    {
        if (!checkLayer(i))
            return false;
    }
    return true;
}

bool rotateLayer(int layer, int* pRotations)
{
    int count = 0;
    for (int i = 0; i < n * n; i++)
    {
        if (*(graph + i) == layer)
            count++;
    }

    //debug(count);

    int list[count];
    count = 0;
    for (int i = 0; i < n * n; i++)
    {
        if (*(graph + i) == layer)
        {
            list[count] = i;
            count++;
        }
    }

    for (int i = *pRotations + 1; i <= (1ll << (count * 2)); i++)
    {
        *pRotations += 1;
        for (int j = 0; j < count; j++)
        {
            int cell = list[j];
            if (i % (1 << (j * 2)) == 0)
            {
                direction[cell] = (direction[cell] + 1) % 4;
                setConnection(cell);
            }
        }
        if (checkLayer(layer))
            return true;
    }
    return false;
}

void removeLayer(int layer)
{
    for (int i = 0; i < n * n; i++)
    {
        if (*(graph + i) >= layer)
            *(graph + i) = -1;
    }
    return;
}

void addLayer(int layer)
{
    for (int i = 0; i < n * n; i++)// <---------------- adds a layer
    {
        if(graph[i] == layer)
        {
            if (*(connected + i * 4 + 0) && *(graph + up(i)) == -1)
                *(graph + up(i)) = layer + 1;
            if (*(connected + i * 4 + 1) && *(graph + right(i)) == -1)
                *(graph + right(i)) = layer + 1;
            if (*(connected + i * 4 + 2) && *(graph + down(i)) == -1)
                *(graph + down(i)) = layer + 1;
            if (*(connected + i * 4 + 3) && *(graph + left(i)) == -1)
                *(graph + left(i)) = layer + 1;
        }
    }
    return;
}

int cnt = 0;

bool containsLayer(int layer)
{
    for (int i = 0; i < n * n; i++)
    {
        if (*(graph + i) == layer)
            return true;
    }
    return false;
}

bool start(int layer)
{
    bool hasEmptyCells = false;
    for (int i = 0; i < n * n; i++)
    {
        if (*(graph + i) == -1)
            hasEmptyCells = true;
    }

    if (!hasEmptyCells)
    {
        if (checkAll())
            return true;
        return false;
    }


    if (!containsLayer(layer - 1))
        return false;



    for (int i = 0; i < n * n; i++)
        *(pgraph + i) = *(graph + i);
    //-----------------------------------------
    addLayer(layer - 1);
    maxLayer++;
    int rotations = 0;

    do
    {
        if (start(layer + 1))
            return true;
    } while (rotateLayer(layer, &rotations));
    removeLayer(layer);
    maxLayer--;
    return false;
}

int main()
{
    scanf("%d", &n);
    int emptyGraph[n][n];
    int initial_direction[n][n];
    graph = (int*) calloc(n * n, sizeof(int));
    pgraph = (int*) calloc(n * n, sizeof(int));
    type = (int*) calloc(n * n, sizeof(int));
    direction = (int*) calloc(n * n, sizeof(int));
    connected = (bool*) calloc(n * n * 4, 1);


    //start getting the input
    char getType;
    for (int i = 0; i < n * n; i++)
    {
        getchar();
        getType = getchar();
        switch (getType)
        {
        case 'S':
            type[i] = 0;
            break;
        case 'C':
            type[i] = 1;
            break;
        case 'L':
            type[i] = 2;
            break;
        case 'I':
            type[i] = 3;
            break;
        case 'T':
            type[i] = 4;
            break;
        }
        scanf("%d", &direction[i]);
    } //finish getting the input


    for (int i = 0; i < n * n; i++)
        setConnection(i);

    for (int i = 0; i < n * n; i++)
    {
        *(graph + i) = -1;
        emptyGraph[i / n][i % n] = -1;
        initial_direction[i / n][i % n] = *(direction + i);
        *(direction + i) = 0;
    }

    for (int i = 0; i < n * n; i++)
    setConnection(i);

    graph[n * n / 2] = 0;

    int x = 0;

    for (int i = 0; i < 4; i++)
    {
        if (start(1))
            break;
        *(direction + n * n / 2) += 1;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", (direction[i * n + j] - initial_direction[i][j] + 4) % 4);
            //printf("%2d ", graph[i * n + j]);
        printf("\n");
    }

    return 17;
}
