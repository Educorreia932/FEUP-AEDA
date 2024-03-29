/*
 * jogo.cpp
 */

#include "jogo.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

unsigned int Jogo::numPalavras(string frase) {
    if ( frase.length() == 0 ) return 0;

    unsigned n=1;
    size_t pos = frase.find(' ');
    while (pos != string::npos) {
        frase = frase.substr(pos+1);
        pos = frase.find(' ');
        n++;
    }
    return n;
}

Jogo::Jogo() {
    criancas = {};
}

Jogo::Jogo(list<Crianca>& lc2) {
    criancas = lc2;
}

void Jogo::insereCrianca(const Crianca &c1) {
    criancas.push_back(c1);
}

list<Crianca> Jogo::getCriancasJogo() const {
    return criancas;
}

string Jogo::escreve() const {
    ostringstream result;

    for (auto const& i : criancas)
        result << i.getNome() << " : " << i.getIdade() << endl;

    return result.str();
}

Crianca& Jogo::perdeJogo(string frase) {
    Crianca *c1 = new Crianca();
    int perdedor = numPalavras(frase) % criancas.size();
    int counter = 0;
    list<Crianca>::iterator it;

    for (it = criancas.begin(); it != criancas.end(); it++) {
        if (counter == perdedor) {
            *c1 = *it;
            //criancas.erase(it);
        }

        counter++;
    }

    return *c1;
}

list<Crianca>& Jogo::inverte() {
    criancas.reverse();

    return criancas;
}

list<Crianca> Jogo::divide(unsigned id) {
    list<Crianca> result;

    list<Crianca>::iterator it;

    for (it = criancas.begin(); it != criancas.end(); it++) {
        if (it->getIdade() > id) {
            result.push_back(*it);
            it = criancas.erase(it);
            it--;
        }
    }

    return result;
}

void Jogo::setCriancasJogo(const list<Crianca>& l1) {
    criancas = l1;
}

bool Jogo::operator==(Jogo& j2) {
    return j2.getCriancasJogo() == criancas;
}

// a implementar
list<Crianca> Jogo::baralha() const {
    list<Crianca> result;
    int random, counter = 0, num_criancas = criancas.size();

    while (result.size() < num_criancas) {
        srand(time(nullptr));
        random = rand() % criancas.size();

        for (const auto &i : criancas) {
            if (counter == random) {
                result.push_back(i);
                break;
            }

            counter++;
        }

        counter = 0;
    }

    return result;
}
