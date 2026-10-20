#include <iostream>
#include <vector>
#include <string>
using namespace std;



struct State {
    int person;
    int wolf;
    int goat;
    int cabbage;
};



struct Transition {
    State state;
    string description;
};



struct Move {
    State state;
    string description;
};



bool isSafe(const State& s){
    // Check if the goat is left alone with the wolf or cabbage
    if ((s.goat == s.wolf && s.person != s.goat) || (s.goat == s.cabbage && s.person != s.goat)) {
        return false;
    }
    return true;
}



vector<Transition> getNextTransitions(const State& current) {
    vector<Transition> transitions;

    string direction = (current.person == 0)
        ? "across"
        : "back";

    // Try crossing alone.
    State next = current;
    next.person = 1 - current.person;

    if (isSafe(next)) {
        transitions.push_back({next, "Person crosses " + direction + " alone"});
    }

    // Try taking the wolf.
    if (current.wolf == current.person) {
        next = current;
        next.person = 1 - current.person;
        next.wolf = 1 - current.wolf;

        if (isSafe(next)) {
            transitions.push_back({next, "Person takes the wolf " + direction});
        }
    }

    // Try taking the goat.
    if (current.goat == current.person) {
        next = current;
        next.person = 1 - current.person;
        next.goat = 1 - current.goat;

        if (isSafe(next)) {
            transitions.push_back({next, "Person takes the goat " + direction});
        }
    }

    // Try taking the cabbages.
    if (current.cabbage == current.person) {
        next = current;
        next.person = 1 - current.person;
        next.cabbage = 1 - current.cabbage;

        if (isSafe(next)) {
            transitions.push_back({next, "Person takes the cabbages " + direction});
        }
    }

    return transitions;
}



struct Node {
    State state;
    string move;
    Node* firstChild;
    Node* nextSibling;

    Node(State s, string m = "") {
        state = s;
        move = m;
        firstChild = nullptr;
        nextSibling = nullptr;
    }
};



void addChild(Node* parent, Node* child) {
    if (parent->firstChild == nullptr) {
        parent->firstChild = child;
    } else {
        Node* current = parent->firstChild;

        while (current->nextSibling != nullptr) {
            current = current->nextSibling;
        }

        current->nextSibling = child;
    }
}



bool sameState(const State& a, const State& b) {
    return a.person == b.person &&
           a.wolf == b.wolf &&
           a.goat == b.goat &&
           a.cabbage == b.cabbage;
}



bool isOnPath(const vector<State>& path, const State& state) {
    for (const State& oldState : path) {
        if (sameState(oldState, state)) {
            return true;
        }
    }
    return false;
}



void buildTree(Node* parent, vector<State>& path) {
    State goal{1, 1, 1, 1};

    if (sameState(parent->state, goal)) {
        return;  // This path has reached the goal.
    }

    vector<Transition> transitions = getNextTransitions(parent->state);

    for (const Transition& transition : transitions) {
        if (isOnPath(path, transition.state)) {
            continue;  
        }

        Node* child = new Node(transition.state, transition.description);
        addChild(parent, child);

        path.push_back(transition.state);
        buildTree(child, path);
        path.pop_back();
    }
}



void printState(const State& s) {
    cout << "(" << s.person << ", "
         << s.wolf << ", "
         << s.goat << ", "
         << s.cabbage << ")";
}



void printTree(Node* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }

    if (node->move.empty()) {
        cout << "Start: ";
    } else {
        cout << node->move << ": ";
    }

    printState(node->state);
    cout << '\n';

    Node* child = node->firstChild;
    while (child != nullptr) {
        printTree(child, depth + 1);
        child = child->nextSibling;
    }
}




void printSolutions(Node* node, vector<Node*>& path) {
    path.push_back(node);

    State goal{1, 1, 1, 1};

    if (sameState(node->state, goal)) {
        cout << "Solution:\n";
        printState(path[0]->state);
        cout << '\n';

        for (size_t i = 1; i < path.size(); ++i) {
            cout << "  " << path[i]->move << " -> ";
            printState(path[i]->state);
            cout << '\n';
        }
        cout << '\n';
    } else {
        Node* child = node->firstChild;
        while (child != nullptr) {
            printSolutions(child, path);
            child = child->nextSibling;
        }
    }

    path.pop_back();
}



int main() {
    State start{0,0,0,0};

    cout << (isSafe(start) ? "Start state is safe\n" : "Start state is unsafe\n");


    Node* root = new Node(start);
    vector<State> path{start};

    buildTree(root, path);

    cout << "\nState Transition Tree:\n";
    printTree(root);

    
    vector<Node*> solutionPath;
    printSolutions(root, solutionPath);
    
    return 0;
}
