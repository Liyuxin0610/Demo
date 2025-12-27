#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>
#include <stack>
using namespace std;

// 最大顶点数
const int MVNum = 100;
// 定义MaxInt是无穷大 
const int MaxInt = INT_MAX;

// ========================== 邻接矩阵结构 ==========================
struct AMGraph{
    // 顶点表
    int vexs[MVNum];
    // 邻接矩阵（权值）
    int arcs[MVNum][MVNum];
    // 当前顶点数和边数
    int vexnum, arcnum;
}; 

// ========================== 邻接表结构 ==========================
// 边结点结构
struct ArcNode {
    // 邻接点在顶点表中的下标
    int adjvex;
    // 边的权值
    int weight;
    // 指向下一个边结点的指针 
    struct ArcNode* nextarc;
};
// 顶点结构
struct VNode {
    int data;
    // 指向第一个边结点的指针 
    ArcNode* firstarc;
};
// 邻接表结构
struct ALGraph {
    // 顶点数组
    VNode vertices[MVNum];
    // 当前顶点数和边数
    int vexnum, arcnum;
};

// 查找顶点在邻接矩阵中的下标
int LocateVex_AM(AMGraph G, int v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) return i;
    }
    return -1; // 未找到
}

// 查找顶点在邻接表中的下标
int LocateVex_AL(ALGraph G, int v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data == v) return i;
    }
    return -1; // 未找到
}

// ========================== 邻接矩阵操作 ==========================
// 1. 邻接矩阵创建带权无向图 
void CreateUDN_AM(AMGraph& G) {
    cout << "=== 邻接矩阵创建带权无向图 ===" << endl;
    cout << "请输入顶点数和边数（用空格分隔）：";
    cin >> G.vexnum >> G.arcnum;

    // 往顶点表输入顶点信息
    cout << "请依次输入" << G.vexnum << "个顶点：";
    for (int i = 0; i < G.vexnum; i++) {
        cin >> G.vexs[i];
    }
    // 初始化邻接矩阵
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            G.arcs[i][j] = MaxInt;
        }
        // 顶点自身到自身权值为0
        G.arcs[i][i] = 0;
    }
    // 输入边信息（v1 v2 权值）
    cout << "请依次输入" << G.arcnum << "条边（格式：顶点1 顶点2 权值）：" << endl;
    for (int k = 0; k < G.arcnum; k++) {
        int v1, v2;
        int w;
        cin >> v1 >> v2 >> w;
        int i = LocateVex_AM(G, v1);
        int j = LocateVex_AM(G, v2);
        // 无向图：对称赋值
        G.arcs[i][j] = w;
        G.arcs[j][i] = w;
    }
    cout << "邻接矩阵创建完成！" << endl << endl;
}

// 2. 邻接矩阵 DFS 遍历（递归实现）
bool visited_AM[MVNum];
void DFS_AM(AMGraph G, int v) {	//从顶点表中下标为v的节点开始遍历 
    // 访问当前顶点
    cout << G.vexs[v] << " ";
    // 标记为已访问
    visited_AM[v] = true;
    // 遍历所有邻接点
    for (int w = 0; w < G.vexnum; w++) {
        // 存在边且未访问
        if (G.arcs[v][w] != MaxInt && G.arcs[v][w] != 0 && !visited_AM[w]) {
            DFS_AM(G, w);
        }
    }
}

// 3. 邻接矩阵 BFS 遍历（队列实现）
void BFS_AM(AMGraph G, int v) {
    bool visited[MVNum] = {false};	
    queue<int> Q;

    // 访问起始顶点
    cout << G.vexs[v] << " ";
    visited[v] = true;
    // 入队
    Q.push(v);
    while (!Q.empty()) {
        // 队首顶点出队
        int u = Q.front();
        Q.pop();
        // 遍历所有邻接点
        for (int w = 0; w < G.vexnum; w++) {
            if (G.arcs[u][w] != MaxInt && G.arcs[u][w] != 0 && !visited[w]) {
                cout << G.vexs[w] << " ";
                visited[w] = true;
                Q.push(w);
            }
        }
    }
}

// 4. 邻接矩阵 Dijkstra 最短路径算法（从起始顶点到所有顶点）
void Dijkstra_AM(AMGraph G, int v0) {
    int n = G.vexnum;
    int D[MVNum];			//D[i]:起始顶点v0到各顶点vi的最短路径长度
    bool S[MVNum];			//S[i]:标记顶点vi是否已确定最短路径
    int Path[MVNum];		//前驱顶点数组：Path[i]表示i的最短路径上的前驱顶点下标

    // 初始化
    for (int v = 0; v < n; v++) {
        D[v] = G.arcs[v0][v];
        S[v] = false;
        // 初始前驱：若有直接边则为v0，否则为-1
        if (D[v] != MaxInt && D[v] != 0) {
            Path[v] = v0;              // 有直接边则前驱为v0
        } else {
            Path[v] = -1;
		}              // 无边则前驱为-1
    }
    D[v0] = 0;
    S[v0] = true;
    Path[v0] = -1; // 起始顶点无前驱
    // 迭代n-1次（确定其余n-1个顶点的最短路径）
    for (int i = 1; i < n; i++) {
        int min = MaxInt;
        int u = -1;
        // 找到当前未确定顶点中距离最小的
        for (int w = 0; w < n; w++) {
            if (!S[w] && D[w] < min) {
                min = D[w];
                u = w;
            }
        }
        if (u == -1) break; // 剩余顶点不可达，提前退出
        S[u] = true;
        // 更新所有未确定顶点的距离和前驱
        for (int w = 0; w < n; w++) {
            // 避免溢出：先判断D[u]和G.arcs[u][w]是否为MaxInt
            if (!S[w] && D[u] != MaxInt && G.arcs[u][w] != MaxInt) {
                if (D[u] + G.arcs[u][w] < D[w]) {
                    D[w] = D[u] + G.arcs[u][w];
                    Path[w] = u; // 更新前驱
                }
            }
        }
    }

    // 输出结果
    cout << "=== Dijkstra 最短路径结果（起始顶点：" << G.vexs[v0] << "）===" << endl;
    for (int i = 0; i < n; i++) {
        cout << G.vexs[v0] << " -> " << G.vexs[i] << "：";
        if (D[i] == MaxInt) {
            cout << "无路径" << endl;
        } else {
            cout << "权值=" << D[i] << "，路径=";
            // 用栈还原路径（前驱是逆序，栈用于反转）
            stack<int> pathStack;
            int cur = i;
            while (cur != -1) {
                pathStack.push(cur);
                cur = Path[cur];
            }
            // 输出路径
            while (!pathStack.empty()) {
                cout << G.vexs[pathStack.top()];
                pathStack.pop();
                if (!pathStack.empty()) cout << "->";
            }
            cout << endl;
        }
    }
}

