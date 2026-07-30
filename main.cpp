#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Node {
    unordered_map<string, Node*> exactChildren; 
    Node* wildcardChild = nullptr;             
    string varName = "";                       
    string handler = "";                        
};

struct Match {
    bool found;
    string handler;
    unordered_map<string, string> params;
};

class TreeRouter {
private:
    Node* root;

    vector<string> split(const string& url) {
        vector<string> tokens;
        stringstream ss(url);
        string token;
        while (getline(ss, token, '/')) {
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }

public:
    TreeRouter() { root = new Node(); }

    void addRoute(const string& path, const string& handler) {
        Node* curr = root;
        for (const string& token : split(path)) {
            if (token.front() == '{' && token.back() == '}') {
                if (!curr->wildcardChild) {
                    curr->wildcardChild = new Node();
                    curr->wildcardChild->varName = token.substr(1, token.length() - 2);
                }
                curr = curr->wildcardChild;
            } 

            else {
                if (!curr->exactChildren.count(token)) {
                    curr->exactChildren[token] = new Node();
                }
                curr = curr->exactChildren[token];
            }
        }
        curr->handler = handler; 
    }

    Match resolve(const string& url) {
        Node* curr = root;
        unordered_map<string, string> params;

        for (const string& token : split(url)) {
            if (curr->exactChildren.count(token)) {
                curr = curr->exactChildren[token];
            }
            else if (curr->wildcardChild) {
                curr = curr->wildcardChild;
                params[curr->varName] = token; 
            }
            else {
                return {false, "", {}};
            }
        }

        if (!curr->handler.empty()) {
            return {true, curr->handler, params};
        }
        
        return {false, "", {}};
    }
};

int main() {
    TreeRouter router;

    router.addRoute("/api/users", "GetAllUsers");
    router.addRoute("/api/users/{id}", "GetUserById");
    router.addRoute("/api/users/{id}/orders/{order_id}", "GetOrder");

    vector<string> tests = {
        "/api/users",
        "/api/users/999",
        "/api/users/999/orders/55",
        "/api/invalid"
    };

    for (const string& url : tests) {
        Match result = router.resolve(url);
        
        if (result.found) {
            cout << "[MATCH] " << url << " -> " << result.handler << "\n";
            for (auto const& [key, val] : result.params) {
                cout << "    Var: " << key << " = " << val << "\n";
            }
        } else {
            cout << "[404]   " << url << " -> Not Found\n";
        }
    }

    return 0;
}
