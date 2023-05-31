#include <iostream>
#include <set>
#include <vector>
std::vector<int>& findDiffStones(const std::vector<int>& inNums);
std::pair<int, int>& findOnePairOfStones(const std::vector<int>& uniNums);
std::vector<std::pair<int, int>>& findAllPairsOfStones(
    const std::vector<int>& uniNums);
std::pair<int, int>& findOnePairOfStones_B(std::vector<int>& inNums);
std::vector<std::pair<int, int>>& findAllPairOfStones_C(
    std::vector<int>& inNums);
std::vector<std::pair<int, int>>& findPathsToHome(
    std::vector<std::vector<int>>& allCells,
    std::vector<std::vector<bool>>& cell_no);
bool canEnter(int rows, int cols, int curr_row, int curr_col,
              std::vector<std::vector<int>>& visited,
              std::vector<std::vector<int>>& cell_no);
/*
 * std::pair<int,int> out_B = F(std::vector<int>& in_B)
 * std::vector<std::pair<int,int>> out_C = F(std::vector<int>& in_C)
 * */
int main() {
  /*Problem 2: Find stone pair*/
  char in_ch = 0;
  std::cin >> in_ch;
  std::vector<int> arr_in = {2, 0, 1, 0};
  std::vector<int> uni_vec = findDiffStones(arr_in);
  std::pair<int, int> res_B_On /*time complexity O(N) for problem B*/,
      res_B_O1 /*time complexity O(1) for problem B*/;
  std::vector<std::pair<int, int>>
      res_C_On /*time complexity O(N) for problem C*/,
      res_C_O1 /*time complexity O(1) for problem C*/;
  /*problemA*/
  if (in_ch == 'A') {
    res_B_On = findOnePairOfStones(uni_vec);
    res_B_O1 = findOnePairOfStones_B(arr_in);
  }
  /*problemB*/
  if (in_ch == 'B') {
    res_C_On = findAllPairsOfStones(uni_vec);
    res_C_O1 = findAllPairOfStones_C(arr_in);
  } /*Problem 1: Rabbit goes home*/
  int rows = 4, cols = 5;
  std::vector<std::vector<int>> cell_no(rows);
  for (auto c_vec : cell_no) {
    c_vec.resize(cols);
  }
  cell_no[1][3] = 1;
  cell_no[3][4] = 1;
//   findNumPathToHome() return 0;
}
std::vector<int>& findDiffStones(std::vector<int>& inNums) {
  std::set<int> tmp;
  std::vector<int> uniVec;
  for (int i = 0; i < inNums.size(); i++) {
    if (tmp.count(inNums[i]) == 0) tmp.insert(inNums[i]);
  }
  std::set<int>::iterator it = tmp.begin();
  for (; it != tmp.end(); it++) {
    uniVec.push_back(*it);
  }
  return uniVec;
}
std::pair<int, int>& findOnePairOfStones(const std::vector<int>& uniNums) {
  std::pair<int, int> res = std::make_pair(uniNums[0], uniNums[1]);
  return res;
}
std::pair<int, int>& findOnePairOfStones_B(std::vector<int>& inNums) {
  std::pair<int, int> res;
  for (int i = 0; i < inNums.size(); i++) {
    while (inNums[i] != i) {
      if (inNums[inNums[i]] != inNums[i]) {
        int tmp = inNums[inNums[i]];
        inNums[inNums[i]] = inNums[i];
        inNums[i] = tmp;
        res = std::make_pair(inNums[inNums[i]], inNums[i]);
        return res;
      }
    }
  }
}
std::vector<std::pair<int, int>>& findAllPairOfStones_C(
    std::vector<int>& inNums) {
  std::vector<std::pair<int, int>> res;
  std::vector<int> uniVec;
  for (int i = 0; i < inNums.size(); i++) {
    while (inNums[i] != i) {
      if (inNums[inNums[i]] != inNums[i]) {
        int tmp = inNums[inNums[i]];
        inNums[inNums[i]] = inNums[i];
        inNums[i] = tmp;
      }
    }
  }
  for (int i = 0; i < inNums.size(); i++) {
    if (inNums[i] == i) {
      uniVec.push_back(inNums[i]);
    }
  }
  for (int i = 0; i < uniVec.size(); i++) {
    for (int j = 0; j < uniVec.size(); j++) {
      res.push_back(std::make_pair(uniVec[i], uniVec[j]));
    }
  }
  return res;
}
std::vector<std::pair<int, int>>& findAllPairsOfStones(
    const std::vector<int>& uniNums) {
  std::vector<std::pair<int, int>> res;
  for (int i = 0; i < uniNums.size(); i++) {
    for (int j = i + 1; j < uniNums.size(); j++) {
      res.push_back(std::make_pair(uniNums[i], uniNums[j]));
    }
  }
  return res;
}
bool canEnter(int rows, int cols, int curr_row, int curr_col,
              std::vector<std::vector<int>>& visited,
              std::vector<std::vector<int>>& cell_no) {
  if (curr_row > 0 && curr_row < rows && curr_col > 0 && curr_col < cols &&
      !visited[curr_row][curr_col] && !cell_no[curr_row][curr_col]) {
    return true;
  }
  return false;
}
std::vector<std::pair<int, int>> paths;
std::pair<int, int>& getAPath(int sumRow, int sumCol,
                              std::vector<std::vector<int>>& cell_no,
                              std::vector<std::vector<int>>& visited,
                              int curr_row, int curr_col) {
  std::pair<int, int> res;
  if (canEnter(sumRow, sumCol, curr_row, curr_col, visited, cell_no)) {
    visited[curr_row][curr_col] = 1;
    paths.push_back(
        getAPath(sumRow, sumCol, cell_no, visited, curr_row - 1, curr_col));
    paths.push_back(
        getAPath(sumRow, sumCol, cell_no, visited, curr_row, curr_col - 1));
    paths.push_back(
        getAPath(sumRow, sumCol, cell_no, visited, curr_row + 1, curr_col));
    paths.push_back(
        getAPath(sumRow, sumCol, cell_no, visited, curr_row, curr_col + 1));
  }
}
std::vector<std::pair<int, int>>& findPathsToHome(
    std::vector<std::vector<int>>& allCells,
    std::vector<std::vector<int>>& cell_no) {
  std::vector<std::pair<int, int>> res;
  res.clear();
  if (allCells.size() < 1) return res;
  std::vector<std::vector<int>> visited(allCells);
  for (int r = 0; r < allCells.size(); r++) {
    for (int c = 0; c < allCells[r].size(); c++) {
      visited[r][c] = 0;
    }
  }
  //
  getAPath(allCells.size(), allCells[0].size(), )
}