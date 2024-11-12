#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char c, int f, Node* l = nullptr, Node* r = nullptr) {
        ch = c;
        freq = f;
        left = l;
        right = r;
    }
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) return;
    
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = code;
    }
    
    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

// Convert string of bits to actual bytes for file writing
vector<unsigned char> convertToBytes(const string& binStr) {
    vector<unsigned char> bytes;
    for(size_t i = 0; i < binStr.length(); i += 8) {
        string byte = binStr.substr(i, 8);
        // Pad with zeros if less than 8 bits
        while(byte.length() < 8) {
            byte += "0";
        }
        bytes.push_back(static_cast<unsigned char>(bitset<8>(byte).to_ulong()));
    }
    return bytes;
}

// Function to write Huffman codes to file for later decompression
void writeHuffmanCodes(const unordered_map<char, string>& huffmanCode, const string& filename) {
    ofstream codeFile(filename);
    for(const auto& pair : huffmanCode) {
        codeFile << pair.first << " " << pair.second << endl;
    }
    codeFile.close();
}

// Function to read Huffman codes from file
unordered_map<char, string> readHuffmanCodes(const string& filename) {
    unordered_map<char, string> huffmanCode;
    ifstream codeFile(filename);
    char ch;
    string code;
    
    while(codeFile >> ch >> code) {
        huffmanCode[ch] = code;
    }
    codeFile.close();
    return huffmanCode;
}

void compressFile(const string& inputFile, const string& outputFile, const string& codesFile) {
    // Read input file
    ifstream inFile(inputFile, ios::binary);
    if(!inFile) {
        cout << "Error opening input file!" << endl;
        return;
    }

    string text;
    char ch;
    while(inFile.get(ch)) {
        text += ch;
    }
    inFile.close();

    // Count frequencies
    unordered_map<char, int> freq;
    for(char c : text) {
        freq[c]++;
    }

    // Create priority queue
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for(const auto& pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build Huffman tree
    while(pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node('\0', left->freq + right->freq, left, right));
    }

    // Generate Huffman codes
    unordered_map<char, string> huffmanCode;
    generateCodes(pq.top(), "", huffmanCode);

    // Save Huffman codes for later decompression
    writeHuffmanCodes(huffmanCode, codesFile);

    // Create encoded text
    string encodedText;
    for(char c : text) {
        encodedText += huffmanCode[c];
    }

    // Write compressed data
    ofstream outFile(outputFile, ios::binary);
    if(!outFile) {
        cout << "Error opening output file!" << endl;
        return;
    }

    // Write the number of bits at the start (needed for decompression)
    int bitCount = encodedText.length();
    outFile.write(reinterpret_cast<const char*>(&bitCount), sizeof(bitCount));

    // Convert bit string to bytes and write to file
    vector<unsigned char> bytes = convertToBytes(encodedText);
    outFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    outFile.close();

    // Print compression statistics
    cout << "Compression Statistics:" << endl;
    cout << "Original size: " << text.length() << " bytes" << endl;
    cout << "Compressed size: " << bytes.size() << " bytes" << endl;
    cout << "Compression ratio: " << (float)bytes.size() / text.length() * 100 << "%" << endl;
}

void decompressFile(const string& inputFile, const string& outputFile, const string& codesFile) {
    // Read Huffman codes
    unordered_map<char, string> huffmanCode = readHuffmanCodes(codesFile);

    // Create reverse mapping for decoding
    unordered_map<string, char> reverseMap;
    for(const auto& pair : huffmanCode) {
        reverseMap[pair.second] = pair.first;
    }

    // Read compressed file
    ifstream inFile(inputFile, ios::binary);
    if(!inFile) {
        cout << "Error opening compressed file!" << endl;
        return;
    }

    // Read the number of bits
    int bitCount;
    inFile.read(reinterpret_cast<char*>(&bitCount), sizeof(bitCount));

    // Read compressed data
    vector<unsigned char> bytes((istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    // Convert bytes back to string of bits
    string encodedText;
    for(unsigned char byte : bytes) {
        encodedText += bitset<8>(byte).to_string();
    }
    encodedText = encodedText.substr(0, bitCount); // Take only the valid bits

    // Decode
    ofstream outFile(outputFile);
    if(!outFile) {
        cout << "Error opening output file!" << endl;
        return;
    }

    string currentCode;
    for(char bit : encodedText) {
        currentCode += bit;
        if(reverseMap.find(currentCode) != reverseMap.end()) {
            outFile.put(reverseMap[currentCode]);
            currentCode = "";
        }
    }
    outFile.close();
}

int main() {
    string choice;
    cout << "Enter 'c' to compress or 'd' to decompress: ";
    cin >> choice;

    if(choice == "c") {
        string inputFile, outputFile;
        cout << "Enter input file name: ";
        cin >> inputFile;
        cout << "Enter output file name: ";
        cin >> outputFile;

        compressFile(inputFile, outputFile, "huffman_codes.txt");
        cout << "File compressed successfully!" << endl;
    }
    else if(choice == "d") {
        string inputFile, outputFile;
        cout << "Enter compressed file name: ";
        cin >> inputFile;
        cout << "Enter output file name: ";
        cin >> outputFile;

        decompressFile(inputFile, outputFile, "huffman_codes.txt");
        cout << "File decompressed successfully!" << endl;
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}