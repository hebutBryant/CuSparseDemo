#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>

const int N = 10;

class Graph
{
private:
std::vector<int> csr_row_offset;
std::vector<int> csr_col_indices;
std::vector<int> csc_col_offset;
std::vector<int> csc_row_indoces;
std::vector<int> values;
int nzz = 0;
int v_num;

    
public:
    Graph(int adjacencyMatrix[][10], int n){
    v_num = n;
    csr_row_offset.resize(n + 1, 0);
    csc_col_offset.resize(n + 1, 0);
    csr_row_offset[0] = 0;
    csc_col_offset[0] = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adjacencyMatrix[i][j] != 0) { 
                csr_col_indices.push_back(j);
                values.push_back(1);
            }else{
                nzz++;
            }
        }
        csr_row_offset[i + 1] = csr_col_indices.size();
    }

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            if (adjacencyMatrix[i][j] != 0) { // If there's an edge from i to j
                csc_row_indoces.push_back(i);
            }
        }
        csc_col_offset[j + 1] = csc_row_indoces.size();
    }


    }
    // Getters
     std::vector<int>& get_csr_row_offset()  { return csr_row_offset; }
     std::vector<int>& get_csr_col_indices()  { return csr_col_indices; }
     std::vector<int>& get_csc_col_offset()  { return csc_col_offset; }
     std::vector<int>& get_csc_row_indoces()  { return csc_row_indoces; }
     std::vector<int>& get_values()  { return values;}
    int get_nzz() const { return nzz; }
    int get_v_num() const { return v_num; }

    ~Graph(){}
};

#endif


