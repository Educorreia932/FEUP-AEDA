#include "../Headers/Menu.h"

#include <iostream>

using namespace std;

/*
void Menu(School SUPSchool) {
    cout << "1) Consultar aluguer material" << endl
         << "2) Consultar conta cliente" << endl
         << "3) Marcar aulas" << endl
         << "4) Horário de aulas, horário de professores" << endl
         << "0) Sair" << endl;
};
*/

int main() {
    vector<Client> Clients;
    vector<Material> Materials;

    School PortoSUPSchool(Clients, Materials);

    Menu SUPMenu(PortoSUPSchool);

    return 0;
}