#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


using namespace std;

// criando a struct de time
struct Time{
    string nome;
    int pontos;
    int vitorias;
    int golsPro;
    int empates;
    int derrotas;
    int saldoGols;
    int jogoDesempate; // não tem no arquivo txt
    int golsContra;
};

// variaveis globais
    const int QTD = 4;
    vector<Time> listaTimes;

// função que lê o arquivo com os times e os pontos desordenados
// arquvio tem que ter tudo que tem na struct
void arquivo (){

    ifstream arquivo ("times.txt");

    string linha, parte;
    char delimitador = ';';
    int i = 0; // contador para preencher os vetores antigos também

    if (!arquivo.is_open()){
        cout << "Erro ao abrir arquivo!" << endl;
        return;
    }

    if (arquivo.is_open()) {
        while (getline(arquivo, linha) && i < QTD) {
            stringstream registro (linha);
            vector<string> campos;

            while (getline (registro, parte, delimitador)) {
                campos.push_back(parte);
            }

            // alimenta o vetor listaTimes direto da struct
             if (campos.size() >= 2) {
                Time t; // cria um tipo da struct

                t.nome = campos[0];
                t.pontos = stoi(campos[1]); // le como string e transforma em int
                t.vitorias = stoi(campos[2]);
                t.golsPro = stoi(campos[3]);
                t.empates = stoi(campos[4]);
                t.derrotas = stoi(campos[5]);
                t.saldoGols = stoi(campos[6]);
                t.golsContra = stoi(campos[7]);
                t.jogoDesempate = 0;

                listaTimes.push_back(t); // adiciona um novo elemento ao final de um vetor
            }
        }
        arquivo.close();
    }
}


// metodo ordenação bolha - para ordenar os times em classificação
void ordena() {
    bool troca;
    do {
        troca = false;
        if(listaTimes.empty()) break; // proteção se o vetor estiver vazio para

        for (int i = 0; i < listaTimes.size() - 1; i++) {
            bool precisaTrocar = false;

            if (listaTimes[i].pontos < listaTimes[i+1].pontos) {
                precisaTrocar = true;
            }

            // desempates
            // por Saldo de Gols
            else if (listaTimes[i].pontos == listaTimes[i+1].pontos &&
                     listaTimes[i].saldoGols < listaTimes[i+1].saldoGols) {
                precisaTrocar = true;
            }
            // por Gols Pro
            else if (listaTimes[i].pontos == listaTimes[i+1].pontos &&
                     listaTimes[i].saldoGols == listaTimes[i+1].saldoGols &&
                     listaTimes[i].golsPro < listaTimes[i+1].golsPro) {
                precisaTrocar = true;
            }

            // jogo para desempate
            else if (listaTimes[i].pontos == listaTimes[i+1].pontos &&
                     listaTimes[i].saldoGols == listaTimes[i+1].saldoGols &&
                     listaTimes[i].golsPro == listaTimes[i+1].golsPro) {

                // só jogam se as variáveis de desempate estiverem zeradas (ou seja, ainda não jogaram)
                // se forem diferentes de zero, significa que o jogo já aconteceu em uma rodada anterior do Bubble Sort
                if (listaTimes[i].jogoDesempate == 0 && listaTimes[i+1].jogoDesempate == 0) {

                    int gols1, gols2;
                    // sorteia até que os gols sejam diferentes (evita empate no jogo único)
                    do {
                        gols1 = rand() % 6;
                        gols2 = rand() % 6;
                    } while (gols1 == gols2);

                    // grava o resultado definitivo na Struct
                    listaTimes[i].jogoDesempate = gols1;
                    listaTimes[i+1].jogoDesempate = gols2;

                    cout << "=====================================================================" << endl;
                    cout << "                          JOGO DE DESEMPATE"                           << endl;
                    cout << "=====================================================================" << endl;
                    cout << "\n" << listaTimes[i].nome << " " << gols1 << "  X  " << gols2 << " " << listaTimes[i+1].nome << endl;

                    if (gols1 < gols2) {
                        cout << "VENCEDOR: " << listaTimes[i+1].nome << endl;
                        cout << endl;
                    } else {
                        cout << "VENCEDOR: " << listaTimes[i].nome << endl;
                        cout << endl;
                    }
                }

                // a decisão de troca olha os valores gravados com segurança
                if (listaTimes[i].jogoDesempate < listaTimes[i+1].jogoDesempate) {
                    precisaTrocar = true;
                }
            }

            if (precisaTrocar) {
                // copia o TIME inteiro (nome, pontos, gols, tudo junto) para a variável auxiliar
                Time aux = listaTimes[i];
                listaTimes[i] = listaTimes[i+1];
                listaTimes[i+1] = aux;
                troca = true;
            }
        }
    } while (troca);
}