// ========================== 邻接表操作 ==========================
// 1. 邻接表创建带权无向网
void CreateUDN_AL(ALGraph& G) {
    cout << "=== 邻接表创建带权无向图 ===" << endl;
    cout << "请输入顶点数和边数：";
    cin >> G.vexnum >> G.arcnum;

    // 输入顶点信息
    cout << "请依次输入" << G.vexnum << "个顶点：";
    for (int i = 0; i < G.vexnum; i++) {
        cin >> G.vertices[i].data;
        // 初始化边表为空
        G.vertices[i].firstarc = NULL;
    }

    // 输入边信息
    cout << "请依次输入" << G.arcnum << "条边（格式：顶点1 顶点2 权值）：" << endl;
    for (int k = 0; k < G.arcnum; k++) {
        int v1, v2;
        int w;
        cin >> v1 >> v2 >> w;
        int i = LocateVex_AL(G, v1);
        int j = LocateVex_AL(G, v2);

        // 创建边结点i->j
        ArcNode* p1 = new ArcNode;
        p1->adjvex = j;
        p1->weight = w;
        // 头插法插入边表
        p1->nextarc = G.vertices[i].firstarc;
        G.vertices[i].firstarc = p1;

        // 创建边结点j->i（无向图对称）
        ArcNode* p2 = new ArcNode;
        p2->adjvex = i;
        p2->weight = w;
        p2->nextarc = G.vertices[j].firstarc;
        G.vertices[j].firstarc = p2;
    }
    cout << "邻接表创建完成！" << endl << endl;
}

// 2. 邻接表 DFS 遍历（递归实现）
bool visited_AL[MVNum];
void DFS_AL(ALGraph G, int v) {
    // 访问当前顶点
    cout << G.vertices[v].data << " ";
    visited_AL[v] = true;

    // 遍历边表
    ArcNode* p = G.vertices[v].firstarc;
    while (p != NULL) {
        int w = p->adjvex;
        // 未访问则递归
        if (!visited_AL[w]) {
            DFS_AL(G, w);
        }
        p = p->nextarc;
    }
}

// 3. 邻接表 BFS 遍历（队列实现）
void BFS_AL(ALGraph G, int v) {
    bool visited[MVNum] = {false};
    queue<int> Q;

    cout << G.vertices[v].data << " ";
    visited[v] = true;
    Q.push(v);
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        // 遍历边表
        ArcNode* p = G.vertices[u].firstarc;
        while (p != NULL) {
            int w = p->adjvex;
            if (!visited[w]) {
                cout << G.vertices[w].data << " ";
                visited[w] = true;
                Q.push(w);
            }
            p = p->nextarc;
        }
    }
}

// ========================== 主函数（测试用例） ==========================
int main() {
    AMGraph amG;
    ALGraph alG;
    int startVex;

    // -------------------------- 邻接矩阵测试 --------------------------
    CreateUDN_AM(amG);
    // 邻接矩阵 DFS
    cout << "邻接矩阵 DFS 遍历（从第一个顶点开始）：";
    memset(visited_AM, false, sizeof(visited_AM));
    DFS_AM(amG, 0);
    cout << endl;
    // 邻接矩阵 BFS
    cout << "邻接矩阵 BFS 遍历（从第一个顶点开始）：";
    BFS_AM(amG, 0);
    cout << endl << endl;
    // Dijkstra 算法
    cout << "请输入 Dijkstra 算法的起始顶点：";
    cin >> startVex;
    int startIdx = LocateVex_AM(amG, startVex);
    if (startIdx != -1) {
         Dijkstra_AM(amG, startIdx);;
    } else {
        cout << "顶点不存在！" << endl;
    }
    cout << endl;
    
    // -------------------------- 邻接表测试 --------------------------
    CreateUDN_AL(alG);
    // 邻接表 DFS
    cout << "邻接表 DFS 遍历（从第一个顶点开始）：";
    memset(visited_AL, false, sizeof(visited_AL));
    DFS_AL(alG, 0);
    cout << endl;
    // 邻接表 BFS
    cout << "邻接表 BFS 遍历（从第一个顶点开始）：";
    BFS_AL(alG, 0);
    cout << endl << endl;

    return 0;
}
