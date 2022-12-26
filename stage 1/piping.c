#include<stdio.h>
#include<stdbool.h>

#define debug(x) printf("%s:%d\n", #x, x)

int cnt = 0;

void printGraph(int n, int graph[n][n])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
}

//--------directions---------
int up(int n, int cell)
{
    if (cell / n == 0)
        return cell - n + n * n;
    return cell - n;
}
int right(int n, int cell)
{
    if (cell % n == n - 1)
        return cell - n + 1;
    return cell + 1;
}
int down(int n, int cell)
{
    if (cell / n == n - 1)
        return cell % n;
    return cell + n;
}
int left(int n, int cell)
{
    if (cell % n == 0)
        return cell + n - 1;
    return cell - 1;
}

int getValue(int n, int cell, int graph[n][n])
{
    return graph[cell / n][cell % n];
}

bool compareGraphs(int n, int g1[n][n], int g2[n][n])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (g1[i][j] != g2[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

//--------configures the connection-----------
void setConnection(int n, int cell, int type[n][n], int direction[n][n], bool* connect)
{
    int k = direction[cell / n][cell % n];
    switch (type[cell / n][cell % n])
    {
    case 0:
        connect[k % 4] = false;
        connect[(k + 1) % 4] = true;
        connect[(k + 2) % 4] = true;
        connect[(k + 3) % 4] = true;
        break;
    case 1:
        connect[k % 4] = true;
        connect[(k + 1) % 4] = false;
        connect[(k + 2) % 4] = false;
        connect[(k + 3) % 4] = false;
        break;
    case 2:
        connect[k % 4] = true;
        connect[(k + 1) % 4] = true;
        connect[(k + 2) % 4] = false;
        connect[(k + 3) % 4] = false;
        break;
    case 3:
        connect[k % 4] = true;
        connect[(k + 1) % 4] = false;
        connect[(k + 2) % 4] = true;
        connect[(k + 3) % 4] = false;
        break;
    case 4:
        connect[k % 4] = false;
        connect[(k + 1) % 4] = true;
        connect[(k + 2) % 4] = true;
        connect[(k + 3) % 4] = true;
        break;
    }
}

//---------checks if the two cells are connected-----------
bool isConnected(int n, int cell1, int cell2, bool connection[n][n][4])
{
    if (cell1 < cell2)
    {
        int tmp = cell2;
        cell2 = cell1;
        cell1 = tmp;
    }
    if (down(n, cell2) == cell1)
        return connection[cell1 / n][cell1 % n][0] && connection[cell2 / n][cell2 % n][2];
    if (right(n, cell2) == cell1)
        return connection[cell1 / n][cell1 % n][3] && connection[cell2 / n][cell2 % n][1];
    if (left(n, cell2) == cell1)
        return connection[cell1 / n][cell1 % n][1] && connection[cell2 / n][cell2 % n][3];
    if (up(n, cell2) == cell1)
        return connection[cell1 / n][cell1 % n][2] && connection[cell2 / n][cell2 % n][0];
    return false;
}

int maxLayer(int n, int graph[n][n])
{
    int max = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (max < graph[i][j])
                max = graph[i][j];
        }
    }
    return max;
}