// times ordenados escritos em um arquivo
void timesOrdenados (){
    ofstream arquivo;
    arquivo.open("timesOrdenados.txt", ios::out);

    if (!arquivo.is_open()){
        cout << "Erro ao abrir arquivo!" << endl;
        return;
    }

    for (int i = 0; i < QTD; i++) {
        arquivo << listaTimes[i].nome << "; "
                << listaTimes[i].pontos << "; "
                << listaTimes[i].vitorias << "; "
                << listaTimes[i].golsPro << "; "
                << listaTimes[i].empates << "; "
                << listaTimes[i].derrotas << "; "
                << listaTimes[i].saldoGols << "; "
                << listaTimes[i].golsContra << "; "
                << endl;
    }

    arquivo.close();
}

Time simularMataMata(vector<Time> timesFase, string nomeDaFase) {
    if (timesFase.size() == 1) {
        return timesFase[0];
    }

    cout << "\n--- " << nomeDaFase << " ---" << endl;
    vector<Time> vencedoresProximaFase;

    // processa os jogos de 2 em 2 (Primeiro da lista vs Último da lista)
    while (!timesFase.empty()) {
        Time casa = timesFase.front();       // pega o primeiro time do vetor
        Time visitante = timesFase.back();   // pega o último time do vetor

        // gera placar aleatório
        int golsCasa = rand() % 6;
        int golsVisitante = rand() % 6;

        // evita empates no mata-mata
        while (golsCasa == golsVisitante) {
            cout << casa.nome << " " << golsCasa << " X " << golsVisitante << " " << visitante.nome << " *EMPATE*" << endl;
            golsVisitante = rand() % 6;
            golsCasa = rand() % 6;
        }

        cout << casa.nome << " " << golsCasa << " X " << golsVisitante << " " << visitante.nome << endl;

        // decide quem avança e guarda na lista de vencedores
        if (golsCasa > golsVisitante) {
            vencedoresProximaFase.push_back(casa);
        } else {
            vencedoresProximaFase.push_back(visitante);
        }

        // remove os dois times que já jogaram para passar para os próximos
        timesFase.erase(timesFase.begin());
        timesFase.pop_back();
    }

    // define o nome da próxima etapa
    string proximaFase = (vencedoresProximaFase.size() == 2) ? "GRANDE FINAL" : "PROXIMA FASE";

    // a função chama ela mesma passando apenas os que ganharam
    return simularMataMata(vencedoresProximaFase, proximaFase);
}

// módulo principal do chaveamento
void chaveamento() {
    srand(time(0)); // usa o tempo atual para gerar o valor aleatorio

    cout << "=====================================================================" << endl;
    cout << "                  FASE CHAVEAMENTO FINAL (COPA '70)                  " << endl;
    cout << "=====================================================================" << endl;

    // chuta o início da recursão passando a lista de times classificados
    Time campeaoDoTorneio = simularMataMata(listaTimes, "SEMIFINAIS");

    cout << "\n CAMPEAO: " << campeaoDoTorneio.nome << " " << endl;
    cout << endl;
}

// tabela final
 void imprimirTabela(const vector<Time>& times) {
	cout << "=====================================================================" << endl;
	cout << "                     TABELA DE PONTOS FINAIS                         " << endl;
	cout << "=====================================================================" << endl;

    // Cabeçalho da tabela
    cout << left  << setw(4)  << "Pos"
         << setw(12) << "Nome"
         << right << setw(5)  << "P"
         << setw(5)  << "V"
         << setw(5)  << "GP"
         << setw(5)  << "E"
         << setw(5)  << "D"
         << setw(5)  << "SG"
         << setw(5)  << "GC" << endl;

    cout << "---------------------------------------------------------------------" << endl;

    // Dados da tabela
    for (size_t i = 0; i < times.size(); i++) {
        cout << left  << setw(4)  << i + 1
             << setw(12) << times[i].nome
             << right << setw(5)  << times[i].pontos
             << setw(5)  << times[i].vitorias
             << setw(5)  << times[i].golsPro
             << setw(5)  << times[i].empates
             << setw(5)  << times[i].derrotas
             << setw(5)  << times[i].saldoGols
             << setw(5)  << times[i].golsContra << endl;
    }

}

int main (){
    // chamando os módulos
    arquivo();
    ordena();
    timesOrdenados();
    imprimirTabela(listaTimes);

    cout << endl;
    cout << endl;
    chaveamento();

    return 0;
}
