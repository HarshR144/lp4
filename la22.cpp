#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <chrono>
#include <iomanip>
#include <sys/stat.h>
using namespace std;
using namespace std::chrono;

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

// Structure to store performance metrics
struct PerformanceMetrics {
    double encodingTime;   // in milliseconds
    double decodingTime;   // in milliseconds
    size_t originalSize;   // in bytes
    size_t compressedSize; // in bytes
    size_t peakMemory;     // in bytes
    
    void print() const {
        cout << fixed << setprecision(2);
        cout << "\nPerformance Metrics:" << endl;
        cout << "===================" << endl;
        cout << "Time Complexity:" << endl;
        cout << "  Encoding Time: " << encodingTime << " ms" << endl;
        cout << "  Decoding Time: " << decodingTime << " ms" << endl;
        cout << "\nSpace Complexity:" << endl;
        cout << "  Original File Size: " << originalSize << " bytes" << endl;
        cout << "  Compressed File Size: " << compressedSize << " bytes" << endl;
        cout << "  Compression Ratio: " << (float)compressedSize/originalSize * 100 << "%" << endl;
        cout << "  Space Saved: " << 100 - ((float)compressedSize/originalSize * 100) << "%" << endl;
        cout << "  Peak Memory Usage: " << peakMemory/1024 << " KB" << endl;
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

// Get file size
size_t getFileSize(const string& filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : 0;
}

// Utility function to estimate current memory usage
size_t getCurrentMemoryUsage() {
    // This is a simple approximation
    return sizeof(Node) * 256 + sizeof(char) * 1024 * 1024;
}

vector<unsigned char> convertToBytes(const string& binStr) {
    vector<unsigned char> bytes;
    for(size_t i = 0; i < binStr.length(); i += 8) {
        string byte = binStr.substr(i, 8);
        while(byte.length() < 8) byte += "0";
        bytes.push_back(static_cast<unsigned char>(bitset<8>(byte).to_ulong()));
    }
    return bytes;
}

class HuffmanCoder {
private:
    size_t peakMemory;
    
    void updatePeakMemory() {
        size_t currentMemory = getCurrentMemoryUsage();
        peakMemory = max(peakMemory, currentMemory);
    }

public:
    HuffmanCoder() : peakMemory(0) {}
    
    PerformanceMetrics processFile(const string& inputFile) {
        PerformanceMetrics metrics;
        peakMemory = 0;
        
        // Check if file exists
        ifstream checkFile(inputFile);
        if(!checkFile) {
            throw runtime_error("Error: Input file does not exist!");
        }
        checkFile.close();
        
        // Get original file size
        metrics.originalSize = getFileSize(inputFile);
        cout << "\nProcessing file: " << inputFile << " (" << metrics.originalSize << " bytes)" << endl;
        
        // Read input file
        ifstream inFile(inputFile, ios::binary);
        string text((istreambuf_iterator<char>(inFile)), {});
        inFile.close();
        updatePeakMemory();
        
        // Start encoding
        auto startEncode = high_resolution_clock::now();
        
        // Count frequencies
        unordered_map<char, int> freq;
        for(char c : text) {
            freq[c]++;
        }
        updatePeakMemory();
        
        // Create Huffman tree
        priority_queue<Node*, vector<Node*>, Compare> pq;
        for(const auto& pair : freq) {
            pq.push(new Node(pair.first, pair.second));
        }
        
        while(pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            pq.push(new Node('\0', left->freq + right->freq, left, right));
        }
        
        // Generate Huffman codes
        unordered_map<char, string> huffmanCode;
        generateCodes(pq.top(), "", huffmanCode);
        
        // Display Huffman codes
        cout << "\nHuffman Codes:" << endl;
        cout << "==============" << endl;
        for(const auto& pair : huffmanCode) {
            cout << "'" << (isprint(pair.first) ? pair.first : ' ') 
                 << "' (" << (int)pair.first << "): " << pair.second << endl;
        }
        
        // Create encoded text
        string encodedText;
        for(char c : text) {
            encodedText += huffmanCode[c];
        }
        updatePeakMemory();
        
        // Write compressed file
        string compressedFile = inputFile + ".huf";
        ofstream outFile(compressedFile, ios::binary);
        
        // Write frequency table size and table
        int tableSize = freq.size();
        outFile.write(reinterpret_cast<const char*>(&tableSize), sizeof(tableSize));
        for(const auto& pair : freq) {
            outFile.write(&pair.first, sizeof(pair.first));
            outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }
        
        // Write encoded data
        int bitCount = encodedText.length();
        outFile.write(reinterpret_cast<const char*>(&bitCount), sizeof(bitCount));
        
        vector<unsigned char> bytes = convertToBytes(encodedText);
        outFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
        outFile.close();
        
        auto endEncode = high_resolution_clock::now();
        metrics.encodingTime = duration_cast<milliseconds>(endEncode - startEncode).count();
        
        // Start decoding
        auto startDecode = high_resolution_clock::now();
        
        string decodedFile = inputFile + ".decoded";
        ofstream decFile(decodedFile);
        
        // Decode using Huffman tree
        Node* current = pq.top();
        for(char bit : encodedText) {
            if(bit == '0') current = current->left;
            else current = current->right;
            
            if(!current->left && !current->right) {
                decFile.put(current->ch);
                current = pq.top();
            }
        }
        decFile.close();
        
        auto endDecode = high_resolution_clock::now();
        metrics.decodingTime = duration_cast<milliseconds>(endDecode - startDecode).count();
        
        // Get compressed file size
        metrics.compressedSize = getFileSize(compressedFile);
        metrics.peakMemory = peakMemory;
        
        // Cleanup
        while(!pq.empty()) {
            delete pq.top();
            pq.pop();
        }
        
        cout << "\nFiles created:" << endl;
        cout << "1. Compressed file: " << compressedFile << endl;
        cout << "2. Decoded file: " << decodedFile << endl;
        
        return metrics;
    }
};

int main() {
    cout << "Huffman Coding Analysis Program" << endl;
    cout << "==============================" << endl;
    
    HuffmanCoder coder;
    string inputFile;
    
    while(true) {
        cout << "\nEnter the path to the file to compress (or 'q' to quit): ";
        getline(cin, inputFile);
        
        if(inputFile == "q" || inputFile == "Q") {
            break;
        }
        
        try {
            PerformanceMetrics metrics = coder.processFile(inputFile);
            metrics.print();
        }
        catch(const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return 0;
}