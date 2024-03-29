#include "parque.h"
#include "insertionSort.h"
#include "sequentialSearch.h"
#include <algorithm>
#include <vector>

using namespace std;

ParqueEstacionamento::ParqueEstacionamento(unsigned int lot, unsigned int nMaxCli): lotacao(lot), numMaximoClientes(nMaxCli) {
    numClientes = 0;
    vagas=lot; 
}

ParqueEstacionamento::~ParqueEstacionamento() {}

vector<InfoCartao> ParqueEstacionamento::getClientes() const {
	return clientes;
}

unsigned int ParqueEstacionamento::getNumLugares() const {
	return lotacao;
}

unsigned int ParqueEstacionamento::getNumLugaresOcupados() const {
	return lotacao-vagas;
}

int ParqueEstacionamento::posicaoCliente(const string &nome) const {
	vector<string> nomes;
	string n = nome;

	for (const auto & cliente : clientes)
		nomes.push_back(cliente.nome);

    return sequentialSearch(nomes, n);
}

int ParqueEstacionamento::getFrequencia(const string &nome) const {
	if (posicaoCliente(nome) == -1)
		throw ClienteNaoExistente(nome);

    return clientes[posicaoCliente(nome)].frequencia;
}

// a alterar/atualizar ?
bool ParqueEstacionamento::adicionaCliente(const string & nome) {
 	if (numClientes == numMaximoClientes )
 		return false;

	if (posicaoCliente(nome) != -1 )
		return false;

	InfoCartao info;
	info.nome = nome;
    info.presente = false;
	clientes.push_back(info);
	numClientes++;

	return true;
}

// a alterar/atualizar ?
bool ParqueEstacionamento::retiraCliente(const string & nome) {
	for (vector<InfoCartao>::iterator it = clientes.begin(); it != clientes.end(); it++)
		if ( it->nome == nome ) {
			if (!it->presente) {
				clientes.erase(it);
				numClientes--;
				return true;
			}

			else
				return false;
		}
	return false;
}

// a alterar/atualizar ?
bool ParqueEstacionamento::entrar(const string & nome) {
	if (vagas == 0)
		return false;

	int pos = posicaoCliente(nome);

	if ( pos == -1 )
		return false;

	if (clientes[pos].presente)
		return false;

	clientes[pos].presente = true;
	clientes[pos].frequencia++;
	vagas--;

	return true;
}

// a alterar/atualizar ?
bool ParqueEstacionamento::sair(const string & nome) {
	int pos = posicaoCliente(nome);
	if ( pos == -1 ) return false;
	if (!clientes[pos].presente)
		return false;

	clientes[pos].presente = false;
	vagas++;
	return true;
}

// a implementar
void ParqueEstacionamento::ordenaClientesPorFrequencia() {
}

// a implementar
void ParqueEstacionamento::ordenaClientesPorNome() {
}

// a implementar
vector<string> ParqueEstacionamento::clientesGamaUso(int n1, int n2) {
    vector<string> nomes;
    return nomes;
}

// a implementar
ostream & operator<<(ostream & os, const ParqueEstacionamento & pe) {
    return os;
}

// a implmentar
InfoCartao ParqueEstacionamento::getClienteAtPos(vector<InfoCartao>::size_type p) const {
    InfoCartao info;
    return info;
}

string ClienteNaoExistente::getNome() {
	return nome;
}

ClienteNaoExistente::ClienteNaoExistente(string nome) {
	this->nome = nome;
}
