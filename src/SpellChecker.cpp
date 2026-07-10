#include <string>
#include <vector>
#include <algorithm>

class SpellChecker {
public:
    // O(M * N) Time Complexity
    static int calculateLevenshtein(const std::string& s1, const std::string& s2) {
        int m = s1.length();
        int n = s2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

        for (int i = 0; i <= m; i++) {
            for (int j = 0; j <= n; j++) {
                if (i == 0) dp[i][j] = j; // Phải thêm j ký tự
                else if (j == 0) dp[i][j] = i; // Phải xóa i ký tự
                else if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1]; // Ký tự giống nhau
                else {
                    dp[i][j] = 1 + std::min({
                        dp[i][j - 1],    // Insert
                        dp[i - 1][j],    // Remove
                        dp[i - 1][j - 1] // Replace
                    });
                }
            }
        }
        return dp[m][n];
    }
};