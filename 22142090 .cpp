#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

class MineSweeper {
private:
    int n; // フィールドのサイズ
    int m; // 地雷の数
    vector<vector<int>> land; // フィールドの状態
    vector<vector<int>> isOpened; // セルが開かれたかどうか

    void initializeRandomSeed() {
        static bool seedInitialized = false;
        if (!seedInitialized) {
            srand(static_cast<unsigned int>(time(nullptr)));
            seedInitialized = true;
        }
    }

public:
    MineSweeper(int size) : n(size), m(size* size / 2), land(size + 1, vector<int>(size + 1, 0)), isOpened(size + 1, vector<int>(size + 1, 0)) {
        initializeRandomSeed();
    }

    void setMines(int mines) {
        m = mines;
    }

    void placeMines() {
        for (int i = 0; i < m; i++) {
            int x, y;
            do {
                x = rand() % n + 1;
                y = rand() % n + 1;
            } while (land[y][x] == 9);

            land[y][x] = 9;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (isInBounds(y + i, x + j) && land[y + i][x + j] != 9) {
                        land[y + i][x + j] += 1;
                    }
                }
            }
        }
    }

    int open(int x, int y) {
        if (!isInBounds(y, x) || isOpened[y][x]) return -1;
        isOpened[y][x] = 1;
        if (land[y][x] == 9) {
            return 9;
        }
        if (land[y][x] == 0) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (isInBounds(y + i, x + j) && !isOpened[y + i][x + j]) {
                        open(x + j, y + i);
                    }
                }
            }
        }
        return land[y][x];
    }

    bool check() {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if ((land[i][j] != 9 && !isOpened[i][j]) || (land[i][j] == 9 && isOpened[i][j])) {
                    return false;
                }
            }
        }
        return true;
    }

    void show() {
        cout << "\n ";
        for (int i = 1; i <= n; i++) {
            cout << " " << i;
        }
        cout << "\n";
        for (int i = 1; i <= n; i++) {
            cout << i << ":";
            for (int j = 1; j <= n; j++) {
                if (isOpened[i][j]) {
                    if (land[i][j] == 9) {
                        cout << "* ";
                    }
                    else {
                        cout << land[i][j] << " ";
                    }
                }
                else {
                    cout << "# ";
                }
            }
            cout << "\n";
        }
    }

    class Iterator {
    private:
        vector<vector<int>>& land;
        int current_row;
        int current_col;

    public:
        Iterator(vector<vector<int>>& l, int row, int col) : land(l), current_row(row), current_col(col) {}

        pair<int, int> operator*() const {
            return make_pair(current_col, current_row);
        }

        Iterator& operator++() {
            if (++current_col > land[0].size()) {
                current_col = 1;
                ++current_row;
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current_row != other.current_row || current_col != other.current_col;
        }
    };

    Iterator begin() {
        return Iterator(land, 1, 1);
    }

    Iterator end() {
        return Iterator(land, n + 1, 1);
    }

    bool isInBounds(int y, int x) {
        return y >= 1 && y <= n && x >= 1 && x <= n;
    }
};

int main() {
    int n;
    cout << "フィールドのサイズを3～8で指定してください。入力例「5」\n";
    cin >> n;

    if (n < 3 || n > 8) {
        cout << "無効なサイズです。3～8の範囲で指定してください。\n";
        return 1;
    }

    MineSweeper game(n);

    int mines;
    cout << "地雷の数を1～" << n * n / 2 << "で指定してください。入力例「3」\n";
    cin >> mines;

    if (mines < 1 || mines > n * n / 2) {
        cout << "無効な地雷の数です。1～" << n * n / 2 << "の範囲で指定してください。\n";
        return 1;
    }

    game.setMines(mines);
    game.placeMines();

    while (true) {
        game.show();
        cout << "座標を「x y」で指定してください。入力例「1 2」\n";
        int x, y;
        cin >> x >> y;

        if (!game.isInBounds(y, x)) {
            cout << "無効な座標です。もう一度入力してください。\n";
            continue;
        }

        int result = game.open(x, y);
        if (result == 9) {
            game.show();
            cout << "Game Over\n";
            break;
        }
        if (game.check()) {
            game.show();
            cout << "Game Clear!\n";
            break;
        }
    }

    return 0;
}
