#include <bits/stdc++.h>
using namespace std;

// Implement deque command interpreter compatible with common OJ formats.
// Supports:
// - Optional first integer Q: number of following commands
// - Otherwise reads commands until EOF
// Commands: push_front X, push_back X, pop_front, pop_back, size, empty, front, back
// Semantics: same as typical BOJ 10866 style

static inline bool readLine(string &s) {
    s.clear();
    int c = cin.peek();
    if (c == EOF) return false;
    getline(cin, s);
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    deque<long long> dq;
    string line;

    // Detect if first token is integer Q
    // Approach: Attempt to read a line first. If it looks like a single integer, treat it as Q.
    // Otherwise, process that line as the first command and continue with the rest.
    if (!readLine(line)) return 0;

    auto is_integer_line = [](const string &s, long long &val)->bool{
        string t = s;
        // trim
        auto l = t.find_first_not_of(" \t\r\n");
        auto r = t.find_last_not_of(" \t\r\n");
        if (l == string::npos) return false;
        t = t.substr(l, r - l + 1);
        // must be integer without spaces
        for (size_t i = 0; i < t.size(); ++i) {
            if (i==0 && (t[i]=='+' || t[i]=='-')) continue;
            if (!isdigit(static_cast<unsigned char>(t[i]))) return false;
        }
        try {
            long long x = stoll(t);
            val = x;
            return true;
        } catch (...) { return false; }
    };

    auto process = [&](const string &cmdline){
        if (cmdline.empty()) return; // ignore
        string cmd; long long x;
        stringstream ss(cmdline);
        ss >> cmd;
        if (cmd == "push_front") {
            if (ss >> x) dq.push_front(x);
        } else if (cmd == "push_back") {
            if (ss >> x) dq.push_back(x);
        } else if (cmd == "pop_front") {
            if (dq.empty()) cout << -1 << '\n';
            else { cout << dq.front() << '\n'; dq.pop_front(); }
        } else if (cmd == "pop_back") {
            if (dq.empty()) cout << -1 << '\n';
            else { cout << dq.back() << '\n'; dq.pop_back(); }
        } else if (cmd == "size") {
            cout << dq.size() << '\n';
        } else if (cmd == "empty") {
            cout << (dq.empty() ? 1 : 0) << '\n';
        } else if (cmd == "front") {
            if (dq.empty()) cout << -1 << '\n'; else cout << dq.front() << '\n';
        } else if (cmd == "back") {
            if (dq.empty()) cout << -1 << '\n'; else cout << dq.back() << '\n';
        } else {
            // unknown command -> ignore
        }
    };

    long long Q = 0;
    if (is_integer_line(line, Q)) {
        for (long long i = 0; i < Q; ++i) {
            if (!readLine(line)) break;
            process(line);
        }
    } else {
        process(line);
        while (readLine(line)) process(line);
    }

    return 0;
}