bool checkLayer(int n, int layer, int graph[n][n], bool connected[n][n][4])
{
    int connected_to_last_layer = 0;
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (graph[i][j] == layer)
            {
                count++;
                //checks if it is connected to only one cell from the previous layer
                if (((getValue(n, up(n, i * n + j), graph) == layer - 1) && isConnected(n, i * n + j, up(n, i * n + j), connected))
                    || ((getValue(n, right(n, i * n + j), graph) == layer - 1) && isConnected(n, i * n + j, right(n, i * n + j), connected))
                    || ((getValue(n, down(n, i * n + j), graph) == layer - 1) && isConnected(n, i * n + j, down(n, i * n + j), connected))
                    || ((getValue(n, left(n, i * n + j), graph) == layer - 1) && isConnected(n, i * n + j, left(n, i * n + j), connected)))
                    connected_to_last_layer++;
                //checks if it is connected to a layer before the last layer
                if (((getValue(n, up(n, i * n + j), graph) < layer - 1) && (getValue(n, up(n, i * n + j), graph) != -1) && isConnected(n, i * n + j, up(n, i * n + j), connected))
                    || ((getValue(n, right(n, i * n + j), graph) < layer - 1) && (getValue(n, right(n, i * n + j), graph) != -1) && isConnected(n, i * n + j, right(n, i * n + j), connected))
                    || ((getValue(n, down(n, i * n + j), graph) < layer - 1) && (getValue(n, down(n, i * n + j), graph) != -1) && isConnected(n, i * n + j, down(n, i * n + j), connected))
                    || ((getValue(n, left(n, i * n + j), graph) < layer - 1) && (getValue(n, left(n, i * n + j), graph) != -1) && isConnected(n, i * n + j, left(n, i * n + j), connected)))
                    return false;
                //checks if it is connected to the same layer
                if (((getValue(n, up(n, i * n + j), graph) == layer) && isConnected(n, i * n + j, up(n, i * n + j), connected))
                    || ((getValue(n, right(n, i * n + j), graph) == layer) && isConnected(n, i * n + j, right(n, i * n + j), connected))
                    || ((getValue(n, down(n, i * n + j), graph) == layer) && isConnected(n, i * n + j, down(n, i * n + j), connected))
                    || ((getValue(n, left(n, i * n + j), graph) == layer) && isConnected(n, i * n + j, left(n, i * n + j), connected)))
                    return false;
            }
        }
    }
    if (connected_to_last_layer == count)
        return true;
    return false;
}

bool checkAll(int n, int graph[n][n], bool connected[n][n][4])
{
    for (int i = 1; i <= maxLayer(n, graph); i++)
    {
        if (!checkLayer(n, i, graph, connected))
            return false;
    }
    return true;
}

bool rotateLayer (int n, int layer, int graph[n][n], int type[n][n], bool* connected, int* direction, int* pRotations)
{
    int count = 0;
    for (int i = 0; i < n * n; i++)
    {
        if (getValue(n, i, graph) == layer)
            count++;
    }

    //debug(count);

    int list[count];
    count = 0;
    for (int i = 0; i < n * n; i++)
    {
        if (getValue(n, i, graph) == layer)
        {
            list[count] = i;
            count++;
        }
    }

    for (int i = *pRotations + 1; i <= (1 << (count * 2)); i++)
    {
        *pRotations += 1;
        for (int j = 0; j < count; j++)
        {
            int cell = list[j];
            if (i % (1 << (j * 2)) == 0)
            {
                *(direction + cell) = (*(direction + cell) + 1) % 4;
                setConnection(n, cell, type, direction, connected + cell * 4);
            }
        }
        if (checkLayer(n, layer, graph, connected))
            return true;
    }
    return false;
}

void removeLayer(int n, int layer, int* graph)
{
    for (int i = 0; i < n * n; i++)
    {
        if (getValue(n, i, graph) >= layer)
            *(graph + i) = -1;
    }
    return;
}

void addLayer(int n, int layer, int* graph, bool* connected)
{
    for (int i = 0; i < n * n; i++)// <---------------- adds a layer
    {
        if ((getValue(n, i, graph) == layer) && *(connected + i * 4) && *(graph + up(n, i)) == -1)
            *(graph + up(n, i)) = layer + 1;
        if ((getValue(n, i, graph) == layer) && *(connected + i * 4 + 1) && *(graph + right(n, i)) == -1)
            *(graph + right(n, i)) = layer + 1;
        if ((getValue(n, i, graph) == layer) && *(connected + i * 4 + 2) && *(graph + down(n, i)) == -1)
            *(graph + down(n, i)) = layer + 1;
        if ((getValue(n, i, graph) == layer) && *(connected + i * 4 + 3) && *(graph + left(n, i)) == -1)
            *(graph + left(n, i)) = layer + 1;
    }
    return;
}



