/***
 *   1.实现了KMP算法
 *   @author: air
 ***/

#include<string>
#include<vector>
#include <iostream>

using namespace std;

// 构造状态转移图 
//vector<vector<int>> KMP(string &pat) {
//	int n = pat.length();
//	int nc = 256;
//	vector<vector<int>> dp(n, vector<int>(256,0));
//	dp[0][pat[0]] = 1;
//	int shade = 0;
//	for (int i = 1; i < n; i++) {
//		for (int c = 0; c < 256; c++) {
//			dp[i][c] = dp[shade][c];
//		}
//		dp[i][pat[i]] = i + 1;
//		shade = dp[shade][pat[i]];
//	}
//	return dp;
//}
//bool search(string& s1, string& s2) {
//	auto dp = KMP(s2);
//	int j = 0;
//	int n1 = s2.length();
//	for (int i = 0; i < s1.length(); i++) {
//		j = dp[j][s1[i]];
//		if (j == n1) return true;
//	}
//
//	return false;
//}

// dp思想构造next数组
// next[i] 可以看作是p[0]-p[i-1]串的最长的相同前缀和后缀串长度
// 也可以理解为当匹配p[i]和s[m]时，没有匹配成功时，下一次要比较p[next[i]] 和 s[m]
//vector<int> KMP(string& pat) {
//	int n = pat.length();
//	vector<int> next(n, 0);
//	next[0] = -1;
//	next[1] = 0;
//	for (int i = 2; i < n; i++) {
//		if (pat[next[i - 1]] == pat[i-1])
//			next[i] = next[i - 1] + 1;
//		else {
//			int k = next[i - 1];
//			while (k != -1 && pat[k] != pat[i - 1])
//				k = next[k];
//			next[i] = k+1;
//		}
//	}
//
//	return next;
//}
//bool search(string& s1, string& s2) {
//	auto next = KMP(s2);
//	int j = 0;
//	int n1 = s2.length();
//	for (int i = 0; i < s1.length();) {
//		if (s1[i] == s2[j]) {
//			i++;
//			j++;
//		}
//		else {
//			j = next[j];
//		}
//		if (j == n1) return true;
//	}
//
//	return false;
//}
// 精简
// next[i] 可以看作是p[0]-p[i-1]串的最长的相同前缀和后缀串长度
// 也可以理解为当匹配p[i]和s[m]时，没有匹配成功时，下一次要比较p[next[i]] 和 s[m]
vector<int> KMP(string& pat) {
	int n = pat.length();
	vector<int> next(n, 0);
	next[0] = -1;
	if(n>1)
	next[1] = 0;
	for (int i = 2,j=0; i < n; i++) {
		if (j > 0 && pat[i - 1] != pat[j]) {
			j = next[j];
		}
		if (pat[i - 1] == pat[j])
			j++;
		next[i] = j;
	}

	return next;
}
int search(string& s1, string& s2) {
	auto next = KMP(s2);
	int j = 0;
	int n1 = s2.length();
	for (int i = 0; i < s1.length();i++) {
		while (j > 0 && s1[i] != s2[j])
			j = next[j];
		if (s1[i] == s2[j])
			j++;
	
		if (j == n1) return i-n1+1;
	}

	return -1;
}

// 官方
    /*int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m == 0) {
            return 0;
        }
        vector<int> pi(m);
        for (int i = 1, j = 0; i < m; i++) {
            while (j > 0 && needle[i] != needle[j]) {
                j = pi[j - 1];
            }
            if (needle[i] == needle[j]) {
                j++;
            }
            pi[i] = j;
        }
        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && haystack[i] != needle[j]) {
                j = pi[j - 1];
            }
            if (haystack[i] == needle[j]) {
                j++;
            }
            if (j == m) {
                return i - m + 1;
            }
        }
        return -1;
    }*/
int main() {
	string s1 = "h";
	string s2 = "h";
	cout << search(s1,s2);
}