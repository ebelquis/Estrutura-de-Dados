// CÓDIGO REALIZADO POR Elisa Belquis de Assumpcao E Miguel Estivalet Pinto 
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

class Cenario {
  public:
    Cenario(string& texto, size_t indice_inicial) {
        size_t pos = indice_inicial;
        nome = proxima_tag_conteudo(texto, pos, "nome");
        altura = static_cast<size_t>( stoi( proxima_tag_conteudo(texto, pos, "altura") ) );
        largura = static_cast<size_t>( stoi( proxima_tag_conteudo(texto, pos, "largura") ) );
        x = static_cast<size_t>( stoi( proxima_tag_conteudo(texto, pos, "x") ) );
        y = static_cast<size_t>( stoi( proxima_tag_conteudo(texto, pos, "y") ) );
        matriz = matriz_remove_espacos( proxima_tag_conteudo(texto, pos, "matriz") );
        indice_final = pos;
    }
    ~Cenario() {};
    string nome;
    size_t altura;
    size_t largura;
    size_t x;
    size_t y;
    string matriz;
    size_t indice_final;

  private:
    string proxima_tag(string& texto, size_t& pos) {
        string tag = "";
        for ( ; pos < texto.length(); pos++) {
            if (texto[pos] == '<') {
                pos++;
                while (texto[pos] != '>') {
                    tag += texto[pos];
                    pos++;
                }
                pos++;
                return tag;
            }
        }
        return tag;
    }
    string proximo_conteudo(string& texto, size_t& pos) {
        string txt = "";
        while (texto[pos] != '<') {
            txt += texto[pos];
            pos++;
        }
        while (texto[pos] != '>') {
            pos++;
        }
        pos++;
        return txt;
    }
    string proxima_tag_conteudo(string& texto, size_t& pos, string nome_tag) {
        string tag = "";
        while (tag != nome_tag) {
            tag = proxima_tag(texto, pos);
        }
        return proximo_conteudo(texto, pos);
    }
    string matriz_remove_espacos(string texto) {
        string saida;
        for (size_t i = 0; i < texto.length(); i++) {
            if (texto[i] == '0' || texto[i] == '1') {
                saida += texto[i];
            }
        }
        return saida;
    }
};

bool validar_xml(const string& texto) {
    stack<string> pilha;
    
    for (size_t i = 0; i < texto.length(); i++) {
        if (texto[i] == '<') {
            i++;
            if (texto[i] == '/') {
                i++;
                string tag_fechamento = "";
                
                while (i < texto.length() && texto[i] != '>') {
                    tag_fechamento += texto[i];
                    i++;
                }
                
                if (pilha.empty()) {
                    return false;
                }
                
                if (pilha.top() != tag_fechamento) {
                    return false;
                }
                
                pilha.pop();
            } 
            else {
                string tag_abertura = "";
                
                while (i < texto.length() && texto[i] != '>') {
                    tag_abertura += texto[i];
                    i++;
                }
                
                pilha.push(tag_abertura);
            }
        }
    }
    
    return pilha.empty();
}

size_t calcular_area_componente_conexo(const Cenario& cenario) {
    // Criar matriz de visitação (inicializada com zeros)
    vector<vector<bool>> visitado(cenario.altura, vector<bool>(cenario.largura, false));
    
    // Verificar se a posição inicial é válida
    if (cenario.x >= cenario.altura || cenario.y >= cenario.largura) {
        return 0;
    }
    
    // Verificar se a posição inicial tem valor 1
    size_t idx_matriz = cenario.x * cenario.largura + cenario.y;
    if (cenario.matriz[idx_matriz] != '1') {
        return 0;
    }
    
    // Fila para BFS
    queue<pair<size_t, size_t>> fila;
    
    // Inserir posição inicial na fila e marcar como visitado
    fila.push(make_pair(cenario.x, cenario.y));
    visitado[cenario.x][cenario.y] = true;
    
    size_t area = 0;
    
    // Direções para vizinhança-4: cima, baixo, esquerda, direita
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    while (!fila.empty()) {
        size_t x = fila.front().first;
        size_t y = fila.front().second;
        fila.pop();
        area++;
        
        // Verificar os 4 vizinhos
        for (int i = 0; i < 4; i++) {
            int nx_int = static_cast<int>(x) + dx[i];
            int ny_int = static_cast<int>(y) + dy[i];
            
            // Verificar se está dentro dos limites da matriz (evitando underflow)
            if (nx_int >= 0 && ny_int >= 0 && 
                nx_int < static_cast<int>(cenario.altura) && 
                ny_int < static_cast<int>(cenario.largura)) {
                
                size_t nx = static_cast<size_t>(nx_int);
                size_t ny = static_cast<size_t>(ny_int);
                
                // Verificar se não foi visitado e se tem valor 1 na matriz original
                size_t idx_vizinho = nx * cenario.largura + ny;
                if (!visitado[nx][ny] && cenario.matriz[idx_vizinho] == '1') {
                    visitado[nx][ny] = true;
                    fila.push(make_pair(nx, ny));
                }
            }
        }
    }
    
    return area;
}

/**********************
    FUNÇÃO PRINCIPAL
***********************/
int main() {

    string filename;

    std::cin >> filename;

    // Abertura do arquivo
    ifstream filexml(filename);
    if (!filexml.is_open()) {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        throw runtime_error("Erro no arquivo XML");
    }

    // Leitura do XML completo para 'texto'
    string texto;
    char character;
    while (filexml.get(character)) {
        texto += character;
    }
    
    // Validação do arquivo
    if (!validar_xml(texto)) {
        cout << "erro" << endl;
        return 1;
    }

    // Processar todos os cenários
    vector<Cenario> cenarios;
    size_t pos_atual = 0;
    
    while (pos_atual < texto.length()) {
        // Procurar próximo cenário
        size_t pos_cenario = texto.find("<cenario>", pos_atual);
        if (pos_cenario == string::npos) {
            break;
        }
        
        Cenario cenario(texto, pos_cenario);
        cenarios.push_back(cenario);
        pos_atual = cenario.indice_final;
    }

    // Calcular e imprimir áreas para cada cenário (COM O NOME) - esqueci de colocar o nome antes .-.
    for (size_t i = 0; i < cenarios.size(); i++) {
        size_t area = calcular_area_componente_conexo(cenarios[i]);
        cout << cenarios[i].nome << " " << area << endl;  // CORREÇÃO AQUI: incluir o nome
    }

    return 0;
}