bool start(int n, int layer, int *graph, int *direction, int type[n][n], bool *connected, int *pgraph) //<--------------correct it later
{
    int countCells = 0;
    for (int i = 0; i < n * n; i++)
    {
        setConnection(n, i, type, direction, connected + i * sizeof(bool) * 4);
        if ((*(graph + i) != -1) && (*(graph + i) <= layer))
            countCells++;
    }

    if (countCells == n * n)
    {
        if (checkAll(n, graph, connected))
            return true;
        return false;
    }// the code under here is for when some cells are not layered yet

    int copyGraph[n][n];
    for (int i = 0; i < n * n; i++)
    {
        copyGraph[i / n][i % n] = *(graph + i);
    }

    if (compareGraphs(n, graph, pgraph))
        return false;
//------------------the code under here is for completing the graph
    int rotaions = 0;

    bool check = true;
    addLayer(n, layer - 1, graph, connected);

    /*debug(layer);
    debug(check);
    printf("graph:\n");
    printGraph(n, graph);
    printf("direction:\n");
    printGraph(n, direction);*/
    /*cnt++;
    debug(cnt);*/
    while (check)
    {
        if (start(n, layer + 1, graph, direction, type, connected, copyGraph))
        {
            return true;
        }
        /*cnt++;
        debug(cnt);*/
        check = rotateLayer(n, layer, graph, type, connected, direction, &rotaions);


    }

    removeLayer(n, layer, graph);
    return false;
}

int main()
{
    int n;
    scanf("%d", &n);
    int graph[n][n], emptyGraph[n][n]; //0 for the root/server. n is the child of n + 1
    int direction[n][n], initial_direction[n][n]; //similar to how the question said
    int type[n][n]; //0 for S (server), 1 for C, 2 for L, 3 for I, 4 for T
    bool connected[n][n][4]; //0 for up, 1, for right, 2 for down, 3 for left

    //start getting the input
    char getType;
    for (int i = 0; i < n * n; i++)
    {
        getchar();
        getType = getchar();
        switch (getType)
        {
        case 'S':
            type[i / n][i % n] = 0;
            break;
        case 'C':
            type[i / n][i % n] = 1;
            break;
        case 'L':
            type[i / n][i % n] = 2;
            break;
        case 'I':
            type[i / n][i % n] = 3;
            break;
        case 'T':
            type[i / n][i % n] = 4;
            break;
        }
        scanf("%d", &direction[i / n][i % n]);
    } //finish getting the input


    for (int i = 0; i < n * n; i++)
        setConnection(n, i, type, direction, &connected[i / n][i % n]);
    /*for (int i = 0; i < n * n; i++)
    {
        for (int j = 0; j < n * n; j++)
        {
            if (isConnected(n, i, j, connected)){}
                printf("(%d:%d)=((%d,%d),(%d,%d))=%d\n", i, j, type[i / 4][i % 4], direction[i / 4][i % 4],
                        type[j / 4][j % 4], direction[j / 4][j % 4], isConnected(n, i, j, connected));
        }
    }*/


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            graph[i][j] = -1, emptyGraph[i][j] = -1;
            initial_direction[i][j] = direction[i][j];
            direction[i][j] = 0;
        }
    }

    graph[n / 2][n / 2] = 0;


    /*debug(compareGraphs(n, compareGraphs, emptyGraph));

    debug(start(n, 1, graph, direction, type, connected, emptyGraph));
    for (int i = 1; i <= maxLayer(n, graph); i++)
        debug(checkLayer(n, i, graph, connected));*/

    /*addLayer(n, 0, graph, connected);
    int rotations = 0;
    while (rotateLayer(n, 1, graph, type, connected, direction, &rotations))
        printGraph(n, direction), printf("\n");*/
    for (int i = 0; i < 4; i++)
    {
        if (start(n, 1, graph, direction, type, connected, emptyGraph))
            break;
        direction[n / 2][n / 2]++;
    }


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d ", (direction[i][j] - initial_direction[i][j] + 4) % 4);
        printf("\n");
    }

    printf("connected:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%d%d%d%d ", connected[i][j][0], connected[i][j][1], connected[i][j][2], connected[i][j][3]);
        printf("\n");
    }

    return 17;
}
