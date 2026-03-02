// PROVA FEITA EM DUPLA


// ! Copyright [2025] <Elisa Belquis 24202220 & João Bruno dos Santos 24203049>

#include <cstdint>  // std::size_t
#include <stdexcept>  // C++ exceptions


namespace structures {

//! Classe Lista Encadeada
template<typename T>
class LinkedList {
 public:
    LinkedList();  // construtor padrão
    ~LinkedList();  // destrutor
    void clear();  // limpar lista
    void push_back(const T& data);  // inserir no fim
    void push_front(const T& data);  // inserir no início
    void insert(const T& data, std::size_t index);  // inserir na posição
    void insert_sorted(const T& data);  // inserir em ordem
    T& at(std::size_t index);  // acessar um elemento na posição index
    T pop(std::size_t index);  // retirar da posição
    T pop_back();  // retirar do fim
    T pop_front();  // retirar do início
    void remove(const T& data);  // remover específico
    bool empty() const;  // lista vazia
    bool contains(const T& data) const;  // contém
    std::size_t find(const T& data) const;  // posição do dado
    std::size_t size() const;  // tamanho da lista

    //*******************************************************************
    // Prova prática - implementações necessárias:

    // (1) inverter a lista, ou seja, o primeiro passa a ser o último,
    //     o segundo passa a ser o penúltimo, e assim por diante:
    void invert();

    // (2) duplicar a lista, ou seja, acrescentar os mesmos elementos,
    //     na mesma ordem, no final:
    void doubling();

    // (3) criar uma lista com dados das posições de
    //     início (start) até fim (stop), saltando com um passo (step):
    LinkedList<T> slicing(int start, int stop, int step);

    // (4) criar um ponto de cruzamento (crossover) entre a lista original
    //     e uma lista de entrada (list_in), de mesmo tamanho par,
    //     exatamente no meio:
    void crossover(structures::LinkedList<T> *list_in);

    // (5) criar uma lista contendo outras duas listas:
    //     a primeira, correspondente aos dados em posições pares
    //     a segunda, correspondente aos dados em posições ímpares:
    LinkedList< LinkedList<T> * > halve();

    //*******************************************************************

 private:
    class Node {  // Elemento
     public:
        //! Construtor usando apenas o dado.
        explicit Node(const T& data):
        data_{data}
        {}

        //! Construtor de um nodo completo.
        explicit Node(const T& data, Node* next):
        data_{data},
        next_{next}
        {}

        //! Retorna o dado armazenado.
        T& data() {
            return data_;
        }

        //! Retorna o dado armazenado.
        const T& data() const {
            return data_;
        }

        //! Retorna ponteiro para próximo nodo.
        Node* next() {  // getter: próximo
            return next_;
        }

        //! Retorna ponteiro para o o próximo node.
        const Node* next() const {  // getter const: próximo
            return next_;
        }

        //! Altera o ponteiro para o próximo.
        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;  // data_
        Node* next_{nullptr};  // next_
    };

    //! Passa pelos nós até o último.
    Node* end() {  // último nó da lista
        auto it = head;
        for (auto i = 1u; i < size(); ++i) {
            it = it->next();
        }
        return it;
    }

    //! Passa pelos nós até o anterior ao índice procurado.
    Node* before_index(std::size_t index) {  // nó anterior ao index
        auto it = head;
        for (auto i = 1u; i < index; ++i) {
            it = it->next();
        }
        return it;
    }

    void insert(const T& data, Node* before);  // inserir na posição polimórfico

    Node* head{nullptr};  // head
    std::size_t size_{0u};  // size_
};

}  // namespace structures


//*******************************************************************

//! Inversão da lista
template<typename T>
void structures::LinkedList<T>::invert() {
    Node *previous = nullptr;
    Node *atual = head;
    Node *next = nullptr;

    while (atual != nullptr) {
        next = atual->next();
        atual->next(previous);
        previous = atual;
        atual = next;
    }

    head = previous;
}

//! Duplicação da lista
template<typename T>
void structures::LinkedList<T>::doubling() {
    Node *atual = head;
    size_t sizeInicial = size();

    for (size_t i = 0; i < sizeInicial; i++) {
        push_back(atual->data());
        atual = atual->next();
    }
}

//! Fatiamento da lista
template<typename T>
structures::LinkedList<T> structures::LinkedList<T>::slicing(int start,
                                                             int stop,
                                                             int step) {
    LinkedList<T> list_slice;

    for (int i = start; i < stop; i+=step) {
        list_slice.push_back(at(i));
    }

    return list_slice;
}

//! Crossover entre a lista original e uma lista de entrada (list_in)
template<typename T>
void structures::LinkedList<T>::crossover(structures::LinkedList<T> *list_in) {
    Node *atual1 = head;
    Node *atual2 = list_in->head;
    Node* aux;

    for (size_t i = 0; i < (size()/2-1); i++) {
        atual1 = atual1->next();
        atual2 = atual2->next();
    }

    aux = atual1->next();
    atual1->next(atual2->next());
    atual2->next(aux);
}

