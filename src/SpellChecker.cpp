#include "../include/SpellChecker.h"
#include <vector>
#include <algorithm>

// Lưu ý: Không còn chữ "class SpellChecker { ... }" bọc bên ngoài nữa
// Không còn chữ "static" ở đây nữa (chữ static chỉ nằm trong file .h)
int SpellChecker::calculateLevenshtein(const std::string& s1, const std::string& s2) {
    int m = s1.length();
    int n = s2.length();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0) dp[i][j] = j; 
            else if (j == 0) dp[i][j] = i; 
            else if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1]; 
            else {
                dp[i][j] = 1 + std::min({
                    dp[i][j - 1],    
                    dp[i - 1][j],    
                    dp[i - 1][j - 1] 
                });
            }
        }
    }
    return dp[m][n];
}