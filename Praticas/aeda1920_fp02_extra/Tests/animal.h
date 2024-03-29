#ifndef SRC_ANIMAL_H_
#define SRC_ANIMAL_H_

#include "veterinario.h"
#include <string>

using namespace std;

class Animal {
    protected:
        string nome;
        int idade;
        Veterinario *vet;
        static int maisJovem;
        bool temVet = false;
    public:
        Animal(string nome, int idade);
        virtual ~Animal() {};
        string getNome() const;
        int getIdade() const;
        Veterinario * getVet() const;
        bool getTemVet() const;
        void setVet(Veterinario * vet);
        static int getMaisJovem();
        virtual bool eJovem() const = 0;
        virtual string getInformacao() const = 0;
};


class Cao: public Animal {
	string raca;
public:
	Cao(string nome, int idade, string raca);
    bool eJovem() const;
    string getInformacao() const;
};


class Voador {
    protected: //Em falta dantes?
        int velocidade_max;
        int altura_max;
    public:
        Voador(int vmax, int amax);
        virtual ~Voador() {};
};


class Morcego: public Animal, public Voador {
    public:
        Morcego(string nome, int idade, int vmax, int amax);
        bool eJovem() const;
        string getInformacao() const;
};

#endif /* SRC_ANIMAL_H_ */
