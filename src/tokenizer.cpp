#include "tokenizer.h"

int getType(char c) {
    if (isalpha(c)) return 1;
    if (isdigit(c)) return 2;
    if (isspace(c)) return 0;
    return -1;
}

Tokenizer::Tokenizer(const char* address) {
    ifstream document(address);

    if (document) {
        document.seekg(0, document.end);
        int length = document.tellg();
        document.seekg(0, document.beg);

        char *buffer = new char[length];
        document.read(buffer, length);
        if (!document)
            cout << "error: only " << document.gcount() << " could be read" << endl;

        this->size = length;
        this->document = string(buffer);

        /*  here is the generator of tokenOrder.
         *  The first few whitespace order would be -1
         *  The substring of tokenOrder will be "order order ... order -order -order... -order"
         */
        unsigned i = 0;
        //  The first few whitespace order would be -1
        while (isspace(this->document[i])) {
            i++;
            this->tokenOrder.push_back(-1);
        }
        // The rest of tokenOrder will be "order order ... order -order -order... -order"
        int order = 0;
        int lastType = 0;
        for (; i < this->size; i++) {
            int t = getType(this->document[i]);
            if (t > 0) {
                if (t != lastType) order++;
                this->tokenOrder.push_back(order);
            } else if (t == 0) {
                this->tokenOrder.push_back(-order);
            } else {
                order++;
                this->tokenOrder.push_back(order);
            }
            lastType = t;
        }
        delete[] buffer;
        document.close();
    }
}

Tokenizer::Tokenizer() {}
