#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct NoTrie {
    NoTrie* filhos[26];
    unsigned long posicao;
    unsigned long comprimento;
    bool fim;

    NoTrie() {
        for (int i = 0; i < 26; i++) filhos[i] = nullptr;
        posicao = 0;
        comprimento = 0;
        fim = false;
    }
};

void inserir(NoTrie* raiz, const string& palavra, unsigned long pos, unsigned long comp) {
    NoTrie* atual = raiz;

    for (char c : palavra) {
        int idx = c - 'a';
        if (!atual->filhos[idx]) atual->filhos[idx] = new NoTrie();
        atual = atual->filhos[idx];
    }

    atual->fim = true;
    atual->posicao = pos;
    atual->comprimento = comp;
}

unsigned long contaPalavras(NoTrie* node) {
    if (!node) return 0;

    unsigned long total = node->fim ? 1 : 0;

    for (int i = 0; i < 26; i++)
        total += contaPalavras(node->filhos[i]);

    return total;
}

NoTrie* buscaPrefixo(NoTrie* raiz, const string& palavra) {
    NoTrie* atual = raiz;

    for (char c : palavra) {
        int idx = c - 'a';
        if (!atual->filhos[idx]) return nullptr;
        atual = atual->filhos[idx];
    }

    return atual;
}

void carregarDicionario(NoTrie* raiz, const string& filename) {
    ifstream arq(filename);
    string linha;

    unsigned long pos = 0;

    while (getline(arq, linha)) {
        unsigned long comp = linha.size();

        int ini = linha.find('[') + 1;
        int fim = linha.find(']') - 1;

        string palavra = linha.substr(ini, fim - ini + 1);

        inserir(raiz, palavra, pos, comp);

        pos = arq.tellg();
    }
}

int main() {
    string filename, word;
    cin >> filename;

    NoTrie* raiz = new NoTrie();
    carregarDicionario(raiz, filename);

    while (true) {
        cin >> word;
        if (word == "0") break;

        NoTrie* nodo = buscaPrefixo(raiz, word);

        if (!nodo) {
            cout << word << " is not prefix" << endl;
            continue;
        }

        unsigned long total = contaPalavras(nodo);

        cout << word << " is prefix of " << total << " words" << endl;

        if (nodo->fim) {
            cout << word << " is at (" << nodo->posicao << "," << nodo->comprimento << ")" << endl;
        }
    }

    return 0;
}