//! Divisão da lista em duas partes (elementos com índices pares e ímpares)
template<typename T>
structures::LinkedList< structures::LinkedList<T> * >
                                           structures::LinkedList<T>::halve() {
    LinkedList< LinkedList<T> * > list_halve;
    LinkedList<T> *pares = new LinkedList<T>();
    LinkedList<T> *impares = new LinkedList<T>();

    for (size_t i = 0; i < size(); i++) {
        if (i%2 == 0) {
            pares->push_back(at(i));
        } else {
            impares->push_back(at(i));
        }
    }

    list_halve.push_back(pares);
    list_halve.push_back(impares);

    return list_halve;
}

//*******************************************************************


//! Construtor padrão
template<typename T>
structures::LinkedList<T>::LinkedList() {}

//! Destrutor
template<typename T>
structures::LinkedList<T>::~LinkedList() {
    clear();
}

//! Esvazia a lista.
template<typename T>
void structures::LinkedList<T>::clear() {
    while (!empty())
        pop_front();
}

//! Inserção no fim da lista.
template<typename T>
void structures::LinkedList<T>::push_back(const T& data) {
    insert(data, size_);
}

//! Inserção no começo da lista.
template<typename T>
void structures::LinkedList<T>::push_front(const T& data) {
    Node* new_node = new Node(data);
    if (new_node == nullptr)
        throw std::out_of_range("Full list!");

    new_node->next(head);
    head = new_node;
    size_++;
}

//! Inserção em qualquer posição da lista.
template<typename T>
void structures::LinkedList<T>::insert(const T& data, std::size_t index) {
    if (index > size_)
        throw std::out_of_range("Invalid index!");

    if (index == 0) {
        push_front(data);
    } else {
        Node* new_node = new Node(data);
        if (new_node == nullptr)
            throw std::out_of_range("Full list!");

        Node* before = before_index(index);
        Node* next = before->next();
        new_node->next(next);
        before->next(new_node);
        size_++;
    }
}

//! Inserção em qualquer posição da lista recebendo um ponteiro para um Node.
template<typename T>
void structures::LinkedList<T>::insert(const T& data, Node* before) {
    Node* new_node = new Node(data);
    if (new_node == nullptr)
        throw std::out_of_range("Full list!");

    new_node->next(before->next());
    before->next(new_node);
    size_++;
}

//! Inserção ordenada na lista.
template<typename T>
void structures::LinkedList<T>::insert_sorted(const T& data) {
    if (empty()) {
        push_front(data);
    } else {
        Node* current = head;
        Node* before = head;
        std::size_t position = size();
        for (auto i = 0u; i < size(); ++i) {
            if (!(data > current->data())) {
                position = i;
                break;
            }
            before = current;
            current = current->next();
        }
        position == 0? push_front(data) : insert(data, before);
    }
}

//! Coleta o dado na posição da lista.
template<typename T>
T& structures::LinkedList<T>::at(std::size_t index) {
    if (index >= size())
        throw std::out_of_range("Invalid index or empty list!");

    Node* current = index == 0? head : before_index(index)->next();
    return current->data();
}

//! Coleta o dado de uma posição específica, removendo-o da lista.
template<typename T>
T structures::LinkedList<T>::pop(std::size_t index) {
    if (empty())
        throw std::out_of_range("Empty list");
    if (index >= size())
        throw std::out_of_range("Invalid index!");

    if (index == 0)
        return pop_front();

    Node* before_out = before_index(index);
    Node* out = before_out->next();
    T data = out->data();
    before_out->next(out->next());
    size_--;
    delete out;
    return data;
}

//! Coleta o dado do final, removendo-o da lista
template<typename T>
T structures::LinkedList<T>::pop_back() {
    return pop(size_ - 1u);
}

//! Coleta o dado do início, removendo-o da lista
template<typename T>
T structures::LinkedList<T>::pop_front() {
    if (empty())
        throw std::out_of_range("Empty list!");

    auto out = head;
    T data = out->data();
    head = out->next();
    size_--;
    delete out;
    return data;
}

//! Remoção de um dado da lista.
template<typename T>
void structures::LinkedList<T>::remove(const T& data) {
    pop(find(data));
}

//! Testa se a lista está vazia.
template<typename T>
bool structures::LinkedList<T>::empty() const {
    return size() == 0u;
}

//! Testa se um dado está na lista.
template<typename T>
bool structures::LinkedList<T>::contains(const T& data) const {
    return find(data) != size();
}

//! Procura o índice do dado; se não encontrar, retorna o tamanho da lista.
template<typename T>
std::size_t structures::LinkedList<T>::find(const T& data) const {
    std::size_t index = 0u;
    Node* current = head;
    while (index < size()) {
        if (current->data() == data)
            break;
        current = current->next();
        index++;
    }
    return index;
}

//! Retorna o tamanho da lista.
template<typename T>
std::size_t structures::LinkedList<T>::size() const {
    return size_;
}
