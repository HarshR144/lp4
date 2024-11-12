#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <string>
#include <queue>

using namespace std;

class Node {
public:
    char symbol;
    int freq;
    Node* left;
    Node* right;

    Node(char symbol, int freq, Node* left = nullptr, Node* right = nullptr)
        : symbol(symbol), freq(freq), left(left), right(right) {}

    // Overload the < operator to use the priority queue
    bool operator<(const Node& other) const {
        return freq > other.freq; // Reverse the comparison to make the priority queue a min-heap
    }
};

// Helper function to recursively calculate Huffman codes
void calculateHuffmanCodes(Node* node, const string& code, map<char, string>& huffmanCodes) {
    if (node) {
        // If it's a leaf node, add code to the map
        if (!node->left && !node->right) {
            huffmanCodes[node->symbol] = code;
        }
        // Recur for left and right children
        calculateHuffmanCodes(node->left, code + "0", huffmanCodes);
        calculateHuffmanCodes(node->right, code + "1", huffmanCodes);
    }
}

string encodeString(const string& input, const map<char, string>& huffmanCodes) {
    string encodedString;
    for (char c : input) {
        encodedString += huffmanCodes.at(c);
    }
    return encodedString;
}

int main() {
    string input;
    cout << "Enter the input string: ";
    getline(cin, input);

    // Create a map to store the frequency of each character
    map<char, int> charFreq;
    for (char c : input) {
        charFreq[c]++;
    }

    // Convert the map to a priority queue (min-heap)
    priority_queue<Node*> pq;
    for (const auto& kv : charFreq) {
        pq.push(new Node(kv.first, kv.second));
    }

    // Start measuring time for Huffman tree construction
    clock_t start_time = clock();

    // Build the Huffman Tree by repeatedly combining the two lowest-frequency nodes
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        Node* newNode = new Node('\0', left->freq + right->freq, left, right);
        pq.push(newNode);
    }

    // End time for Huffman tree construction
    clock_t end_time = clock();
    double tree_duration = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Start measuring time for Huffman code calculation
    clock_t code_start_time = clock();
    map<char, string> huffmanCodes;
    calculateHuffmanCodes(pq.top(), "", huffmanCodes); // Root of the Huffman tree is in pq.top()
    clock_t code_end_time = clock();
    double code_duration = double(code_end_time - code_start_time) / CLOCKS_PER_SEC;

    // Calculate estimated space in bytes for storing Huffman codes
    double spaceUsed = 0;
    for (const auto& kv : huffmanCodes) {
        spaceUsed += kv.second.length() * charFreq[kv.first];
    }
    spaceUsed = ceil(spaceUsed / 8);

    // Encode the input string
    string encodedString = encodeString(input, huffmanCodes);

    // Output results
    cout << "Huffman Tree Construction Time: " << tree_duration << " seconds" << endl;
    cout << "Huffman Code Calculation Time: " << code_duration << " seconds" << endl;
    cout << "Estimated Space Used for Huffman Codes: " << spaceUsed << " bytes" << endl;
    cout << "Encoded String: " << encodedString << endl;

    return 0;
}